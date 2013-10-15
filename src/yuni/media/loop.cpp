#include "loop.h"
#include "queueservice.h"

namespace Yuni
{
namespace Media
{

	Loop::Loop(QueueService* mediaService):
		pMediaService(mediaService),
		pClosing(false)
	{}

	void Loop::beginClose()
	{
		pClosing = true;
	}

	void Loop::endClose()
	{
		stop();
	}


	bool Loop::onLoop()
	{
		if (!pMediaService || pClosing)
			return false;
		pMediaService->updateDispatched();
		return true;
	}



} // namespace Media
} // namespace Yuni
