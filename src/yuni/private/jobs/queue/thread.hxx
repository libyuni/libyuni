#pragma once
#include "thread.h"



namespace Yuni
{
namespace Private
{
namespace QueueService
{

	inline QueueThread::QueueThread(Yuni::Job::QueueService& queueservice)
		: pQueueService(queueservice)
	{
	}


	inline QueueThread::~QueueThread()
	{
		// Ensure that the thread is really stopped
		stop();
	}


	inline Yuni::Job::IJob::Ptr QueueThread::currentJob() const
	{
		return pJob;
	}


	inline void WaitingRoom::add(const Yuni::Job::IJob::Ptr& job)
	{
		add(job, Yuni::Job::priorityDefault);
	}





} // namespace QueueService
} // namespace Private
} // namespace Yuni
