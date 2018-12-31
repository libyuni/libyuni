/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#include "program.h"
#include "../../../thread/thread.h"
#ifndef YUNI_OS_MSVC
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# ifndef YUNI_OS_WINDOWS
#	include <sys/wait.h>
# else
#	define SIGKILL SIGTERM // SIGKILL is not defined on Windows
# endif
#else
#endif
#include "../../../datetime/timestamp.h"
#include "../../../io/directory.h"
#include "process-info.h"
#include <iostream>

#ifdef YUNI_OS_UNIX
# include <sys/types.h>
#endif
#include <signal.h>




namespace Yuni
{
namespace Process
{


	struct Program::ThreadMonitor final : public Yuni::Thread::IThread
	{
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ThreadMonitor(Yuni::Process::Program& process);
		//! Destructor
		virtual ~ThreadMonitor() = default;
		//@}


		bool spawnProcess();


	protected:
		virtual bool onExecute() override;

		virtual void onPause() override {}

		virtual void onStop() override {}

		virtual void onKill() override;

	private:
		void prepareCommandLineWL();
		void waitForSubProcess();
		void cleanupAfterChildTermination();
		void theProcessHasStopped(bool killed, int exitstatus);

		//! Get the current timestamp according the requested precision
		sint64 currentTime() const;


	private:
		typedef char* CharPtr;

	private:
		//! Reference to the original shared data structure
		std::shared_ptr<Yuni::Process::Program::ProcessSharedInfo> procinfoRef;
		//! Convient alias to the shared data
		Yuni::Process::Program::ProcessSharedInfo& procinfo;

		//! Reference to the original stream handler
		// \internal Automatically reset by \p theProcessHasStopped
		std::shared_ptr<Stream> stream;

		# ifndef YUNI_OS_WINDOWS
		typedef int FD;
		pid_t pid = -1;
		#else
		typedef HANDLE FD;
		HANDLE processHandle;
		HANDLE threadHandle;
		#endif

		struct
		{
			FD outfd[2];
			FD infd[2];
			FD errd[2];
		}
		channels;

		bool pRedirectToConsole = false;

		int pExitStatus = -1;
		//! Flag to determine whether the process was killed or not
		bool pKilled = false;

		sint64 pStartTime = -1;
		sint64 pEndTime = 0;

		//! Duration precision
		const DurationPrecision pDurationPrecision;

	}; // class ThreadMonitor




	inline Program::ThreadMonitor::ThreadMonitor(Yuni::Process::Program& process)
		: procinfoRef(process.pEnv)
		, procinfo(*(process.pEnv))
		, stream(process.pStream)
		, pRedirectToConsole(procinfo.redirectToConsole)
		, pDurationPrecision(process.pEnv->durationPrecision)
	{
		// to avoid compiler warning
		(void) procinfoRef;
	}


	inline sint64 Program::ThreadMonitor::currentTime() const
	{
		switch (pDurationPrecision)
		{
			case dpSeconds:      return Yuni::DateTime::Now();
			case dpMilliseconds: return Yuni::DateTime::NowMilliSeconds();
			case dpNone:         return 0;
		}
		assert(false and "precision type not handled");
		return 0;
	}


	bool Program::ThreadMonitor::onExecute()
	{
		// some reset, just in case
		pExitStatus = 0;
		pKilled     = false;
		pEndTime    = 0;

		waitForSubProcess();
		cleanupAfterChildTermination();

		theProcessHasStopped(pKilled, pExitStatus);
		return false; // stop the thread
	}


