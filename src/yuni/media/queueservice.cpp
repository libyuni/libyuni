/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#include "queueservice.h"
#include <cassert>
#include "../core/atomic/bool.h"
#include "../core/point3D.h"
#include "../core/vector3D.h"
#include "../thread/signal.h"
#include "../private/media/av.h"
#include "../private/media/file.h"
#include "../private/media/openal.h"

namespace Yuni
{
namespace Media
{

	//! Static to make sure only one manager is started
	static Atomic::Bool sHasRunningInstance = 0;


	QueueService::QueueService() :
		pReady(false),
		pMediaLoop(this)
	{
		library.pQueueService = this;
		emitter.pQueueService = this;
		emitter.pLibrary = &library;
	}


	QueueService::~QueueService()
	{
		if (pReady)
			stop();
	}


	bool QueueService::start()
	{
		if (sHasRunningInstance)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		// Do not initialize the manager twice
		if (pReady)
			return false;

		pMediaLoop.start();

		// Init media from the media loop
		{
			Thread::Signal signal;
			InitData initData(signal, pReady);
			Bind<bool()> callback;
			callback.bind(this, &QueueService::initDispatched, (InitData&)initData);
			pMediaLoop.dispatch(callback);
			// Wait for the initDispatched to finish
			signal.wait();
		}

		if (!pReady)
			return false;

		sHasRunningInstance = 1;
		return true;
	}


	void QueueService::stop()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		// Do not stop the manager if it was not properly started
		if (not pReady)
			return;

		pMediaLoop.beginClose();

		// Close OpenAL buffers properly
		library.clear();

		// Close OpenAL emitters properly
		{
			Emitter::Map::iterator sEnd = emitter.pEmitters.end();
			for (Emitter::Map::iterator it = emitter.pEmitters.begin(); it != sEnd; ++it)
			{
				Private::Media::OpenAL::DestroySource(it->second->id());
			}
		}
		emitter.pEmitters.clear();
		// Close OpenAL
		Yuni::Bind<bool()> callback;
		callback.bind(&Private::Media::OpenAL::Close);
		pMediaLoop.dispatch(callback);
		pMediaLoop.endClose();
		pReady = false;
		sHasRunningInstance = 0;
	}


	bool QueueService::initDispatched(InitData& data)
	{
		data.ready = Private::Media::AV::Init() and Private::Media::OpenAL::Init();
		// The variable must be considered as destroyed as soon as the method
		// 'notify()' is called
		bool ready = data.ready;
		data.signal.notify();
		return ready;
	}


	bool QueueService::loadSource(const String& filePath, bool video, bool audio, bool strict)
	{
		// Try to open the file
		Private::Media::File* file = new Private::Media::File(filePath);
		if (not file->valid())
		{
			delete file;
			return false;
		}

		Private::Media::Stream<Private::Media::stAudio>::Ptr aStream;
		Private::Media::Stream<Private::Media::stVideo>::Ptr vStream;

		if (audio)
		{
			// Try to get an audio stream from it
			aStream = file->getStream<Private::Media::stAudio>();
			if ((!aStream or !aStream->valid()) and strict)
			{
				delete file;
				return false;
			}
		}

		if (video)
		{
			// Try to get a video stream from it
			vStream = file->getStream<Private::Media::stVideo>();
			if ((!vStream or !vStream->valid()) and strict)
			{
				delete file;
				return false;
			}
		}

		// At that point, if we have no valid stream, we should fail,
		// even on the non-strict case.
		if (not (vStream and vStream->valid()) and not (aStream and aStream->valid()))
		{
			delete file;
			return false;
		}

		// Associate the source with the stream
		{
			ThreadingPolicy::MutexLocker locker(*this);
			Source::Ptr source = library.get(filePath);
			assert(nullptr != source);
			// Set the streams
			source->stream(vStream, aStream);
		}

		return true;
	}


	bool QueueService::updateDispatched()
	{
		const Emitter::Map::iterator end = emitter.pEmitters.end();
		for (Emitter::Map::iterator it = emitter.pEmitters.begin(); it != end; ++it)
			(it->second)->updateDispatched();
		return true;
	}


