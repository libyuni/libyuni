/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#pragma once
#include "../../yuni.h"
#include "../job.h"
#include "waitingroom.h"
#include "../../core/atomic/bool.h"
#include "../../thread/signal.h"
#include "q-event.h"
#include "../../core/dictionary.h"
#include "../../core/smartptr/intrusive.h"



namespace Yuni
{
namespace Job
{

	/*!
	** \brief Multithreaded Job QueueService
	*/
	class YUNI_DECL QueueService final
		: public IIntrusiveSmartPtr<QueueService, false>
		, public NonCopyable<QueueService>
	{
	public:
		//! Ancestor
		typedef IIntrusiveSmartPtr<QueueService, false>  Ancestor;
		//! The most suitable smart pointer for the class
		typedef Ancestor::SmartPtrType<QueueService>::PtrThreadSafe Ptr;
		//! The threading policy
		typedef Ancestor::ThreadingPolicy ThreadingPolicy;

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

		//! Destructor
		~QueueService();
		//@}


		//! \name Service
		//@{
		/*!
		** \brief Start the service and execute the jobs
		*/
		bool start();

		/*!
		** \brief Wait for an event
		**
		** Example: wait for all jobs to be finished
		** \code
		** queueservice.wait(qsIdle);
		** \endcode
		**
		** Example: wait for queue service being stopped
		** \code
		** queueservice.wait(qsStopped);
		** \endcode
		**
		** \param event A queue service event
		*/
		void wait(QServiceEvent event);

		/*!
		** \brief Wait for all jobs are finished
		**
		** \param event A queue service event
		** \param timeout Timeout (in milliseconds)
		** \return True if all jobs are finished, false if the timeout has been reached
		*/
		bool wait(QServiceEvent event, uint timeout);


		/*!
		** \brief Stop the service
		**
		** All unfinished jobs will be kept and re-executed at the next start.
		** It is of their responsibility to properly resume if they have to.
		** All working threads will be destroyed at the very end of this method.
		**
		** \important Since this method waits for the end of all running jobs, it
		**   must not be called from a job or it will result in deadlock
		**   (use 'gracefulStop()' instead)
		**
		** \param timeout Timeout (in milliseconds)
		*/
		void stop(uint timeout = defaultTimeout);


		/*!
		** \brief Ask to Stop the execution of the thread as soon as possible
		**
		** After a call to this method, the method suspend() will return true,
		** which indicates that the job should stop as soon as possible.
		**
		** \note This method can be safely called from a job
		*/
		void gracefulStop();

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
		** \warning The job may already be into the queue (thus the same job might be
		** executed several times at once). However it must obviously ensure its own
		** thread-safety in this case.
		**
		** \param job The job to add
		*/
		void add(const IJob::Ptr& job);

		/*!
		** \brief Add a job into the queue
		**
		** \warning The job may already be into the queue (thus the same job might be
		** executed several times at once). However it must obviously ensure its own
		** thread-safety in this case.
		**
		** \param job The job to add
		** \param priority Its priority execution
		*/
		void add(const IJob::Ptr& job, Priority priority);

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

		/*!
		** \brief Remove all jobs waiting for being executed
		**
		** All jobs currently running won't be stopped
		*/
		void clear();
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
		//! Register a new thread in active duty
		void registerWorker(void* threadself);
		//! Unregister a thread no longer in active duty
		void unregisterWorker(void* threadself);
		//! Wait for all threads to finish
		bool waitForAllThreads(uint timeout);
		//! Wake up thread - some work here !
		void wakeupWorkers();

	private:
		//! Flag to know if the service is started [must be protected by the internal mutex]
		enum State
		{
			sStopped,
			sRunning,
			sStopping,
		}
		pStatus;

		//! The list of all remaining jobs
		Yuni::Private::QueueService::WaitingRoom pWaitingRoom;

		// Scheduler

		//! The minimum number of threads [must be protected by the internal mutex]
		volatile uint pMinimumThreadCount;
		//! The maximum number of threads [must be protected by the internal mutex]
		volatile uint pMaximumThreadCount;
		//! Array of threads
		volatile void* pThreads;

		//! Signal, for being notified when all threads have stopped to work
		Yuni::Thread::Signal pSignalAllThreadHaveStopped;
		//!
		Yuni::Thread::Signal pSignalShouldStop;

		//! Set of workers in active duty
		// The workers may need to unregister several times and it is not safe
		// to let the worker handle the state (it might be killed or something nasty
		// may happen - race conditions)
		// This set is useless (and should not be used in this current form)
		Yuni::Set<void*>::Unordered  pWorkerSet;

		// Nakama !
		friend class Yuni::Private::QueueService::QueueThread;

	}; // class QueueService






} // namespace Job
} // namespace Yuni

#include "service.hxx"
