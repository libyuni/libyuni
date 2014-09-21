
#include "thread.h"



namespace Yuni
{
namespace Private
{
namespace QueueService
{

	namespace // anonymous
	{

		static inline
		bool FetchNextJob(Yuni::Job::IJob::Ptr& out, Yuni::Private::QueueService::WaitingRoom& waitingRoom)
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


	} // anonymous namespace






	inline void QueueThread::notifyEndOfWork()
	{
		// Notify the scheduler that this thread goes to sleep
		if (0 == --pQueueService.pWorkerCountInActiveDuty)
			pQueueService.onAllThreadsHaveStopped();
	}


	bool QueueThread::onExecute()
	{
		// Notify the scheduler that this thread has begun its work
		++pQueueService.pWorkerCountInActiveDuty;

		// Asking for the next job
		while (FetchNextJob(pJob, pQueueService.pWaitingRoom))
		{
			// Execute the job, via a wrapper for symbol visibility issues
			Yuni::Private::QueueService::JobAccessor<Yuni::Job::IJob>::Execute(*pJob, this);

			// We must release our pointer to the job here to avoid its destruction
			// in `pQueueService.nextJob()` (when `pJob` is re-assigned).
			// This method uses a lock and the destruction of the job may take some time.
			// Obviously, there is absolutely no guarantee that the job will be destroyed
			// at this point but we don't really care
			pJob = nullptr;

			// Cancellation point
			if (YUNI_UNLIKELY(shouldAbort())) // We have to stop as soon as possible, no need for hibernation
				return false;

		} // loop for retrieving jobs to execute

		// Returning true, for hibernation
		return true;
	}


	void QueueThread::onStop()
	{
		notifyEndOfWork(); // we are done here !
	}


	void QueueThread::onPause()
	{
		notifyEndOfWork(); // we are done here !
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

		notifyEndOfWork(); // we are done here !
	}






} // namespace QueueService
} // namespace Private
} // namespace Yuni

