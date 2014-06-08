#include "loop.h"
#include "queueservice.h"

namespace Yuni
{
namespace Media
{

	Loop::Loop(QueueService* queueservice)
		: pMediaService(queueservice)
		, pClosing(false)
	{
		timeout(50);
	}


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
		if (not pMediaService or pClosing)
			return false;

		pMediaService->updateDispatched();
		return true;
	}






} // namespace Media
} // namespace Yuni
