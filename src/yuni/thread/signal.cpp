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
#include "signal.h"
#include <cassert>

#ifndef YUNI_NO_THREAD_SAFE
# ifdef YUNI_OS_WINDOWS
#	include "../core/system/windows.hdr.h"
# else
#	include <time.h>
#	include <sys/time.h>
#	include <errno.h>
#	include "../core/system/gettimeofday.h"
# endif
#endif


namespace Yuni
{
namespace Thread
{


	Signal::Signal()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		// Making sure that our pseudo HANDLE type is valid
		assert(sizeof(HANDLE) >= sizeof(void*) and "Invalid type for Signal::pHandle");

		pHandle = (void*) CreateEvent(
			NULL,     // default security attributes
			TRUE,     // manual-reset event
			FALSE,    // initial state is nonsignaled
			NULL );   // unamed

		# else
		pSignalled = false;
		::pthread_mutex_init(&pMutex, nullptr);
		::pthread_cond_init(&pCondition, nullptr);
		# endif
		#endif
	}


	Signal::Signal(const Signal&)
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		// Making sure that our pseudo HANDLE type is valid
		assert(sizeof(HANDLE) >= sizeof(void*) and "Invalid type for Signal::pHandle");

		pHandle = (void*) CreateEvent(
			NULL,     // default security attributes
			TRUE,     // manual-reset event
			FALSE,    // initial state is nonsignaled
			NULL );   // unamed

		# else
		pSignalled = false;
		::pthread_mutex_init(&pMutex, nullptr);
		::pthread_cond_init(&pCondition, nullptr);
		# endif
		#endif
	}


	Signal::~Signal()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		CloseHandle(pHandle);
		# else
		::pthread_cond_destroy(&pCondition);
		::pthread_mutex_destroy(&pMutex);
		# endif
		#endif
	}


	bool Signal::reset()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		return (pHandle and ResetEvent(pHandle));
		# else

		::pthread_mutex_lock(&pMutex);
		pSignalled = false;
		::pthread_mutex_unlock(&pMutex);
		return true;
		# endif

		#else // NO THREADSAFE
		return true;
		#endif
	}


	void Signal::wait()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS

		if (pHandle)
			WaitForSingleObject(pHandle, INFINITE);

		# else

		// The pthread_cond_wait will unlock the mutex and wait for
		// signalling.
		::pthread_mutex_lock(&pMutex);

		while (not pSignalled)
		{
			// Wait for signal
			// Note that the pthread_cond_wait routine will automatically and
			// atomically unlock mutex while it waits.
			//
			// Spurious wakeups from this function can occur.
			// Therefore we must check out pSignalled variable to ensure we have
			// really been signalled.
			::pthread_cond_wait(&pCondition, &pMutex);
		}

		// The condition was signalled: the mutex is now locked again.
		::pthread_mutex_unlock(&pMutex);
		# endif

		# else // NO THREADSAFE
		# endif
	}


	void Signal::waitAndReset()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS

		if (pHandle)
		{
			WaitForSingleObject(pHandle, INFINITE);
			ResetEvent(pHandle);
		}

		# else

		// The pthread_cond_wait will unlock the mutex and wait for
		// signalling.
		::pthread_mutex_lock(&pMutex);

		while (not pSignalled)
		{
			// Wait for signal
			// Note that the pthread_cond_wait routine will automatically and
			// atomically unlock mutex while it waits.
			//
			// Spurious wakeups from this function can occur.
			// Therefore we must check out pSignalled variable to ensure we have
			// really been signalled.
			::pthread_cond_wait(&pCondition, &pMutex);
		}

		// reset
		pSignalled = false;

		// The condition was signalled: the mutex is now locked again.
		::pthread_mutex_unlock(&pMutex);
		# endif

		# else // NO THREADSAFE
		# endif

	}


	bool Signal::wait(uint timeout)
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		if (pHandle)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(pHandle, (DWORD) timeout))
				return true;
		}
		return false;
		# else

		::pthread_mutex_lock(&pMutex);
		if (pSignalled)
		{
			::pthread_mutex_unlock(&pMutex);
			return true;
		}

		Yuni::timeval now;
		struct timespec t;

		// Set the timespec t at [timeout] milliseconds in the future.
		assert(timeout < 2147483648u and "Invalid range for timeout (Signal::wait(timeout))");
		YUNI_SYSTEM_GETTIMEOFDAY(&now, NULL);
		t.tv_nsec  =  (long)   (now.tv_usec * 1000 + (((int) timeout % 1000) * 1000000));
		t.tv_sec   =  (time_t) (now.tv_sec + timeout / 1000 + (t.tv_nsec / 1000000000L));
		t.tv_nsec  %= 1000000000L;

		int error = 0;

		do
		{
			// Wait for signal
			// Note that the pthread_cond_wait routine will automatically and
			// atomically unlock mutex while it waits.
			//
			// Avoid spurious wakeups (see wait() above for explanations)
			error = ::pthread_cond_timedwait(&pCondition, &pMutex, &t);
		}
		while (not pSignalled       // Condition not verified
			and error != ETIMEDOUT  // We have not timedout
			and error != EINVAL);   // When t is in the past, we got EINVAL. We consider this as a timeout.

		bool result = (pSignalled != false);
		::pthread_mutex_unlock(&pMutex);
		// The condition was signalled or has timeoutted:
		return result;
		# endif

		# else // NO THREADSAFE
		return true;
		# endif
	}


	bool Signal::notify()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS

		return (pHandle and SetEvent(pHandle));

		# else

		::pthread_mutex_lock(&pMutex);
		pSignalled = true;
		::pthread_cond_signal(&pCondition);
		::pthread_mutex_unlock(&pMutex);
		return true;

		# endif

		# else // NO THREADSAFE
		return true;
		# endif
	}





} // namespace Thread
} // namespace Yuni
