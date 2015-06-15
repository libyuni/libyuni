#pragma once
#include "service.h"
#include "../../thread/timer.h"



namespace Yuni
{
namespace Job
{

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
		MutexLocker locker(*this);
		return pWorkerSet.empty();
	}


	inline bool QueueService::started() const
	{
		MutexLocker locker(*this);
		return (pStatus != sStopped);
	}





} // namespace Job
} // namespace Yuni
