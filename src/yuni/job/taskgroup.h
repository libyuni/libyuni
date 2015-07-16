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
#include "../yuni.h"
#include "../core/string.h"
#include "../core/event/event.h"
#include "../core/noncopyable.h"
#include "../core/smartptr/intrusive.h"
#include "../thread/signal.h"




namespace Yuni
{
namespace Job
{

	//! Forward declaration
	class IJob;
	class QueueService;



	/*!
	** \brief Group of jobs
	**
	** \code
	** Job::QueueService queueservice;
	** Job::Taskgroup taskgroup(queueservice);
	**
	** taskgroup += [&](IJob&) -> bool {
	**		std::cout << "job 1 complete !" << std::endl;
	**		return true;
	** };
	** taskgroup += [&](IJob&) -> bool {
	**		std::cout << "job 2 complete !" << std::endl;
	**		return true;
	** };
	** taskgroup += [&](IJob&) -> bool {
	**		std::cout << "job 3 complete !" << std::endl;
	**		return true;
	** };
	**
	** taskgroup.start();
	** taskgroup.wait();
	** \endcode
	** \internal This class must keep the fewer dependencies as possible
	*/
	class YUNI_DECL Taskgroup final
		: public IIntrusiveSmartPtr<Taskgroup, false>
		, public NonCopyable<Taskgroup>
	{
	public:
		//! Ancestor
		typedef IIntrusiveSmartPtr<Taskgroup, false>  Ancestor;
		//! The most suitable smart pointer for the class
		typedef Ancestor::SmartPtrType<Taskgroup>::PtrThreadSafe Ptr;
		//! The threading policy
		typedef Ancestor::ThreadingPolicy ThreadingPolicy;
		//! Job class for the task
		class YUNI_DECL ITaskgroupJob;

		enum Status
		{
			//! The task is currently running
			stRunning,
			//! The task has succeeded
			stSucceeded,
			//! The task has failed
			stFailed,
			//! The task has been canceled
			stCanceled,
		};


	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Default constructor
		** \param queueservice The default queueservice to use when not specified
		*/
		explicit Taskgroup(QueueService& queueservice, bool cancelOnError = true, bool autostart = false);

		# ifdef YUNI_HAS_CPP_MOVE
		//! Move constructor
		Taskgroup(Taskgroup&&) = delete;
		# endif

		/*!
		** \brief Destructor (will stop all underlying jobs)
		**
		** The task will be canceled if running. Use `wait()` to avoid this behavior
		*/
		~Taskgroup();
		//@}


		//! \name Task
		//@{
		/*!
		** \brief Run the task
		**
		** This method has no effect if the taskgroup was already running and will
		** stop immediatly if no job/callback has been added first.
		**
		** \param cancelOnError True to automatically cancel the task when the first error is encountered. False to continue anyway
		*/
		void start();

		/*!
		** \brief Cancel the task
		**
		** All underlying jobs will be stopped as soon as possible
		*/
		void cancel();

		/*!
		** \brief Wait for the task being complete
		** \return The status of the task
		*/
		Status wait();

		/*!
		** \brief Wait for the task being complete (with timeout)
		**
		** \param timeout A timeout, in milliseconds
		** \return The status of the task. stRunning if the timeout has been reached
		*/
		Status wait(uint timeout);

		/*!
		** \brief Get the current status of the task and Fetch various information in the same time
		**
		** \param[out] status The current status of the task
		** \param jobCount The total number of jobs [optional]
		** \param doneCount The total number of jobs which have terminated [optional]
		*/
		Status status(uint* jobCount = nullptr, uint* doneCount = nullptr) const;


		/*!
		** \brief Make the task succceed
		**
		** All underlying jobs will be stopped as soon as possible
		*/
		void markAsSucceeded();

		/*!
		** \brief Make the task fail
		**
		** All underlying jobs will be stopped as soon as possible
		*/
		void markAsFailed();
		//@}


		//! \name Jobs
		//@{
		/*!
		** \brief Add a new job in the task
		**
		** The job will be added to the default queueservice immediatly if the taskgroup is running
		** \param queueservice Any queueservice can be used.
		** \param callback A functor / lambda
		*/
		void add(const Bind<bool (IJob&)>& callback);

		/*!
		** \brief Add a new job in the task
		**
		** The job will be added to the queueservice immediatly if the taskgroup is running
		** \param queueservice Any queueservice can be used.
		** \param callback A functor / lambda
		** \param autostart True to automatically run the task if not already running
		*/
		void add(QueueService& queueservice, const Bind<bool (IJob&)>& callback);
		//@}


		//! \name Operators
		//@{
		//! Add a new job
		Taskgroup& operator += (const Bind<bool (IJob&)>& callback);

		# ifdef YUNI_HAS_CPP_MOVE
		//! Move constructor
		Taskgroup& operator = (Taskgroup&&) = delete;
		# endif
		//@}



	public:
		//! All events
		struct
		{
			//! Event: the task has started (can be called from any thread)
			Event<void ()> started;
			//! Event: the task has terminated its work (can be called from any thread)
			Event<void (Status success)> terminated;
		}
		on;



	private:
		//! Array of sub-jobs
		typedef std::vector<ITaskgroupJob*> JobList;

	private:
		inline void startWL();
		inline void stopAllJobsWL();
		inline void deleteAllJobsWL();
		inline void onJobTerminated(ITaskgroupJob&, bool success);
		inline void onTaskStoppedWL();

	private:
		//! The task has been told to start (thus 'is running' and jobs are waiting to run)
		bool pTaskHasStarted;
		//! Current state of the task
		// (should not be directly used if the task is still running)
		Status pTaskStatus;
		//! Signal for notifyng that the task has finished (canceled or all jobs are terminated)
		Thread::Signal pSignalTaskStopped;

		//! All jobs registered for the task (can be added while running)
		JobList pJobs;
		//! The total number of jobs which have finish their work
		uint pJobsDoneCount;
		//! True to automatically cancel when the first error is encountered
		const bool pCancelOnError;
		//! True to automatically run the task (if not already running) when adding a job
		const bool pAutostart;

		//! The default queueservice
		QueueService& pDefaultQueueservice;

	}; // class Taskgroup







} // namespace Job
} // namespace Yuni

#include "taskgroup.hxx"
