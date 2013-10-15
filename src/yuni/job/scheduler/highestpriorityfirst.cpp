
# include "highestpriorityfirst.h"
#include "../../core/system/cpu.h"


namespace Yuni
{
namespace Job
{
namespace Scheduler
{


	HighestPriorityFirst::HighestPriorityFirst(Private::QueueService::WaitingRoom& room) :
		pWaitingRoom(room),
		pStarted(0),
		pMaximumThreadCount(2)
	{
	}


	HighestPriorityFirst::~HighestPriorityFirst()
	{
		if (pStarted)
		{
			pStarted = 0;
			pServiceMutex.lock();
			pThreads.stop();
			pServiceMutex.unlock();
		}
	}


	bool HighestPriorityFirst::schedulerStart()
	{
		if (!pStarted)
		{
			pStarted = 1;
			pServiceMutex.lock();

			// Creating all threads we need
			pThreads.clear();
			for (uint i = 0; i != pMaximumThreadCount; ++i)
				pThreads += new Yuni::Private::Jobs::QueueThread<SchedulerType>(*this);

			// Starting all threads at once
			pThreads.start();
			pServiceMutex.unlock();
		}
		return true;
	}


	bool HighestPriorityFirst::schedulerStop(uint timeout)
	{
		if (pStarted)
		{
			pStarted = 0;
			pServiceMutex.lock();
			// Stopping all threads
			pThreads.stop(timeout);
			pThreads.clear();
			pServiceMutex.unlock();
		}
		return true;
	}


	bool HighestPriorityFirst::maximumThreadCount(uint count)
	{
		if (count > 512)
			return false;

		if (count == 0)
		{
			// automatic discovery
			count = System::CPU::Count();
			if (count < 1)
				count = 1;
			else if (count > 2)
				--count; // keep one core for other tasks
		}

		pMaximumThreadCount = count;
		return true;
	}




} // namespace Scheduler
} // namespace Job
} // namespace Yuni

