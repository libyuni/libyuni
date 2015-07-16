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
#include "process-info.h"


namespace Yuni
{
namespace Process
{


	inline Program::ProcessSharedInfo::ProcessSharedInfo()
		: running(false)
		, processID(-1)
		, processInput(-1)
		, duration(0)
		, durationPrecision(dpSeconds)
		, timeout()
		, exitstatus(-1)
		, redirectToConsole(false)
		, timeoutThread(nullptr)
	{}


	inline Program::ProcessSharedInfo::~ProcessSharedInfo()
	{
		if (YUNI_UNLIKELY(timeoutThread))
		{
			// should never go in this section
			assert(false and "the thread for handling the timeout has not been properly stopped");
			timeoutThread->stop();
			delete timeoutThread;
		}
	}


	# ifdef YUNI_OS_WINDOWS

	namespace // anonymous
	{

		void QuitProcess(DWORD processID)
		{
			assert(processID != 0);
			// Loop on process windows
			for (HWND hwnd = ::GetTopWindow(nullptr); hwnd; hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT))
			{
				DWORD windowProcessID;
				DWORD threadID = ::GetWindowThreadProcessId(hwnd, &windowProcessID);
				if (windowProcessID == processID)
					// Send WM_QUIT to the process thread
					::PostThreadMessage(threadID, WM_QUIT, 0, 0);
			}
		}

	} // namespace anonymous

	# endif


	template<bool WithLock>
	inline bool Program::ProcessSharedInfo::sendSignal(int sigvalue)
	{
		if (WithLock)
			mutex.lock();
		if (0 == running)
		{
			if (WithLock)
				mutex.unlock();
			return false;
		}

		# ifndef YUNI_OS_WINDOWS
		{
			const pid_t pid = static_cast<pid_t>(processID);
			if (WithLock)
				mutex.unlock();
			if (pid > 0)
				return (0 == ::kill(pid, sigvalue));
		}
		# else
		{
			switch (sigvalue)
			{
			// All signals are handled by force-quitting the child process' window threads.
			default:
				QuitProcess(processID);
				break;
			}

			if (WithLock)
				mutex.unlock();
		}
		# endif

		return false;
	}




	namespace // anonymous
	{

		class TimeoutThread final : public Thread::IThread
		{
		public:
			TimeoutThread(int pid, uint timeout)
				: timeout(timeout)
				, pid(pid)
			{
				assert(pid > 0);
			}

			~TimeoutThread() {}


		protected:
			virtual bool onExecute() override
			{
				// wait for timeout... (note: the timeout is in seconds)
				if (not suspend(timeout * 1000))
				{
					// the timeout has been reached

					#ifdef YUNI_OS_UNIX
					::kill(pid, SIGKILL);
					#else
					QuitProcess(pid);
					#endif
				}
				return false; // stop the thread, does not suspend it
			}


		private:
			//! Timeout in seconds
			uint timeout;
			//! PID of the sub process
			int pid;
		};



	} // anonymous namespace


	inline void Program::ProcessSharedInfo::createThreadForTimeoutWL()
	{
		delete timeoutThread; // for code safety

		if (processID > 0 and timeout > 0)
		{
			timeoutThread = new TimeoutThread(processID, timeout);
			timeoutThread->start();
		}
		else
		{
			// no valid pid, no thread required for a timeout
			timeoutThread = nullptr;
		}
	}







} // namespace Process
} // namespace Yuni

