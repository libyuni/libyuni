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
#include "loop.h"



namespace Yuni
{
namespace Private
{
namespace Core
{
namespace EventLoop
{

	template<class EventLoopT>
	class Thread final : public Yuni::Thread::IThread
	{
	public:
		//! The type of the calling event loop
		typedef EventLoopT EventLoopType;

	public:
		Thread(EventLoopType& loop) :
			pEventLoop(loop)
		{
		}

		virtual ~Thread()
		{
			// Needed for code robustness and to prevent against corruption
			// of the v-table
			stop();
		}

		void suspendTheThread(uint timeout)
		{
			(void) suspend(timeout);
		}

	protected:
		virtual bool onExecute()
		{
			// Starting an infinite loop
			// We don't have to check for the thread termination, since a request
			// will be dispatched especially for that. This request will return
			// `false`, which will abort the loop.
			pEventLoop.runInfiniteLoopWL();
			return false;
		}

		virtual void onStop()
		{
			// Notifying the event loop that our work is done
			typename EventLoopType::ThreadingPolicy::MutexLocker locker(pEventLoop);
			pEventLoop.pIsRunning = false;
		}

		virtual void onKill()
		{
			// Notifying the event loop that our work is done
			typename EventLoopType::ThreadingPolicy::MutexLocker locker(pEventLoop);
			pEventLoop.pIsRunning = false;
		}

	private:
		EventLoopType& pEventLoop;

	}; // class Thread<>



} // namespace EventLoop
} // namespace Core
} // namespace Private
} // namespace Yuni







