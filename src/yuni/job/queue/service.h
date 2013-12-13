#ifndef __YUNI_JOB_QUEUE_SERVICE_H__
# define __YUNI_JOB_QUEUE_SERVICE_H__

# include "../../yuni.h"
# include "../job.h"
# include "waitingroom.h"
# include "../../core/atomic/bool.h"



namespace Yuni
{
namespace Job
{

	/*!
	** \brief Multithreaded Job QueueService
	*/
	class YUNI_DECL QueueService final
		: public Policy::ObjectLevelLockableNotRecursive<QueueService>
	{
	public:
		//! The threading policy
		typedef Policy::ObjectLevelLockableNotRecursive<QueueService> ThreadingPolicy;
		//! The most suitable smart pointer for the class
		typedef SmartPtr<QueueService> Ptr;
		enum
		{
			//! A default timeout
			defaultTimeout = Yuni::Thread::defaultTimeout,
		};

		//! Information about a single thread
		class ThreadInfo final
		{
		public:
			//! The most suitable smart pointer for the class
			typedef SmartPtr<ThreadInfo> Ptr;
			//! Vector of ThreadInfo
			typedef std::vector<typename ThreadInfo::Ptr>  Vector;

		public:
			//! Reference to the working thread
			Thread::IThread::Ptr thread;
			//! Reference to the job currently in execution
			Job::IJob::Ptr job;

			//! Flag to know if the thread has a job currently in execution
			bool hasJob;

			//! State of the job (if any)
			Job::State state;
			//! Flag to know if the job is canceling its work
			bool canceling;
			//! Progression (in percent) of the job (if any, between 0 and 100)
			int progression;
			//! Name of the job
			String name;

		}; // class ThreadInfo


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		*/
		QueueService();
		/*!
		** \brief Constructor, with an autostart mode
		**
		** \param autostart True to automatically start the service
		*/
		explicit QueueService(bool autostart);
		/*!
		** \brief Destructor
		*/
		~QueueService();
		//@}


		//! \name Service
		//@{
		/*!
		** \brief Start the service and execute the jobs
		*/
		bool start();

		/*!
		** \brief Wait until all jobs are finished
		*/
		void wait();

		/*!
		** \brief Wait until all jobs are finished
		**
		** \param timeout Timeout
		** \param pollInterval Interval in milliseconds between each poll when waiting
		** \return True if all jobs are finished, false if the timeout has been reached
		*/
		bool wait(uint timeout, uint pollInterval = 150);

		/*!
		** \brief Stop the service
		**
		** All unfinished jobs will be kept and re-executed at the next start.
		** It is of their responsibility to properly resume if they have to.
		** All working threads will be destroyed at the very end of this method.
		*/
		bool stop(uint timeout = defaultTimeout);

		/*!
		** \brief Stop then start the service
		**
		** All unfinished jobs will be kept and re-executed at the next start.
		** It is of their responsibility to properly resume if they have to.
		*/
		bool restart(uint timeout = defaultTimeout);

		//! Get if the service is started
		bool started() const;

		//! Get if the scheduler is idle (no worker is in active duty)
		bool idle() const;
		//@}


		//! \name Jobs handling
		//@{
		/*!
		** \brief Add a job into the queue
		**
		** \warning The job may already be into the queue. However it must ensure
		** its thread-safety in this case.
		**
		** \param job The job to add
		*/
		void add(IJob* job);

		/*!
		** \brief Add a job into the queue
		**
		** \warning The job may already be into the queue. However it must ensure
		** its thread-safety in this case.
		**
		** \param job The job to add
		*/
		void add(const IJob::Ptr& job);

		/*!
		** \brief Add a job into the queue
		**
		** \warning The job may already be into the queue. However it must ensure
		** its thread-safety in this case.
		**
		** \param job The job to add
		** \param priority Its priority execution
		*/
		void add(const IJob::Ptr& job, Priority priority);

		/*!
		** \brief Add a job into the queue
		**
		** \warning The job may already be into the queue. However it must ensure
		** its thread-safety in this case.
		**
		** \param job The job to add
		** \param priority Its priority execution
		*/
		void add(IJob* job, Priority priority);

		/*!
		** \brief Retrieve information about the activity of the queue manager
		**
		** \note Event if in the list, a job may already have finished
		**   its work at the end of this method.
		*/
		void activitySnapshot(ThreadInfo::Vector& out);

		/*!
		** \brief Get the number of jobs waiting to be executed
		**
		** This value does not take into account the number of jobs
		** currently running.
		*/
		uint waitingJobsCount() const;
		//@}


		//! \name Threads
		//@{
		//! Get the minimum number of threads
		uint minimumThreadCount() const;
		//! Set the minimum number of threads (must be less than maximumNu)
		bool minimumThreadCount(uint count);

		//! Get the maximum number of threads
		uint maximumThreadCount() const;
		//! Set the maximum number of threads (must be >= 1)
		bool maximumThreadCount(uint count);

		//! Get the minimum and the maximum number of threads
		std::pair<uint,uint> minmaxThreadCount() const;
		//! Set the minimum and the maximum number of threads
		bool minmaxThreadCount(const std::pair<uint, uint>& minmaxcount);

		//! Get the current number of working threads
		uint threadCount() const;
		//@}


		//! \name Operators
		//@{
		//! The operator << (add a job)
		QueueService& operator += (IJob* job);
		//! The operator << (add a job)
		QueueService& operator << (IJob* job);
		//! The operator << (add a job)
		QueueService& operator += (const IJob::Ptr& job);
		//! The operator << (add a job)
		QueueService& operator << (const IJob::Ptr& job);
		//@}


	private:
		//! Flag to know if the service is started
		Atomic::Bool pStarted;
		//! The list of all remaining jobs
		Yuni::Private::QueueService::WaitingRoom pWaitingRoom;

		// Scheduler

		//! The minimum number of threads (must be protected by the internal mutex)
		volatile uint pMinimumThreadCount;
		//! The maximum number of threads (must be protected by the internal mutex)
		volatile uint pMaximumThreadCount;
		//! Array of threads
		volatile void* pThreads;
		//! Number of workers in active duty
		Atomic::Int<32> pWorkerCountInActiveDuty;

		// Nakama !
		friend class Yuni::Private::QueueService::QueueThread;

	}; // class QueueService






} // namespace Job
} // namespace Yuni

# include "service.hxx"

#endif // __YUNI_JOB_QUEUE_SERVICE_H__
