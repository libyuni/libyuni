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
#include "thread.h"



namespace Yuni
{
namespace Private
{
namespace QueueService
{


	inline void QueueThread::notifyEndOfWork()
	{
		// Notify the scheduler that this thread goes to sleep
		pQueueService.unregisterWorker(this);
	}


	bool QueueThread::onExecute()
	{
		// Notify the scheduler that this thread has begun its work
		pQueueService.registerWorker(this);

		// Asking for the next job
		while (pQueueService.pWaitingRoom.pop(pJob))
		{
			// Execute the job, via a wrapper for symbol visibility issues
			Yuni::Private::QueueService::JobAccessor<Yuni::Job::IJob>::Execute(*pJob, this);

			// We must release our pointer to the job here to avoid its destruction
			// in `pQueueService.nextJob()` (when `pJob` is re-assigned).
			// This method uses a lock and the destruction of the job may take some time.
			// Obviously, there is absolutely no guarantee that the job will be destroyed
			// at this point but we don't really care
			pJob = nullptr;

			// Cancellation point
			if (YUNI_UNLIKELY(shouldAbort())) // We have to stop as soon as possible, no need for hibernation
				return false;

		} // loop for retrieving jobs to execute

		// Returning true, for hibernation
		return true;
	}


	void QueueThread::onStop()
	{
		notifyEndOfWork(); // we are done here !
	}


	void QueueThread::onPause()
	{
		notifyEndOfWork(); // we are done here !
	}


	void QueueThread::onKill()
	{
		if (!(!(pJob)))
		{
			// Notify the job that it has been killed
			// (via the wrapper for symbol visibility issues)
			Yuni::Private::QueueService::JobAccessor<Yuni::Job::IJob>::ThreadHasBeenKilled(*pJob);
			// Release the pointer, if possible of course
			pJob = nullptr;
		}

		notifyEndOfWork(); // we are done here !
	}






} // namespace QueueService
} // namespace Private
} // namespace Yuni