	bool QueueService::playing() const
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::const_iterator end = emitter.pEmitters.end();
		for (Emitter::Map::const_iterator it = emitter.pEmitters.begin(); it != end; ++it)
		{
			if (it->second->playing())
				return true;
		}
		return false;
	}



	///////////////////////////////// Emitters


	Emitter::Ptr QueueService::Emitters::get(const AnyString& name)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::iterator it = pEmitters.find(name);
		if (it == pEmitters.end())
			return nullptr;
		return it->second;
	}


	bool QueueService::Emitters::add(const AnyString& emitterName)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (not pQueueService->pReady)
			return false;

		// Create the emitter and add it
		Emitter::Ptr newEmitter(new Emitter());
		pEmitters[emitterName] = newEmitter;

		Media::Loop::RequestType callback;
 		callback.bind(newEmitter, &Emitter::prepareDispatched);
		// Dispatching...
 		pQueueService->pMediaLoop.dispatch(callback);

		return true;
	}


	bool QueueService::Emitters::attach(const AnyString& emitterName,
		const AnyString& sourceName)
	{
		Source::Ptr source = pLibrary->get(sourceName);
		if (not source)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (not pQueueService->pReady)
				return false;

			Emitter::Map::iterator it = pEmitters.find(emitterName);
			if (it == pEmitters.end())
				return false;
			Emitter::Ptr& emitter = it->second;

			Media::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachSourceDispatched, source);
			// Dispatching...
			pQueueService->pMediaLoop.dispatch(callback);
		}
		return true;
	}


	bool QueueService::Emitters::attach(Emitter::Ptr& emitter, const AnyString& sourceName)
	{
		if (not emitter or !sourceName)
			return false;

		Source::Ptr source = pLibrary->get(sourceName);
		if (not source)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (not pQueueService->pReady)
				return false;

			Media::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachSourceDispatched, source);
			// Dispatching...
			pQueueService->pMediaLoop.dispatch(callback);
		}
		return true;
	}


	bool QueueService::Emitters::attach(const AnyString& emitterName, Source::Ptr source)
	{
		if (not source)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (not pQueueService->pReady)
				return false;

			Emitter::Map::iterator it = pEmitters.find(emitterName);
			if (it == pEmitters.end())
				return false;
			Emitter::Ptr& emitter = it->second;

			assert(!(!emitter) and "invalid emitter");
			Media::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachSourceDispatched, source);
			// Dispatching...
	 		pQueueService->pMediaLoop.dispatch(callback);
		}
		return true;
	}


	bool QueueService::Emitters::attach(Emitter::Ptr& emitter, Source::Ptr source)
	{
		if (not emitter or not source)
			return false;

		ThreadingPolicy::MutexLocker locker(*this);
		{
			if (not pQueueService->pReady)
				return false;

			Media::Loop::RequestType callback;
			callback.bind(emitter, &Emitter::attachSourceDispatched, source);
			// Dispatching...
	 		pQueueService->pMediaLoop.dispatch(callback);
		}
		return true;
	}


	void QueueService::Emitters::detach(const AnyString& name)
	{
		return detach(get(name));
	}


	void QueueService::Emitters::detach(Emitter::Ptr emitter)
	{
		if (not emitter)
			return;

		Media::Loop::RequestType callback;
 		callback.bind(emitter, &Emitter::detachSourceDispatched);
		// Dispatching...
 		pQueueService->pMediaLoop.dispatch(callback);
	}


	bool QueueService::Emitters::move(const AnyString& emitterName,
		const Point3D<>& position)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::iterator it = pEmitters.find(emitterName);
		if (it == pEmitters.end())
			return false;
		it->second->position(position);
		return true;
	}


	bool QueueService::Emitters::move(const AnyString& emitterStr, const Point3D<>& position,
		const Vector3D<>& velocity, const Vector3D<>& direction)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Emitter::Map::iterator it = pEmitters.find(emitterStr);
		if (it == pEmitters.end())
			return false;

 		it->second->position(position);
 		it->second->velocity(velocity);
 		it->second->direction(direction);
		return true;
	}


	bool QueueService::Emitters::playing(const AnyString& name)
	{
		return playing(get(name));
	}


	bool QueueService::Emitters::playing(Emitter::Ptr emitter)
	{
		if (not emitter)
			return false;
		return emitter->playing();
	}


	bool QueueService::Emitters::paused(const AnyString& name)
	{
		return paused(get(name));
	}


	bool QueueService::Emitters::paused(Emitter::Ptr emitter)
	{
		if (not emitter)
			return false;
		return emitter->paused();
	}


	bool QueueService::Emitters::play(Emitter::Ptr emitter)
	{
		if (not emitter)
			return false;
		Media::Loop::RequestType callback;
 		callback.bind(emitter, &Emitter::playSourceDispatched);
		// Dispatching...
 		pQueueService->pMediaLoop.dispatch(callback);
		return true;
	}


	bool QueueService::Emitters::pause(Emitter::Ptr emitter)
	{
		if (not emitter)
			return false;
		Media::Loop::RequestType callback;
 		callback.bind(emitter, &Emitter::pauseSourceDispatched);
		// Dispatching...
 		pQueueService->pMediaLoop.dispatch(callback);
		return true;
	}


	bool QueueService::Emitters::stop(const Emitter::Ptr& emitter)
	{
		if (not emitter)
			return false;
		Media::Loop::RequestType callback;
 		callback.bind(emitter, &Emitter::stopSourceDispatched);
		// Dispatching...
 		pQueueService->pMediaLoop.dispatch(callback);
		return true;
	}



	////////////////////////// Library

	Source::Ptr QueueService::Library::get(const AnyString& name)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Source::Map::iterator it = pSources.find(name);
		if (it == pSources.end())
			return nullptr;
		return it->second;
	}

	const Source::Ptr QueueService::Library::get(const AnyString& name) const
	{
		ThreadingPolicy::MutexLocker locker(*this);

		Source::Map::const_iterator it = pSources.find(name);
		if (it == pSources.end())
			return nullptr;
		return it->second;
	}


	void QueueService::Library::clear()
	{
		ThreadingPolicy::MutexLocker locker(*this);

		{
			Thread::Signal signal;
			Yuni::Bind<bool()> callback;
			Source::Map::iterator bEnd = pSources.end();
			for (Source::Map::iterator it = pSources.begin(); it != bEnd; ++it)
			{
				// We have to pass a pointer here, otherwise bind() will call the copy ctor
				callback.bind(it->second, &Source::destroyDispatched, &signal);
				pQueueService->pMediaLoop.dispatch(callback);
				signal.wait();
				signal.reset();
			}
		}
		pSources.clear();
	}


	bool QueueService::Library::load(const AnyString& filePath)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (not pQueueService->pReady)
			return false;

		// Create the source, store it in the map
		pSources[filePath] = new Source();

		// Yuni::Bind<bool()> callback;
		// callback.bind(pQueueService, &QueueService::loadSourceDispatched, filePath);
		// pQueueService->pMediaLoop.dispatch(callback);
		if (not pQueueService->loadSource(filePath, true, true, false))
		{
			pSources.erase(filePath);
			return false;
		}
		return true;
	}

	bool QueueService::Library::loadSound(const AnyString& filePath)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (not pQueueService->pReady)
			return false;

		// Create the source, store it in the map
		pSources[filePath] = new Source();

		// Yuni::Bind<bool()> callback;
		// callback.bind(pQueueService, &QueueService::loadSourceDispatched, filePath);
		// pQueueService->pMediaLoop.dispatch(callback);
		if (not pQueueService->loadSource(filePath, false, true, true))
		{
			pSources.erase(filePath);
			return false;
		}
		return true;
	}


	bool QueueService::Library::loadVideo(const AnyString& filePath)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (not pQueueService->pReady)
			return false;

		// Create the source, store it in the map
		pSources[filePath] = new Source();

		// Yuni::Bind<bool()> callback;
		// callback.bind(pQueueService, &QueueService::loadSourceDispatched, filePath);
		// pQueueService->pMediaLoop.dispatch(callback);
		if (not pQueueService->loadSource(filePath, true, false, true))
		{
			pSources.erase(filePath);
			return false;
		}
		return true;
	}



	bool QueueService::Library::unload(const AnyString& name)
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (not pQueueService->pReady)
			return false;

		Source::Map::iterator it = pSources.find(name);
		if (pSources.end() == it)
			return false;

		{
			Thread::Signal signal;
			Yuni::Bind<bool()> callback;

			callback.bind(it->second, &Source::destroyDispatched, &signal);
			pQueueService->pMediaLoop.dispatch(callback);
			signal.wait();
		}
		// Wait for data to be properly unloaded before removing from the list
		pSources.erase(it);

		return true;
	}


	uint QueueService::Library::duration(const AnyString& name) const
	{
		Source::Ptr source = get(name);
		if (not source)
			return 0;

		ThreadingPolicy::MutexLocker locker(*this);
		if (not pQueueService->pReady)
			return 0;
		return source->duration();
	}


	uint QueueService::Library::samplingRate(const AnyString& name) const
	{
		Source::Ptr source = get(name);
		if (not source)
			return 0;

		ThreadingPolicy::MutexLocker locker(*this);
		if (not pQueueService->pReady)
			return 0;
		if (!source->hasAudio())
			return 0;
		return source->samplingRate();
	}


	uint QueueService::Library::channels(const AnyString& name) const
	{
		Source::Ptr source = get(name);
		if (not source)
			return 0;

		ThreadingPolicy::MutexLocker locker(*this);
		if (not pQueueService->pReady)
			return 0;
		if (!source->hasAudio())
			return 0;
		return source->channels();
	}


	uint QueueService::Library::bitsPerSample(const AnyString& name) const
	{
		Source::Ptr source = get(name);
		if (not source)
			return 0;

		ThreadingPolicy::MutexLocker locker(*this);
		if (not pQueueService->pReady)
			return 0;
		if (!source->hasAudio())
			return 0;
		return source->bitsPerSample();
	}


	float QueueService::Library::elapsedTime(const AnyString& name) const
	{
		Source::Ptr source = get(name);
		if (not source)
			return 0;

		ThreadingPolicy::MutexLocker locker(*this);
		if (not pQueueService->pReady)
			return 0;
		return source->elapsedTime();
	}


	bool QueueService::running() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pReady and pMediaLoop.running();
	}






} // namespace Media
} // namespace Yuni

