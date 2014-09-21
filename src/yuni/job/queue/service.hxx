#ifndef __YUNI_JOB_QUEUE_SERVICE_HXX__
# define __YUNI_JOB_QUEUE_SERVICE_HXX__

# include "../../thread/timer.h"
# include <iostream>


namespace Yuni
{
namespace Job
{

	inline bool QueueService::restart(uint timeout)
	{
		stop(timeout);
		return start();
	}


	inline uint QueueService::waitingJobsCount() const
	{
		return pWaitingRoom.size();
	}


	inline QueueService& QueueService::operator += (IJob* job)
	{
		add(job);
		return *this;
	}


	inline QueueService& QueueService::operator << (IJob* job)
	{
		add(job);
		return *this;
	}


	inline QueueService& QueueService::operator += (const IJob::Ptr& job)
	{
		add(job);
		return *this;
	}


	inline QueueService& QueueService::operator << (const IJob::Ptr& job)
	{
		add(job);
		return *this;
	}


	inline bool QueueService::idle() const
	{
		return (0 == pWorkerCountInActiveDuty);
	}


	inline bool QueueService::started() const
	{
		MutexLocker locker(*this);
		return (pStatus != sStopped);
	}





} // namespace Job
} // namespace Yuni

#endif // __YUNI_JOB_QUEUE_SERVICE_H__
