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
#include "../core/math.h"
#include "../core/system/gettimeofday.h"
#include "emitter.h"
#include "../private/media/av.h"
#include "../private/media/openal.h"
#include <iostream>


namespace Yuni
{
namespace Media
{

	const float Emitter::DefaultPitch = 1.0f;
	const float Emitter::DefaultGain  = 1.0f;




	bool Emitter::attachSourceDispatched(Source::Ptr& source)
	{
		// Check source validity
		if (!source || !source->valid())
		{
			std::cerr << "Invalid Source !" << std::endl;
			return false;
		}

		pSource = source;
		if (!pSource->prepareDispatched(pID))
		{
			std::cerr << "Failed loading sources !" << std::endl;
			return false;
		}
		return true;
	}


	bool Emitter::detachSourceDispatched()
	{
		if (!pReady || !pSource)
			return false;

		stopSourceDispatched();

		Private::Media::OpenAL::UnbindBufferFromSource(pID);
		pSource = nullptr;
		return true;
	}


	bool Emitter::playSourceDispatched()
	{
		if (!pSource)
			return false;

		pPlaying = Private::Media::OpenAL::PlaySource(pID);
		if (!pPlaying)
		{
			std::cerr << "Emitter " << pID << " failed playing !" << std::endl;
			Private::Media::OpenAL::UnqueueBufferFromSource(pID);
			return false;
		}
		// Store start time
		Yuni::timeval now;
		YUNI_SYSTEM_GETTIMEOFDAY(&now, NULL);
		pStartTime = now.tv_sec;
		return true;
	}


	bool Emitter::playSourceDispatched(Source::Ptr& source)
	{
		if (!pReady && !prepareDispatched())
			return false;

		if (!attachSourceDispatched(source))
			return false;

		return playSourceDispatched();
	}


	bool Emitter::pauseSourceDispatched()
	{
		if (!pPlaying)
			return false;

		Private::Media::OpenAL::PauseSource(pID);
		return true;
	}


	bool Emitter::stopSourceDispatched()
	{
		if (!pPlaying && !pPaused)
			return false;

		Private::Media::OpenAL::StopSource(pID);
		return pSource->rewindDispatched(pID);
	}


	bool Emitter::updateDispatched()
	{
		if (!pReady)
			return false;
		pPlaying = Private::Media::OpenAL::IsSourcePlaying(pID);
		pPaused = Private::Media::OpenAL::IsSourcePaused(pID);
		// If not playing, nothing else to do
		if (!pPlaying)
			return false;
		if (pModified)
		{
			if (!Private::Media::OpenAL::MoveSource(pID, pPosition, pVelocity, pDirection))
			{
				std::cerr << "Source position update failed !" << std::endl;
				return false;
			}
			if (!Private::Media::OpenAL::ModifySource(pID, DefaultPitch, pGain, DefaultAttenuation, false))
			{
				std::cerr << "Source characteristics update failed !" << std::endl;
				return false;
			}
		}
		if (pSource)
		{
			if (!pSource->updateDispatched(pID))
			{
				if (pLoop)
				{
					pSource->rewindDispatched(pID);
					pSource->updateDispatched(pID);
				}
			}
		}
		return true;
	}


	bool Emitter::prepareDispatched()
	{
		if (pReady)
			return true;

		unsigned int source = Private::Media::OpenAL::CreateSource(pPosition, pVelocity,
			pDirection, DefaultPitch, pGain, DefaultAttenuation, pLoop);

		pID = source;
		pReady = (source > 0);
		return pReady;
	}


	float Emitter::elapsedTime() const
	{
		ThreadingPolicy::MutexLocker locker(*this);

		if (!pPlaying)
			return 0;
		return pSource->elapsedTime();
		// Yuni::timeval now;
		// YUNI_SYSTEM_GETTIMEOFDAY(&now, NULL);
		// return now.tv_sec - pStartTime;
	}


} // namespace Media
} // namespace Yuni