	void Program::ThreadMonitor::theProcessHasStopped(bool killed, int exitstatus)
	{
		// prevent against invalid code
		if (YUNI_UNLIKELY(pEndTime == 0))
		{
			assert(false and "endTime is invalid");
			pEndTime = currentTime();
		}

		// execution time for the sub-process
		sint64 duration = (pEndTime >= pStartTime) ? (pEndTime - pStartTime) : 0;

		// Making sure that the process ID is invalid
		{
			MutexLocker locker(procinfo.mutex);
			if (YUNI_UNLIKELY(not procinfo.running)) // already stopped - should never happen
				return;

			procinfo.running      = false;
			procinfo.processInput = -1;
			procinfo.exitstatus   = exitstatus;
			procinfo.duration     = duration;
		}

		if (!(!stream))
		{
			stream->onStop(killed, exitstatus, duration);
			// remove the reference to the stream as soon as possible
			stream = nullptr;
		}
	}









	Program::ProcessSharedInfo::~ProcessSharedInfo()
	{
		if (YUNI_UNLIKELY(timeoutThread.get()))
		{
			// should never go in this section
			assert(false and "the thread for handling the timeout has not been properly stopped");
			timeoutThread->stop();
		}

		if (thread and thread->release())
			delete thread;
	}


	# ifdef YUNI_OS_WINDOWS
	namespace // anonymous
	{

		static inline void QuitProcess(DWORD processID)
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


	bool Program::ProcessSharedInfo::sendSignal(bool withLock, int sigvalue)
	{
		if (withLock)
			mutex.lock();
		if (0 == running)
		{
			if (withLock)
				mutex.unlock();
			return false;
		}

		# ifndef YUNI_OS_WINDOWS
		{
			const pid_t pid = static_cast<pid_t>(processID);
			if (withLock)
				mutex.unlock();
			if (pid > 0)
				return (0 == ::kill(pid, sigvalue));
		}
		# else
		{
			// switch (sigvalue) ...

			// All signals are handled by force-quitting the child process' window threads.
			QuitProcess(processID);

			if (withLock)
				mutex.unlock();
		}
		# endif

		// mutex must be unlocked here
		return false;
	}




	namespace // anonymous
	{

		class TimeoutThread final: public Thread::IThread
		{
		public:
			TimeoutThread(int pid, uint timeout)
				: timeout(timeout)
				, pid(pid)
			{
				assert(pid > 0);
			}

			virtual ~TimeoutThread() {}


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


	void Program::ProcessSharedInfo::createThreadForTimeoutWL()
	{
		timeoutThread.reset();
		if (processID > 0 and timeout > 0)
		{
			timeoutThread = std::make_unique<TimeoutThread>(processID, timeout);
			timeoutThread->start();
		}
	}



} // namespace Process
} // namespace Yuni

#include "unix.inc.hpp"
#include "windows.inc.hpp"




namespace Yuni
{
namespace Process
{


	Program::Program()
	{
		// keep the symbol local
	}


	Program::Program(const Program& rhs)
		: pEnv(rhs.pEnv)
		, pStream(rhs.pStream)
	{
		// keep the symbol local
	}


	Program::~Program()
	{
		// keep the symbol local
	}


	Program& Program::operator = (const Program& rhs)
	{
		pEnv = rhs.pEnv;
		pStream = rhs.pStream;
		return *this;
	}


	void Program::signal(int sig)
	{
		#ifndef YUNI_OS_MSVC
		auto envptr = pEnv;
		if (!!envptr)
			envptr->sendSignal(true, sig);
		#else
		// Signals are not supported on Windows. Silently ignoring it.
		(void) sig;
		#endif
	}


	void Program::terminate()
	{
		#ifndef YUNI_OS_MSVC
		this->signal(SIGTERM);
		#endif
	}



	void Program::kill()
	{
		#ifndef YUNI_OS_MSVC
		this->signal(SIGKILL);
		#endif
	}


	bool Program::execute(uint timeout)
	{
		auto envptr = pEnv;
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);

		if (env.running)
			return false;

		// notifying that the process is running
		env.running      = true;

		// some reset
		env.processID    = 0;
		env.processInput = -1;
		env.exitstatus   = -128;
		env.timeout      = timeout;
		env.duration     = 0;

		if (env.executable.empty())
		{
			env.running = false;
			return true;
		}

