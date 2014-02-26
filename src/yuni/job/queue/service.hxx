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
		return stop(timeout) and start();
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





} // namespace Job
} // namespace Yuni

#endif // __YUNI_JOB_QUEUE_SERVICE_H__
