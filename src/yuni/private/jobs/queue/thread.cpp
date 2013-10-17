
#include "thread.h"



namespace Yuni
{
namespace Private
{
namespace QueueService
{


	static inline bool FetchNextJob(Yuni::Job::IJob::Ptr& out, Yuni::Private::QueueService::WaitingRoom& waitingRoom)
	{
		using namespace ::Yuni::Job;

		while (not waitingRoom.empty())
		{
			if (waitingRoom.hasJob[priorityHigh])
			{
				if (waitingRoom.pop(out, priorityHigh))
					return true;
				continue;
			}

			// medium priority
			if (waitingRoom.hasJob[priorityDefault])
			{
				if (waitingRoom.pop(out, priorityDefault))
					return true;
				continue;
			}

			// low
			if (waitingRoom.hasJob[priorityLow])
			{
				if (waitingRoom.pop(out, priorityLow))
					return true;
				continue;
			}
		}
		return false;
	}



	bool QueueThread::onExecute()
	{
		// assert
		assert(this != NULL and "Queue: Thread: Oo `this' is null !?");

		// Notify the scheduler that this thread has begun its work
		++pScheduler.pWorkerCountInActiveDuty;

		// Asking for the next job
		while (FetchNextJob(pJob, pScheduler.pWaitingRoom))
		{
			// Execute the job, via a wrapper for symbol visibility issues
			Yuni::Private::QueueService::JobAccessor<Yuni::Job::IJob>::Execute(*pJob, this);

			// We must release our pointer to the job here to avoid its destruction
			// in `pScheduler.nextJob()` (when `pJob` is re-assigned).
			// This method uses a lock and the destruction of the job may take some time.
			// Obviously, there is absolutely no guarantee that the job will be destroyed
			// at this point but we don't really care
			pJob = nullptr;

			// Cancellation point
			if (shouldAbort())
			{
				// Notify the scheduler that this thread goes to sleep
				--pScheduler.pWorkerCountInActiveDuty;
				// We have to stop as soon as possible, no need for hibernation
				return false;
			}
		} // loop for retrieving jobs to execute


		// Notify the scheduler that this thread goes to sleep
		--pScheduler.pWorkerCountInActiveDuty;

		// Returning true, for hibernation
		return true;
	}


	void QueueThread::onKill()
	{
		if (!(!(pJob)))
		{
			// Notify the job that it has been killed
			// (via the wrapper for symbol visibility issues)
			Yuni::Private::QueueService::JobAccessor<Yuni::Job::IJob>::ThreadHasBeenKilled(*pJob);
			// Release the pointer, if possible of course
			pJob = nullptr;
		}
	}






} // namespace QueueService
} // namespace Private
} // namespace Yuni

