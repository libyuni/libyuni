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
#include "mutex.h"
#include "signal.h"
#include "../core/string.h"
#include <vector>
#include "fwd.h"
#include "../core/noncopyable.h"
#include "../core/smartptr/intrusive.h"



namespace Yuni
{
namespace Thread
{

	/*!
	** \brief Base class interface for Threads (abstract)
	**
	** Example for implementing your own thread :
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/thread/thread.h>
	**
	**
	** class MyThread : public Yuni::Thread::IThread
	** {
	** public:
	**	MyThread() {}
	**	virtual ~MyThread() {}
	**
	** protected:
	**	virtual bool onExecute()
	**	{
	**		// do some time-consumming work here
	**		// ...
	**		// from time to time, you should check if the work should not be stopped
	**		if (shouldAbort())
	**			return false;
	**
	**		// continuing our long task
	**		// ...
	**
	**		// the work is done
	**		return true;
	**	}
	** };
	** \endcode
	**
	**
	** \internal The thread is really created when started (with the method start()), and
	**   destroyed when stopped by the method stop() (or when the object is
	**   destroyed too).
	**
	** \warning : Windows Server 2003 and Windows XP:  The target thread's initial
	**   stack is not freed when stopping the native thread, causing a resource leak
	*/
	class YUNI_DECL IThread : public IIntrusiveSmartPtr<IThread, false>, private NonCopyable<IThread>
	{
	public:
		//! Ancestor
		typedef IIntrusiveSmartPtr<IThread, false>  Ancestor;
		//! The most suitable smart pointer for the class
		typedef Ancestor::SmartPtrType<IThread>::PtrThreadSafe Ptr;
		//! The threading policy
		typedef Ancestor::ThreadingPolicy ThreadingPolicy;


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param stacksize The stack size for the new thread (in KByte)
		*/
		IThread(uint stacksize = 512);
		//! Destructor
		virtual ~IThread();
		//@}


		//! \name Execution flow
		//@{
		/*!
		** \brief Start the execution of the thread, if not already started
		**
		** If already started, the thread will be woke up
		** \return True if the thread has been started
		*/
		Error start();

		/*!
		** \brief Stop the execution of the thread and wait for it, if not already stopped
		**
		** \param timeout The timeout in milliseconds before killing the thread (default: 5000ms)
		** \return An error status (`errNone` if succeeded)
		*/
		Error stop(uint timeout = defaultTimeout);

		/*!
		** \brief Wait for an infinite amount of time for the end of the thread
		**
		** The thread is not stopped during the process.
		** \return An error status (`errNone` if succeeded)
		*/
		Error wait();

		/*!
		** \brief Wait for the end of the thread
		**
		** The thread is not stopped during the process.
		** \param milliseconds A timeout in milliseconds
		** \return An error status (`errNone` if succeeded)
		*/
		Error wait(uint milliseconds);

		/*!
		** \brief Restart the thread
		**
		** \param timeout The timeout in milliseconds before killing the thread (default: 5000ms)
		** \return True if the thread has been stopped then started
		**
		** \see stop()
		** \see start()
		*/
		Error restart(uint timeout = defaultTimeout);

		/*!
		** \brief Ask to Stop the execution of the thread as soon as possible
		**
		** After a call to this method, the method suspend() will return true,
		** which indicates that the thread should stop.
		** \see suspend()
		*/
		void gracefulStop();

		/*!
		** \brief Get if the thread is currently running
		** \return True if the thread is running
		*/
		bool started() const;

		/*!
		** \brief Interrupt the thread if suspended
		**
		** A call to this methid will interrupt a suspended state.
		** This method has no effect if the thread is not started.
		*/
		void wakeUp();
		//@}


		//! \name Thread cancellation
		//@{
		/*!
		** \brief Suspend the execution of the thread of X miliseconds
		**
		** This is a convenient method to know if the thread should stop as soon as possible.
		** If you do not want to suspend the execution of thread, you should use the
		** method `shouldAbort()` instead.
		**
		** \attention This method must only be called inside the execution of the thread
		**
		** \param timeout The delay in milliseconds to wait. O will only return if the thread should exit
		** \return True indicates that the thread should stop immediately
		*/
		bool suspend(uint timeout = 0);

