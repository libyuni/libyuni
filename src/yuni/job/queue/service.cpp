
#include "service.h"
#include "../../core/system/cpu.h"
#include "../../thread/array.h"
#include "../../private/jobs/queue/thread.h"



namespace Yuni
{
namespace Job
{

	enum
	{
		maxNumberOfThreads = 512,
	};


	typedef Yuni::Thread::Array<Yuni::Private::QueueService::QueueThread>  ThreadArray;



	static inline uint OptimalCPUCount()
	{
		uint count = System::CPU::Count();
		if (count < 1)
			return 1;
		if (count > 2)
			return count - 1;
		return count;
	}







	QueueService::QueueService() :
		pThreads(((void*) new ThreadArray()))
	{
		uint count = OptimalCPUCount();
		pMinimumThreadCount = count;
		pMaximumThreadCount = count;
	}


	QueueService::QueueService(bool autostart) :
		pThreads(((void*) new ThreadArray()))
	{
		uint count = OptimalCPUCount();
		pMinimumThreadCount = count;
		pMaximumThreadCount = count;

		if (autostart)
			start();
	}


	QueueService::~QueueService()
	{
		if (pStarted)
		{
			// wait for the execution of all jobs
			wait();

			ThreadArray* threads = nullptr; // the thread pool

			// remove the reference to avoid useless mutex locks
			{
				MutexLocker locker(*this);
				threads = (ThreadArray*) pThreads;
				pThreads = nullptr;
			}

			// Destroying the thread pool
			if (threads)
			{
				threads->stop(defaultTimeout);
				delete threads;
			}
		}
		else
			delete ((ThreadArray*) pThreads);
	}


	bool QueueService::maximumThreadCount(uint count)
	{
		if (count > maxNumberOfThreads) // hard-coded value
			return false;
		if (0 == count)
			count = OptimalCPUCount();

		MutexLocker locker(*this);

		// checking for the lower bound
		if (pMinimumThreadCount > count)
			pMinimumThreadCount = count;
		// reseting the upper bound
		pMaximumThreadCount = count;
		return true;
	}


	uint QueueService::maximumThreadCount() const
	{
		MutexLocker locker(*this);
		return pMaximumThreadCount;
	}


	bool QueueService::minimumThreadCount(uint count)
	{
		if (count > maxNumberOfThreads) // hard-coded value
			return false;
		if (0 == count)
			count = OptimalCPUCount();

		MutexLocker locker(*this);

		// checking for the upper bound
		if (pMaximumThreadCount < count)
			pMaximumThreadCount = count;
		// reseting the lower bound
		pMinimumThreadCount = count;
		return true;
	}


	uint QueueService::minimumThreadCount() const
	{
		MutexLocker locker(*this);
		return pMinimumThreadCount;
	}


	std::pair<uint,uint> QueueService::minmaxThreadCount() const
	{
		MutexLocker locker(*this);
		return std::pair<uint, uint>(pMinimumThreadCount, pMaximumThreadCount);
	}


	bool QueueService::minmaxThreadCount(const std::pair<uint, uint>& values)
	{
		uint maxv = values.second;
		if (maxv > maxNumberOfThreads)
			return false;
		uint minv = values.first;
		if (minv > maxv)
			minv = maxv;
		if (maxv == 0)
		{
			maxv = OptimalCPUCount();
			minv = maxv;
		}

		MutexLocker locker(*this);
		pMinimumThreadCount = minv;
		pMaximumThreadCount = maxv;
		return true;
	}


	bool QueueService::start()
	{
		MutexLocker locker(*this);
		if (not pStarted)
		{
			// alias to the thread pool
			ThreadArray& array = *((ThreadArray*) pThreads);

			// recreate the thread pool
			// adding the minimum number of threads
			array.clear();
			for (uint i = 0; i != pMinimumThreadCount; ++i)
				array += new Yuni::Private::QueueService::QueueThread(*this);

			// Start all threads at once
			array.start();

			// Ok now we have started
			pStarted = true;
		}
		return true;
	}


