#ifndef __YUNI_JOB_SCHEDULER_HIGHEST_PRIORITY_FIRST_HXX__
# define  __YUNI_JOB_SCHEDULER_HIGHEST_PRIORITY_FIRST_HXX__


namespace Yuni
{
namespace Job
{
namespace Scheduler
{


	inline bool HighestPriorityFirst::idle() const
	{
		return (0 == pWorkerCount);
	}


	inline uint HighestPriorityFirst::schedulerThreadCount() const
	{
		return pThreads.count();
	}


	inline void HighestPriorityFirst::schedulerIncrementWorkerCount()
	{
		++pWorkerCount;
	}


	inline void HighestPriorityFirst::schedulerDecrementWorkerCount()
	{
		--pWorkerCount;
	}


	inline void HighestPriorityFirst::schedulerNotifyNewJobInWaitingRoom(Yuni::Job::Priority)
	{
		pThreads.wakeUp();
	}


	inline uint HighestPriorityFirst::minimumThreadCount() const
	{
		return pMaximumThreadCount;
	}


	inline bool HighestPriorityFirst::minimumThreadCount(uint)
	{
		return false;
	}


	inline uint HighestPriorityFirst::maximumThreadCount() const
	{
		return pMaximumThreadCount;
	}


	inline bool HighestPriorityFirst::nextJob(IJob::Ptr& out)
	{
		while (not pWaitingRoom.empty())
		{
			if (pWaitingRoom.hasJob[priorityHigh])
			{
				if (pWaitingRoom.pop(out, priorityHigh))
					return true;
				continue;
			}
			if (pWaitingRoom.hasJob[priorityDefault])
			{
				if (pWaitingRoom.pop(out, priorityDefault))
					return true;
				continue;
			}
			if (pWaitingRoom.hasJob[priorityLow])
			{
				if (pWaitingRoom.pop(out, priorityLow))
					return true;
				continue;
			}
		}
		return false;
	}





} // namespace Scheduler
} // namespace Job
} // namespace Yuni

#endif //  __YUNI_JOB_SCHEDULER_HIGHEST_PRIORITY_FIRST_HXX__