namespace Yuni
{
namespace Core
{
namespace EventLoop
{


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	inline IEventLoop<ParentT,FlowT,StatsT,DetachedT>::IEventLoop() :
		pHasRequests(),
		pRequests(nullptr),
		pIsRunning(false),
		pThread(nullptr)
	{
		// Note: Visual Studio does not like `this` in the initialization section
		// Broadcast the pointer of the event loop to the policies
		FlowPolicy::onInitialize(this);

		// Initialize the thread if in detached mode
		if (detached)
			pThread = new ThreadType(*this);
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	IEventLoop<ParentT,FlowT,StatsT,DetachedT>::~IEventLoop()
	{
		// Stopping gracefully the loop if not already done
		stop();

		typename ThreadingPolicy::MutexLocker locker(*this);
		{
			// Destroying the thread
			if (detached)
				delete pThread;
			pThread = NULL; // for code safety
			// Destroying the request list
			delete pRequests;
			pRequests = NULL; // for code safety
		}
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	void IEventLoop<ParentT,FlowT,StatsT,DetachedT>::start()
	{
		// startup process
		{
			// Locking for inserting the new request
			typename ThreadingPolicy::MutexLocker locker(*this);
			// Aborting if the event loop is already running
			if (pIsRunning)
				return;
			// Flow
			if (FlowPolicy::onStart())
			{
				// The event loop is running
				pIsRunning = true;
			}

			// Initializing the request list
			if (NULL == pRequests)
				pRequests = new RequestListType();
		}

		if (detached)
		{
			// The loop is launched from another thread
			pThread->start();
		}
		else
		{
			// Launching the event loop from the calling thread
			this->runInfiniteLoopWL();

			// Resetting internal status
			typename ThreadingPolicy::MutexLocker locker(*this);
			pIsRunning = false;
		}
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	void IEventLoop<ParentT,FlowT,StatsT,DetachedT>::gracefulStop()
	{
		// Locking for checking internal status and inserting a 'stop' request
		typename ThreadingPolicy::MutexLocker locker(*this);
		// Aborting if the event loop is already stopped
		if (not pIsRunning or not FlowPolicy::onStop())
			return;

		// Posting a request that will fail (return false) in order to stop
		// the event loop.
		// The object is still locked and we directly inject the request into
		// the request list.
		if (NULL == pRequests)
			pRequests = new RequestListType();

		pRequests->push_back(RequestStop);

		// Informing the event loop that a new request is available
		pHasRequests = true;
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	void IEventLoop<ParentT,FlowT,StatsT,DetachedT>::stop(uint timeout)
	{
		// Locking for checking internal status and inserting a 'stop' request
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			// Aborting if the event loop is already stopped
			if (not pIsRunning or not FlowPolicy::onStop())
				return;

			// Posting a request that will fail (return false) in order to stop
			// the event loop.
			// The object is still locked and we directly inject the request into
			// the request list.
			if (NULL == pRequests)
				pRequests = new RequestListType();

			pRequests->push_back(RequestStop);

			// Informing the event loop that a new request is available
			pHasRequests = true;
		}


		if (detached)
		{
			// The event loop is running is detached mode
			// Waiting for the end of the execution of the external thread
			assert(NULL != pThread && "Event loop: Invalid thread pointer");
			pThread->stop(timeout);
		}
		else
		{
			// Trying to wait for the end of the event loop
			// Spinning lock, since we don't have better ways here.
			uint elapsed = 0;
			do
			{
				// Checking for the thread termination
				// This check is performed first. With luck, the loop may have already stopped.
				{
					typename ThreadingPolicy::MutexLocker locker(*this);
					if (not pIsRunning)
						break;
				}

				// Sleeping a bit...
				Yuni::SuspendMilliSeconds(50u);
				elapsed += 50u;
			}
			while (elapsed < timeout);
		}
	}



	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	bool IEventLoop<ParentT,FlowT,StatsT,DetachedT>::running() const
	{
		return pIsRunning;
	}



	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	void IEventLoop<ParentT,FlowT,StatsT,DetachedT>::dispatch(const typename IEventLoop<ParentT,FlowT,StatsT,DetachedT>::RequestType& request)
	{
		// Locking for inserting the new request
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			// Initializing pRequests allows for dispatching a request before calling start()
			if (not pRequests)
				pRequests = new RequestListType();
			// Flow
			if (not FlowPolicy::onRequestPosted(request))
				return;
			// Inserting the new request
			pRequests->push_back(request);
			// Statistics
			StatisticsPolicy::onRequestPosted(request);
		}

		// Informing the event loop that a new request is available
		pHasRequests = true;
	}



	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	void IEventLoop<ParentT,FlowT,StatsT,DetachedT>::runInfiniteLoopWL()
	{
		// Statistics
		StatisticsPolicy::onStart();

		while (true)
		{
			// Run the cycle
			if (not FlowPolicy::onNewCycle() or not runCycleWL())
				break;
		}

		// Statistics
		StatisticsPolicy::onStop();
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	bool IEventLoop<ParentT,FlowT,StatsT,DetachedT>::runCycleWL()
	{
		// Statistics
		StatisticsPolicy::onNewCycle();

		// Performing requests, if any
		if (pHasRequests)
		{
			if (not performAllRequestsWL())
			{
				// At least one request has failed. Aborting
				return false;
			}
		}
		// Execute the parent loop
		if (static_cast<ParentType*>(this)->onLoop())
		{
			// Statistics
			StatisticsPolicy::onEndOfCycle();
			return true;
		}
		return false;
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	bool IEventLoop<ParentT,FlowT,StatsT,DetachedT>::performAllRequestsWL()
	{
		// The request list. The pointer will be retrieved as soon as the
		// inner mutex is locked
		const RequestListType* requests;

		// Lock and retrieve the request list
		// We will keep a pointer to the request list, and will create a new one
		// to release the mutex as soon as possible, to allow other threads to
		// dispatch new requests while the event loop is working.
		{
			// Locking
			typename ThreadingPolicy::MutexLocker locker(*this);
			// No request will remain into the queue after this method
			pHasRequests = false;

			// This method may sometimes be called even if there is no request
			// in the list.
			if (pRequests->empty())
				return true;

			// We will take the ownership on the list, and will create a new one
			// to release the mutex as soon as possible and to process the requests
			// asynchronously.
			requests = pRequests;
			pRequests = new RequestListType();

			// The mutex is unlocked here
		}

		// Assert
		assert(requests != NULL && "IEventLoop: the request list is NULL");

		// Executing all requests
		typename RequestListType::const_iterator end = requests->end();
		for (typename RequestListType::const_iterator i = requests->begin(); i != end; ++i)
		{
			// Statistics
			StatisticsPolicy::onProcessRequest(*i);

			// Processing the request
			if (not (*i)())
			{
				// The request has failed. Aborting now.
				delete requests;
				return false;
			}
		}

		// The request list has been processed and can be destroyed
		delete requests;
		return true;
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	inline void
	IEventLoop<ParentT,FlowT,StatsT,DetachedT>::suspend(uint timeout)
	{
		if (detached)
		{
			// In detached mode, the thread pointer is valid. We will use the
			// method suspend which is far better (cancellation point) than a mere
			// sleep.
			assert(pThread and "invalid thread pointer");
			pThread->suspendTheThread(timeout);
		}
		else
		{
			// However, when not in detached mode, we don't have any thread instance
			// to use. The only way to achieve a pause is to use a sleep
			SuspendMilliSeconds(timeout);
		}
	}



	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	inline IEventLoop<ParentT,FlowT,StatsT,DetachedT>&
	IEventLoop<ParentT,FlowT,StatsT,DetachedT>::operator += (const typename IEventLoop<ParentT,FlowT,StatsT,DetachedT>::RequestType& request)
	{
		dispatch(request);
		return *this;
	}


	template<class ParentT, template<class> class FlowT, template<class> class StatsT,
		bool DetachedT>
	inline IEventLoop<ParentT,FlowT,StatsT,DetachedT>&
	IEventLoop<ParentT,FlowT,StatsT,DetachedT>::operator << (const typename IEventLoop<ParentT,FlowT,StatsT,DetachedT>::RequestType& request)
	{
		dispatch(request);
		return *this;
	}





} // namespace EventLoop
} // namespace Core
} // namespace Yuni
