
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
		return true;
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
		//std::cout << "Emitter still playing !" << std::endl;
		if (pModified)
		{
			if (!Private::Media::OpenAL::MoveSource(pID, pPosition, pVelocity, pDirection))
			{
				std::cerr << "Source position update failed !" << std::endl;
				return false;
			}
			if (!Private::Media::OpenAL::ModifySource(pID, DefaultPitch, pGain, DefaultAttenuation, pLoop))
			{
				std::cerr << "Source characteristics update failed !" << std::endl;
				return false;
			}
		}
		if (pSource)
		{
			//std::cout << "Emitter source still ok !" << std::endl;
			pSource->updateDispatched(pID);
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
