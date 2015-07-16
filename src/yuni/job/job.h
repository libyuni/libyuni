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
#include "fwd.h"
#include "../thread/thread.h"
#include "../core/string.h"
#include "../core/atomic/bool.h"
#include "../core/smartptr/intrusive.h"




namespace Yuni
{

/*!
** \brief Jobs (Multithreading programming)
** \ingroup Jobs
*/
namespace Job
{

	/*!
	** \brief Job Interface (time consuming operations)
	**
	** This class is designed to implement a single task executed by
	** Job::QueueService.
	**
	** This clas can be used for wrapping a downloader, an image
	** processing algorithm, a movie encoder, etc.
	**
	** Indeed, doing time-consuming operations like downloads and database
	** transactions in the main thread can cause your user interface to seem as
	** though it has stopped responding while they are running. To avoid this
	** problem, you want to execute your long-running task in an asynchonous
	** manner (a background task).
	**
	** See async() for a more convenient way for simple tasks
	**
	** \code
	**
	** class MyJob final : public Job::IJob
	** {
	** public:
	**   virtual ~MyJob() { }
	** private:
	**   virtual void onExecute() override
	**   {
	**		[... time consuming job ...]
	**   }
	** };
	**
	** [... add a MyJob instance to a Job::QueueService ...]
	**
	** \endcode
	**
	** \see QueueService
	** \ingroup Jobs
	*/
	class YUNI_DECL IJob : public IIntrusiveSmartPtr<IJob, false>
	{
	public:
		//! Ancestor
		typedef IIntrusiveSmartPtr<IJob, false>  Ancestor;
		//! The most suitable smart pointer for the class
		typedef Ancestor::SmartPtrType<IJob>::PtrThreadSafe Ptr;
		//! The threading policy
		typedef Ancestor::ThreadingPolicy ThreadingPolicy;
		//! List of jobs
		typedef std::list<Ptr> List;

	public:
		//! \name Contructor & Destructor
		//@{
		//! Default constructor
		IJob();
		//! Destructor
		virtual ~IJob();
		//@}


		//! \name Informations about the job itself
		//@{
		//! Get the progression in percent (value between 0 and 100)
		int progression() const;

		/*!
		** \brief Get if the job is finished
		**
		** This is a convenient (and faster) replacement for the following code :
		** \code
		** (state() == stateIdle and progression() == 100)
		** \endcode
		*/
		bool finished() const;

		//! Get the caption of the job
		virtual String caption() const;
		//@}


		//! \name States
		//@{
		//! Get the current state of the job
		enum Job::State state() const;

		//! Get if the job is idle
		bool idle() const;
		//! Get if the job is waiting for being executed
		bool waiting() const;
		//! Get if the job is running
		bool running() const;
		//! Get if the job is canceling its work (and it is currently running)
		bool canceling() const;
		//@}


		//! \name Execution flow
		//@{
		/*!
		** \brief Ask to the job to cancel its work as soon as possible
		**
		** This method has no effect if the job is not currently running.
		*/
		void cancel();

		/*!
		** \brief Execute the job
		**
		** In standard uses, 't' must not be NULL.
		** \param t The thread which will execute this job (can be null)
		*/
		void execute(Thread::IThread* t);
		//@}


		template<class T> void fillInformation(T& info);

	protected:
		/*!
		** \brief Implement this method to define your own time-consuming task
		*/
		virtual void onExecute() = 0;

		/*!
		** \brief Set the progression in percent (0..100)
		*/
		void progression(const int p);

		/*!
		** \brief Suspend the execution of the job during X miliseconds
		**
		** This method is nearly identical to IThread::suspend() in its behavior.
		** But this method should not have to be called in most of the jobs,
		** except when polling events or equivalent.
		** If you only want to know if the job should abort, prefer shouldAbort()
		** instead.
		**
		** \attention This method must only be called inside the execution of the job
		**
		** \param delay The delay in miliseconds. 0 will only return if the thread should exit
		** \return True indicates that the job should stop immediately
		** \see IThread::suspend()
		*/
		bool suspend(uint delay = 0) const;

		/*!
		** \brief Get if the job should abort as soon as possible
		**
		** This is a convenient replacement of `suspend(0)`, and a bit faster.
		**
		** \attention This method must only be called inside the execution of the job
		** \return True indicates that the thread should stop immediately
		*/
		bool shouldAbort() const;


	private:
		//! State of the job
		Atomic::Int<32> pState;
		//! Progression
		Atomic::Int<32> pProgression;
		//! Flag to cancel the work
		Atomic::Bool pCanceling;
		//! The attached thread to this job, if any
		ThreadingPolicy::Volatile<Thread::IThread*>::Type pThread;

		// our friends !
		template<class JobT> friend class Yuni::Private::QueueService::JobAccessor;

	}; // class IJob






} // namespace Job
} // namespace Yuni

# include "job.hxx"

