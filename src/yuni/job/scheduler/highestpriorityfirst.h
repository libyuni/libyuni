#ifndef __YUNI_JOB_SCHEDULER_HIGHEST_PRIORITY_FIRST_H__
# define  __YUNI_JOB_SCHEDULER_HIGHEST_PRIORITY_FIRST_H__

# include "../../yuni.h"
# include "../queue/waitingroom.h"
# include "../job.h"
# include "../queue/thread.h"
# include "../../thread/array.h"



namespace Yuni
{
namespace Job
{
namespace Scheduler
{


	/*!
	** \brief Basic Scheduler, where the job with the highest priority is executed in first
	*/
	class YUNI_DECL HighestPriorityFirst
	{
	public:
		//! The scheduler itself
		typedef HighestPriorityFirst SchedulerType;

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param room Reference to the Waiting room
		*/
		explicit HighestPriorityFirst(Private::QueueService::WaitingRoom& room);

		//! Destructor
		~HighestPriorityFirst();
		//@}

		uint minimumThreadCount() const;

		bool minimumThreadCount(uint);

		uint maximumThreadCount() const;

		bool maximumThreadCount(uint n);

		/*!
		** \brief Get if the scheduler is idle
		*/
		bool idle() const;


	protected:
		/*!
		** \brief Start all threads to execute the jobs
		*/
		bool schedulerStart();

		/*!
		** \brief Stop all working threads
		*/
		bool schedulerStop(uint timeout);

		/*!
		** \brief Event: A job has just been added into the waiting room
		**
		** \param priority The priority of this job
		*/
		void schedulerNotifyNewJobInWaitingRoom(Yuni::Job::Priority);

		/*!
		** \brief Get the next job to execute
		*/
		bool nextJob(IJob::Ptr& out);

		/*!
		** \brief Get the number of threads currently in use
		*/
		uint schedulerThreadCount() const;

		template<class PredicateT>
		void schedulerForeachThread(PredicateT& predicate)
		{
			pThreads.foreachThread(predicate);
		}

		void schedulerIncrementWorkerCount();

		void schedulerDecrementWorkerCount();


	private:
		//! Index of the next thread to wake up
		Atomic::Int<32> pRunningThreads;

		//! Reference to the array of threads
		Yuni::Thread::Array<Yuni::Private::Jobs::QueueThread<SchedulerType> >  pThreads;
		//! Reference to the waiting room
		Private::QueueService::WaitingRoom& pWaitingRoom;
		//! Mutex, used for start/stop methods
		Mutex pServiceMutex;
		//! Flag to know if the scheduler is started
		Atomic::Int<32> pStarted;
		//! Total number of workers
		Atomic::Int<32> pWorkerCount;

		//! The maximum number of thread
		uint pMaximumThreadCount;

		// friend
		template<class T> friend class Yuni::Private::Jobs::QueueThread;

	}; // class HighestPriorityFirst





} // namespace Scheduler
} // namespace Job
} // namespace Yuni

# include "highestpriorityfirst.hxx"

#endif // __YUNI_JOB_SCHEDULER_HIGHEST_PRIORITY_FIRST_H__