		// starting a new thread
		ThreadMonitor* localRef = new (std::nothrow) ThreadMonitor(*this);
		if (!localRef)
			return false;

		localRef->addRef();
		// keep somewhere
		env.thread = localRef;
		// execute the sub command from the **calling** thread
		bool processReady = localRef->spawnProcess();

		// start a sub thread to monitor the underlying process
		if (processReady)
			localRef->start();
		return processReady;
	}


	int Program::wait(sint64* duration)
	{
		auto envptr = pEnv;
		if (YUNI_UNLIKELY(!envptr))
		{
			if (duration)
				*duration = 0;
			return 0;
		}
		ProcessSharedInfo& env = *envptr;

		ThreadMonitor* thread = nullptr;
		// checking environment
		{
			MutexLocker locker(env.mutex);
			if (not env.running or (nullptr == env.thread))
			{
				if (duration)
					*duration = env.duration;
				return env.exitstatus;
			}
			// capture the thread
			thread = env.thread;
			thread->addRef();
		}

		// wait for the end of the thread
		assert(thread != nullptr);
		thread->wait();


		MutexLocker locker(env.mutex);

		// since the thread has finished, we can safely destroy it
		if (env.thread)
			env.thread->release();
		env.thread = nullptr;

		if (thread->release())
			delete thread;

		if (duration)
			*duration = env.duration;
		return env.exitstatus;
	}














	namespace // anonymous
	{

		class ExecutionHelper final
		{
		public:
			//! The most suitable smart pointer for the class
			typedef SmartPtr<ExecutionHelper> Ptr;

		public:
			ExecutionHelper(Process::Program& process, Thread::Signal& signal)
				: signal(signal)
				, process(process)
			{
				# ifndef NDEBUG
				pAlreadyLaunched = false;
				# endif
			}

			~ExecutionHelper()
			{
			}

			bool perform(uint timeout)
			{
				# ifndef NDEBUG
				assert(!pAlreadyLaunched and "internal execute error");
				pAlreadyLaunched = true;
				# endif

				// Start the process (e.g. fork)
				result = process.execute(timeout);

				// notifying any waiter that the process has been launched
				// it will reduce contention if the notify is called from here instead
				// from the destructor
				// After this call, no other operation must be performed
				signal.notify();
				return true; // let the main loop continue
			}

		public:
			//! The command to execute
			String command;
			//!
			Thread::Signal& signal;
			//! Process to execute (to keep inner data alive)
			Process::Program process;
			//!
			bool result;

		private:
			# ifndef NDEBUG
			//! Flag to prevent against multiple launches (which must never happen)
			bool pAlreadyLaunched;
			# endif

		}; // class ExecutionHelper

	} // anonymous namespace


	bool Program::dispatchExecution(const Bind<void (const Callback&)>& dispatcher, uint timeout)
	{
		auto envptr = pEnv;
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}

		Thread::Signal signal;
		if (not signal.valid())
		{
			std::cerr << "impossible to initialize signal\n";
			return false;
		}

		// Dispatch the message
		// The code is within a block to let the variable
		// `runner` destroyed, thus to notify the end of the execution
		// (via signal->notify())
		ExecutionHelper runner(*this, signal);

		// scope to destroy our callback as soon as possible
		{
			Bind<bool ()>  callback;
			callback.bind(&runner, &ExecutionHelper::perform, timeout);
			dispatcher(callback);
		}

		// waiting for the process startup from the main loop
		signal.wait();

		// ok ready
		return runner.result;
	}


	bool Program::running() const
	{
		auto envptr = pEnv;
		return (!envptr) ? false : (envptr->running);
	}