	bool QueueService::stop(uint timeout)
	{
		if (pStarted)
		{
			ThreadArray* threads; // the thread pool

			// The method stop must avoid as much as possible to lock the inner mutex
			{
				MutexLocker locker(*this);
				if (not pStarted)
					return true;

				threads = (ThreadArray*) pThreads;
				pThreads = nullptr;

				// The service is now really stopped
				pStarted = false;
			}

			// Destroying the thread pool
			if (threads)
			{
				threads->stop(timeout);
				delete threads;
			}
		}
		return true;
	}



	namespace // anonymous
	{

		class QueueServiceWaitHelper final : public Thread::Timer
		{
		public:
			QueueServiceWaitHelper(QueueService& scheduler, Yuni::Private::QueueService::WaitingRoom& room,
				uint pollInterval) :
				Thread::Timer(pollInterval),
				pRoom(room),
				pScheduler(scheduler),
				pStatus(false)
			{}
			virtual ~QueueServiceWaitHelper()
			{
				stop();
			}

			bool status() const {return pStatus;}

		protected:
			virtual bool onInterval(uint) override
			{
				// Checking if the scheduler still has workers
				if (pRoom.empty() and pScheduler.idle())
				{
					pStatus = true;
					// We can stop now
					return false;
				}
				// Continuing...
				return true;
			}

		private:
			//! Reference to the waiting room
			Yuni::Private::QueueService::WaitingRoom& pRoom;
			//! Reference to the scheduler
			QueueService& pScheduler;
			//! The returned status
			bool pStatus;

		}; // class QueueServiceWaitHelper

	} // anonymous namespace



	void QueueService::wait()
	{
		// TODO QueueService::wait: Find a more efficient way for doing this
		if (not pWaitingRoom.empty() or not idle())
		{
			QueueServiceWaitHelper helper(*this, pWaitingRoom, 150);
			helper.start();
			helper.wait();
		}
	}


	bool QueueService::wait(uint timeout, uint pollInterval)
	{
		// TODO QueueService::wait: Find a more efficient way for doing this
		if (not pWaitingRoom.empty() or not idle())
		{
			QueueServiceWaitHelper helper(*this, pWaitingRoom, pollInterval);
			helper.start();
			helper.wait(timeout);
			return helper.status();
		}
		return true;
	}


	void QueueService::add(IJob* job)
	{
		if (job)
		{
			pWaitingRoom.add(job);
			((ThreadArray*) pThreads)->wakeUp();
		}
	}


	void QueueService::add(const IJob::Ptr& job)
	{
		if (!(!job))
		{
			pWaitingRoom.add(job);
			((ThreadArray*) pThreads)->wakeUp();
		}
	}


	void QueueService::add(const IJob::Ptr& job, Priority priority)
	{
		if (!(!job))
		{
			pWaitingRoom.add(job, priority);
			((ThreadArray*) pThreads)->wakeUp();
		}
	}


	void QueueService::add(IJob* job, Priority priority)
	{
		if (!(!job))
		{
			pWaitingRoom.add(job, priority);
			((ThreadArray*) pThreads)->wakeUp();
		}
	}


	uint QueueService::threadCount() const
	{
		return ((const ThreadArray*) pThreads)->size();
	}




	namespace // anonymous
	{

		class QueueActivityPredicate final
		{
		public:
			//!
			typedef typename QueueService::ThreadInfo ThreadInfoType;
			typedef typename ThreadInfoType::Vector VectorType;

		public:
			QueueActivityPredicate(VectorType* out) :
				pList(out)
			{
				pList->clear();
			}

			template<class ThreadPtrT>
			bool operator () (const ThreadPtrT& thread) const
			{
				ThreadInfoType* info = new ThreadInfoType();
				info->thread = thread;
				if (!(!(info->thread)))
				{
					info->job = thread->currentJob();
					if (!(!(info->job)))
					{
						// We have a job which is currently working !
						info->hasJob = true;
						info->job->fillInformation(*info);
						pList->push_back(info);
						return true;
					}
				}
				info->hasJob = false;
				info->state = Yuni::Job::stateIdle;
				info->canceling = false;
				info->progression = 0;
				pList->push_back(info);
				return true;
			}

		private:
			mutable VectorType* pList;
		};

	} // anonymous namespace


	void QueueService::activitySnapshot(QueueService::ThreadInfo::Vector& out)
	{
		QueueActivityPredicate predicate(&out);
		((ThreadArray*) pThreads)->foreachThread(predicate);
	}







} // namespace Job
} // namespace Yuni

