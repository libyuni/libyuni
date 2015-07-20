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
# include "../private/media/openal.h"
# include "source.h"
# include "../private/media/frame.h"

namespace Yuni
{
namespace Media
{

	uint Source::fillBuffer()
	{
		pData.clear();
		uint size = 0;
		while (size < minBufferSize)
		{
			// Make sure we get some data to give to the buffer
			Private::Media::Frame::Ptr frame = pAStream->nextFrame();
			if (!frame)
				return size;
			uint count = frame->audioSize();
			if (!count)
				return size;
			count = Math::Min(count, (uint)maxBufferSize - size);
			pData.append((const char*)frame->audioData(), count);
			size += count;
		}
		return size;
	}


	void Source::fillQueue()
	{
		for (uint i = pFrames.size(); i < bestQueueSize; )
		{
			Private::Media::Frame::Ptr frame = pVStream->nextFrame();
			if (!frame)
				return;

			if (frame->valid())
			{
				pFrames.push_back(frame);
				++i;
			}
		}
	}


	bool Source::prepareDispatched(uint source)
	{
		if (!pAStream and !pVStream)
			return false;

		// Audio
		if (nullptr != pAStream)
		{
			pBufferCount = 0;
			for (uint i = 0; i < maxBufferCount; ++i)
			{
				uint size = fillBuffer();
				if (!size)
					return false;

				if (!Private::Media::OpenAL::CreateBuffer(pIDs + i))
					return false;

				++pBufferCount;

				// Buffer the data with OpenAL
				if (!Private::Media::OpenAL::SetBufferData(pIDs[i], pAStream->alFormat(),
					pData.data(), size, pAStream->rate() / pAStream->channels()))
					return false;
				// Queue the buffers onto the source
				if (!Private::Media::OpenAL::QueueBufferToSource(pIDs[i], source))
					return false;
				// No more data to read
				if (size < minBufferSize)
					break;
			}
		}

		// Video
		if (nullptr != pVStream)
		{
			// fillQueue();
			// if (not pFrames.size())
			// 	return false;
		}

		return true;
	}


	bool Source::updateDispatched(uint source)
	{
		if (!valid())
			return false;

		// Audio
		if (hasAudio())
		{
			// Update time progression in the current buffer
			::alGetSourcef(source, AL_SEC_OFFSET, &pSecondsCurrent);
			// Check if a buffer has finished playing
			ALint processed = 0;
			::alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
			if (!processed)
				return true;

			// A buffer has finished playing, unqueue it
			ALuint buffer = Private::Media::OpenAL::UnqueueBufferFromSource(source);
			// Reset current buffer time
			pSecondsCurrent = 0.0f;
			uint bits = pAStream->bits();
			uint channels = pAStream->channels();
			uint frequency = pAStream->rate();
			int bufferSize;
			::alGetBufferi(buffer, AL_SIZE, &bufferSize);
			pSecondsElapsed += bufferSize * 8.0f / bits / frequency;
			// Get the next data to feed the buffer
			uint size = fillBuffer();
			if (!size)
				return false;

			// Buffer the data with OpenAL and queue the buffer onto the source
			if (!Private::Media::OpenAL::SetBufferData(buffer, pAStream->alFormat(), pData.data(),
				size, frequency / channels))
				return false;
			if (!Private::Media::OpenAL::QueueBufferToSource(buffer, source))
				return false;
		}

		// Video
		if (hasVideo())
		{
			/*
			if (hasAudio() and Private::Media::OpenAL::IsSourcePlaying(source))
			{
				std::cout << "Video and audio sync !" << std::endl;
				// Try to sync with audio
				ALfloat elapsed;
				::alGetSourcef(source, AL_SEC_OFFSET, &elapsed);
				while (!pFrames.empty() and elapsed > pFrames.front()->timestamp())
				{
					pFrames.pop_front();
					if (pFrames.empty())
						fillQueue();
				}
			}
			*/

			// TEMPORARY
			// The sync code is not working yet, just get some frames when we need them for now
			// if (pFrames.empty())
			// 	fillQueue();

			if (pFrames.empty())
				// Failed to load anymore
				return false;
		}

		return true;
	}


	bool Source::rewindDispatched(uint source)
	{
		for (uint i = 0; i < pBufferCount; ++i)
			Private::Media::OpenAL::UnqueueBufferFromSource(source);
		Private::Media::OpenAL::UnbindBufferFromSource(source);
		Private::Media::OpenAL::DestroyBuffers(pBufferCount, pIDs);
		if (pAStream)
			pAStream->rewind();
		if (pVStream)
			pVStream->rewind();
		return prepareDispatched(source);
	}


	bool Source::destroyDispatched(Thread::Signal* signal)
	{
		if (!pAStream and !pVStream)
			return false;

		pFrames.clear();
		if (pAStream)
		{
			delete pAStream->parent();
			Private::Media::OpenAL::DestroyBuffers(pBufferCount, pIDs);
			pBufferCount = 0;
		}
		else
			delete pVStream->parent();
		pAStream = nullptr;
		pVStream = nullptr;
		if (signal)
			signal->notify();
		return true;
	}


	Private::Media::Frame::Ptr Source::nextFrame()
	{
		assert(hasVideo() && "Source::nextFrame : Cannot get a video frame from an audio-only source !");
		// FIXME : There is probably a race condition here !
		// We need a mutex lock
		if (pFrames.empty())
		{
			return pVStream->nextFrame();
		}
		auto frame = pFrames.front();
		pFrames.pop_front();
		return frame;
	}



} // namespace Media
} // namespace Yuni
