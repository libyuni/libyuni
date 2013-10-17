#ifndef __YUNI_PRIVATE_JOBS_QUEUE_THREAD_HXX__
# define __YUNI_PRIVATE_JOBS_QUEUE_THREAD_HXX__


namespace Yuni
{
namespace Private
{
namespace QueueService
{

	inline QueueThread::QueueThread(Yuni::Job::QueueService& scheduler) :
		pScheduler(scheduler)
	{}


	inline QueueThread::~QueueThread()
	{
		// Ensure that the thread is really stopped
		stop();
	}


	inline Yuni::Job::IJob::Ptr QueueThread::currentJob() const
	{
		return pJob;
	}





} // namespace QueueService
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_JOBS_QUEUE_THREAD_HXX__