		/*!
		** \brief Get if the thread should abort as soon as possible
		**
		** This is a convenient routine instead of `suspend(0)`, but a bit faster.
		** \attention This method must only be called inside the execution of the thread
		** \return True indicates that the thread should stop immediately
		*/
		bool shouldAbort();
		//@}


		//! \name Operators
		//@{
		//! Get if the thread is currently stopped
		bool operator ! () const;
		//@}


	protected:
		/*!
		** \brief Event: The thread has just been started
		**
		** This event is executed in the thread which has just been created.
		**
		** It can be directly stopped if returning false. However the `onStopped` event
		** will not be called.
		**
		** \return True to continue the execution of the thread, false to abort the
		** execution right now
		*/
		virtual bool onStarting() {return true;}

		/*!
		** \brief Event: The thread is running
		**
		** The thread has been successfully started (and `onStarting()` returned true).
		**
		** If this method returns false, the thread will be stopped (and the thread
		** context will be destroyed). The method `onStopped` will be called.
		** Otherwise, the thread will be paused for an infinite amount of time and the
		** method onPause() will be called. In this case it will be possible to re-run it
		** in calling wakeUp() or start().
		**
		** \attention This method should check from time to time if the thread has to stop. For that,
		** a call to `suspend(0)` is recommended.
		** \code
		**    if (suspend())
		**       return false;
		** \endcode
		**
		** \return True to pause the thread and wait a `wake up` signal, False to effectively stop it.
		** \see suspend()
		** \see onStarting()
		** \see onStopped()
		** \see wakeUp()
		*/
		virtual bool onExecute() = 0;

		/*!
		** \brief Event: The thread has finished its job and is waiting for being re-executed
		**
		** This event will be fired if the method onExecute returns false.
		*/
		virtual void onPause() {}

		/*!
		** \brief Event: The thread has been gracefully stopped
		**
		** This event is executed in the thread.
		**
		** \attention You should not rely on this event to release your resources. There is no guaranty
		** that this method will be called, especially if the thread has been killed because
		** it did not stop before the timeout was reached.
		*/
		virtual void onStop() {}

		/*!
		** \brief Event: The thread has been killed
		**
		** This method might be called from any thread
		*/
		virtual void onKill() {}


	private:
		# ifndef YUNI_NO_THREAD_SAFE
		//! Stop the native thread
		Error stopWL(uint timeout);
		# endif

	private:
		# ifndef YUNI_NO_THREAD_SAFE
		//! Mutex for starting up, see start()
		Signal pSignalStartup;
		//! Signal for stopping
		Signal pSignalHaveStopped;
		//! The thread must stop as soon as possible
		Signal pSignalMustStop;
		//! The thread can wake up
		Signal pSignalWakeUp;
		//! Mutex for protecting pShouldStop and pStarted
		Mutex pInnerFlagMutex;

		# ifdef YUNI_OS_WINDOWS
		void* pThreadHandle;
		# else
		//! ID of the thread, for pthread
		pthread_t pThreadID;
		//! Flag to determine whether pThreadID is valid or not
		// There is no portable value to determine if pThreadID is valid or not. We have to use a separate flag
		bool pThreadIDValid;
		# endif // YUNI_OS_WINDOWS
		# endif // YUNI_NO_THREAD_SAFE

		//! Get if the thread is running (must be protected by pInnerFlagMutex)
		volatile bool pStarted;
		# ifndef YUNI_NO_THREAD_SAFE
		//! Should stop the thread ? (must be protected by pInnerFlagMutex)
		volatile bool pShouldStop;
		# endif

		# ifndef YUNI_NO_THREAD_SAFE
		//! Thread stack size
		const uint pStackSize;
		# endif

		# ifndef YUNI_NO_THREAD_SAFE
		// our friend
		friend class Yuni::Job::IJob;
		friend YUNI_THREAD_FNC_RETURN Yuni::Private::Thread::threadCallbackExecute(void* arg);
		# endif

	}; // class IThread






} // namespace Thread
} // namespace Yuni

# include "thread.hxx"

