#ifndef __YUNI_JOB_QUEUE_QUEUE_HXX__
# define __YUNI_JOB_QUEUE_QUEUE_HXX__

# include "../../thread/timer.h"
# include <iostream>


namespace Yuni
{
namespace Job
{

	template<class SchedulerT>
	inline QueueService<SchedulerT>::QueueService() :
		SchedulerPolicy(pWaitingRoom),
		pWaitingRoom()
	{}


	template<class SchedulerT>
	inline QueueService<SchedulerT>::QueueService(bool autostart) :
		SchedulerPolicy(pWaitingRoom),
		pWaitingRoom()
	{
		if (autostart)
			start();
	}


	template<class SchedulerT>
	QueueService<SchedulerT>::~QueueService()
	{
		if (pStarted)
		{
			// For the execution of all jobs
			wait();
			// Stop the service wahtever the result of the previous method
			stop();
		}
	}


	template<class SchedulerT>
	bool QueueService<SchedulerT>::start()
	{
		if (!pStarted)
		{
			pStarted = 1;
			SchedulerPolicy::schedulerStart();
		}
		return true;
	}


	template<class SchedulerT>
	bool QueueService<SchedulerT>::stop(uint timeout)
	{
		if (pStarted)
		{
			pStarted = 0;
			SchedulerPolicy::schedulerStop(timeout);
		}
		return true;
	}


	template<class SchedulerT>
	inline bool QueueService<SchedulerT>::restart(uint timeout)
	{
		return (SchedulerPolicy::schedulerStop(timeout) and SchedulerPolicy::schedulerStart());
	}



	namespace // anonymous
	{

		template<class SchedulerT>
		class QueueServiceWaitHelper final : public Thread::Timer
		{
		public:
			QueueServiceWaitHelper(SchedulerT& scheduler, Yuni::Private::QueueService::WaitingRoom& room,
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
			SchedulerT& pScheduler;
			//! The returned status
			bool pStatus;

		}; // class QueueServiceWaitHelper

	} // anonymous namespace



	template<class SchedulerT>
	void QueueService<SchedulerT>::wait()
	{
		// TODO QueueService::wait: Find a more efficient way for doing this
		QueueServiceWaitHelper<SchedulerT> helper(static_cast<SchedulerT&>(*this), pWaitingRoom, 150);
		helper.start();
		helper.wait();
	}


	template<class SchedulerT>
	bool QueueService<SchedulerT>::wait(uint timeout, uint pollInterval)
	{
		// TODO QueueService::wait: Find a more efficient way for doing this
		QueueServiceWaitHelper<SchedulerT> helper(static_cast<SchedulerT&>(*this), pWaitingRoom, pollInterval);
		helper.start();
		helper.wait(timeout);
		return helper.status();
	}




	template<class SchedulerT>
	void QueueService<SchedulerT>::add(IJob* job)
	{
		if (job)
		{
			pWaitingRoom.add(job);
			SchedulerPolicy::schedulerNotifyNewJobInWaitingRoom(priorityDefault);
		}
	}


	template<class SchedulerT>
	void QueueService<SchedulerT>::add(const IJob::Ptr& job)
	{
		if (!(!job))
		{
			pWaitingRoom.add(job);
			SchedulerPolicy::schedulerNotifyNewJobInWaitingRoom(priorityDefault);
		}
	}


	template<class SchedulerT>
	void QueueService<SchedulerT>::add(const IJob::Ptr& job, Priority priority)
	{
		if (!(!job))
		{
			pWaitingRoom.add(job, priority);
			SchedulerPolicy::schedulerNotifyNewJobInWaitingRoom(priority);
		}
	}


	template<class SchedulerT>
	void QueueService<SchedulerT>::add(IJob* job, Priority priority)
	{
		if (!(!job))
		{
			pWaitingRoom.add(job, priority);
			SchedulerPolicy::schedulerNotifyNewJobInWaitingRoom(priority);
		}
	}


	template<class SchedulerT>
	inline uint QueueService<SchedulerT>::size() const
	{
		return pWaitingRoom.size();
	}


	template<class SchedulerT>
	inline uint QueueService<SchedulerT>::count() const
	{
		return pWaitingRoom.size();
	}


	template<class SchedulerT>
	inline QueueService<SchedulerT>& QueueService<SchedulerT>::operator += (IJob* job)
	{
		add(job);
		return *this;
	}


	template<class SchedulerT>
	inline QueueService<SchedulerT>& QueueService<SchedulerT>::operator << (IJob* job)
	{
		add(job);
		return *this;
	}


	template<class SchedulerT>
	inline QueueService<SchedulerT>& QueueService<SchedulerT>::operator += (const IJob::Ptr& job)
	{
		add(job);
		return *this;
	}


	template<class SchedulerT>
	inline QueueService<SchedulerT>& QueueService<SchedulerT>::operator << (const IJob::Ptr& job)
	{
		add(job);
		return *this;
	}


	template<class SchedulerT>
	inline uint QueueService<SchedulerT>::threadCount() const
	{
		return SchedulerPolicy::schedulerThreadCount();
	}


	namespace // anonymous
	{

		template<class SchedulerT>
		class QueueActivityPredicate
		{
		public:
			//!
			typedef typename QueueService<SchedulerT>::ThreadInfo ThreadInfoType;
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


	template<class SchedulerT>
	void QueueService<SchedulerT>::activitySnapshot(
		typename QueueService<SchedulerT>::ThreadInfo::Vector& out)
	{
		QueueActivityPredicate<SchedulerT> predicate(&out);
		SchedulerPolicy::schedulerForeachThread(predicate);
	}





} // namespace Job
} // namespace Yuni

#endif // __YUNI_JOB_QUEUE_QUEUE_H__