	void Program::commandLine(AnyString cmd)
	{
		// remove all whitespaces
		cmd.trim();

		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		env.executable.clear();
		env.arguments.clear();

		if (cmd.empty())
			return;


		String* str = &env.executable;
		char instring = '\0';
		const AnyString::null_iterator end = cmd.utf8end();
		for (AnyString::const_utf8iterator i = cmd.utf8begin(); i != end; ++i)
		{
			char c = *i;
			switch (c)
			{
				default:
				{
					*str += i.value();
					break;
				}
				case '"':
					[[fallthrough]];
				case '\'':
				{
					if (instring == '\0')
					{
						instring = c;
					}
					else
					{
						if (instring == c)
							instring = '\0';
						else
							*str += c;
					}
					break;
				}
				case '\\':
				{
					++i;
					if (YUNI_UNLIKELY(i == end))
						return;
					c = *i;
					switch (c)
					{
						case 'n':  (*str) += '\n'; break;
						case 't':  (*str) += '\t'; break;
						case 'r':  (*str) += '\r'; break;
						case 'b':  (*str) += '\b'; break;
						case 'f':  (*str) += '\f'; break;
						case 'v':  (*str) += '\v'; break;
						case '0':  (*str) += '\0'; break;
						case 'e': [[fallthrough]];
						case 'a': [[fallthrough]];
						case 'E':  break;
						default:   (*str) << '\\' << c; break;
					}
				}
				case ' ':
					[[fallthrough]];
				case '\t':
				{
					if (instring == '\0')
					{
						if (not str->empty())
						{
							env.arguments.push_back(nullptr);
							str = &(env.arguments.back());
						}
					}
					else
						*str += c;
					break;
				}
			}
		}
	}


	void Program::workingDirectory(const AnyString& directory)
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		env.workingDirectory = directory;
	}


	String Program::workingDirectory() const
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return nullptr;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.workingDirectory;
	}


	bool Program::redirectToConsole() const
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return true;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.redirectToConsole;

	}


	void Program::redirectToConsole(bool flag)
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			if (flag)
				return; // default is true, useless to instanciate something
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		env.redirectToConsole = flag;
	}



	String Program::program() const
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return nullptr;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.executable;
	}


	void Program::program(const AnyString& prgm)
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		assert((not env.running) and "the process name can not be updated while the process is running");
		if (not env.running)
			env.executable = prgm;
	}


	void Program::argumentClear()
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		assert((not env.running) and "the arguments can not be updated while the process is running");
		if (not env.running)
			env.arguments.clear();
	}


	void Program::argumentAdd(const AnyString& arg)
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		assert((not env.running) and "the arguments can not be updated while the process is running");
		if (not env.running)
			env.arguments.push_back(arg);
	}


	Program::DurationPrecision  Program::durationPrecision() const
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!(!envptr))
		{
			ProcessSharedInfo& env = *envptr;
			MutexLocker locker(env.mutex);
			return env.durationPrecision;
		}
		return dpSeconds;
	}


	void Program::durationPrecision(Program::DurationPrecision precision)
	{
		auto envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = std::make_shared<ProcessSharedInfo>();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		assert((not env.running) and "a precision can not be updated while the process is running");
		if (not env.running)
			env.durationPrecision = precision;
	}


	bool Execute(const AnyString& commandLine, uint timeout)
	{
		Program program;
		program.commandLine(commandLine);
		return (program.execute(timeout)) ? (0 == program.wait()) : false;
	}


	bool System(String* cout, String* cerr, const AnyString& commandline, uint timeout)
	{
		Program program;
		program.commandLine(commandline);

		auto output = std::make_shared<CaptureOutput>();
		program.stream(output);
		bool success = program.execute(timeout) and (0 == program.wait());

		if (cout)
			*cout = output->cout;
		if (cerr)
			*cerr = output->cerr;
		return success;
	}


	String System(const AnyString& commandline, bool trim, uint timeout)
	{
		Program program;
		program.commandLine(commandline);

		auto output = std::make_shared<CaptureOutput>();
		program.stream(output);
		program.execute(timeout) and (0 == program.wait());
		if (trim)
			output->cout.trim();
		return output->cout;
	}




} // namespace Process
} // namespace Yuni
