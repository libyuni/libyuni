
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
#	warning check the definition of SIGKILL on VS and mingw
#	define SIGKILL SIGINT // SIGKILL is not defined on Windows
# endif
#else
#endif
#include "../../../datetime/timestamp.h"
#include "../../../io/directory.h"
#include "process-info.h"




namespace Yuni
{
namespace Process
{


	class Program::ThreadMonitor final : public Yuni::Thread::IThread
	{
	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ThreadMonitor(Yuni::Process::Program& process);
		//! Destructor
		virtual ~ThreadMonitor();
		//@}



		bool spawnProcess();


	protected:
		virtual bool onExecute() override;

		virtual void onPause();

		virtual void onStop();

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
		Yuni::Process::Program::ProcessSharedInfo::Ptr procinfoRef;
		//! Convient alias to the shared data
		Yuni::Process::Program::ProcessSharedInfo& procinfo;

		//! Reference to the original stream handler
		// \internal Automatically reset by \p theProcessHasStopped
		Stream::Ptr stream;

		# ifndef YUNI_OS_WINDOWS
		struct
		{
			int outfd[2];
			int infd[2];
			int errd[2];
		}
		channels;
		pid_t pid;
		#endif

		bool pRedirectToConsole;

		int pExitStatus;
		//! Flag to determine whether the process was killed or not
		bool pKilled;

		sint64 pStartTime;
		sint64 pEndTime;

		//! Duration precision
		const DurationPrecision pDurationPrecision;

	}; // class ThreadMonitor




	inline Program::ThreadMonitor::ThreadMonitor(Yuni::Process::Program& process)
		: procinfoRef(process.pEnv)
		, procinfo(*(process.pEnv))
		, stream(process.pStream)
		, pRedirectToConsole(procinfo.redirectToConsole)
		, pExitStatus(-1)
		, pKilled(false)
		, pStartTime(-1)
		, pEndTime(0)
		, pDurationPrecision(process.pEnv->durationPrecision)
	{
		// to avoid compiler warning
		(void) procinfoRef;

		#ifndef YUNI_OS_WINDOWS
		pid = -1;
		#endif
	}


	inline Program::ThreadMonitor::~ThreadMonitor()
	{
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


	inline void Program::ThreadMonitor::onPause()
	{
	}


	inline void Program::ThreadMonitor::onStop()
	{
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


	Program::~Program()
	{
		// keep the symbol local
	}


	void Program::signal(int sig)
	{
		#ifndef YUNI_OS_MSVC
		ProcessSharedInfo::Ptr envptr = pEnv;
		if (!(!envptr))
			envptr->sendSignal<true>(sig);
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
		// new environment
		ProcessSharedInfo::Ptr envptr = pEnv;
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
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
		// prepare commands
		ThreadMonitor* newthread = new ThreadMonitor(*this);

		// keep a local reference to avoid race condition if `env.thread` is modified
		// by another thread
		ThreadMonitor::Ptr localRef = newthread;
		(void) localRef; // avoid compiler warning
		// keep somewhere
		env.thread = newthread;

		// execute the sub command from the **calling** thread
		bool processReady = newthread->spawnProcess();

		// start a sub thread to monitor the underlying process
		if (processReady)
			newthread->start();
		return processReady;
	}


	int Program::wait(sint64* duration)
	{
		ProcessSharedInfo::Ptr envptr = pEnv;
		if (YUNI_UNLIKELY(!envptr))
		{
			if (duration)
				*duration = 0;
			return 0;
		}
		ProcessSharedInfo& env = *envptr;

		ThreadPtr thread;
		// checking environment
		{
			MutexLocker locker(env.mutex);
			if (not env.running or not env.thread)
			{
				if (duration)
					*duration = env.duration;
				return env.exitstatus;
			}
			// capture the thread
			thread = env.thread;
		}

		// wait for the end of the thread
		thread->wait();

		// since the thread has finished, we can safely destroy it
		env.thread = nullptr;

		// results
		MutexLocker locker(env.mutex);
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
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
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
		ProcessSharedInfo::Ptr envptr = pEnv;
		return (!envptr) ? false : (envptr->running);
	}


	void Program::commandLine(const AnyString& cmd)
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		env.executable.clear();
		env.arguments.clear();

		if (not cmd.empty())
		{
			uint offset = 0;
			bool foundExecutable = false;

			do
			{
				// Looking for the next whitespace
				offset = cmd.find_first_not_of(" \t\r\n", offset);
				if (offset >= cmd.size())
					break;

				// ok, we have a new entry
				uint next = offset;
				bool escape = false;
				char withinString = '\0';
				do
				{
					char c = cmd[next];
					if (c == '\\')
					{
						escape = not escape;
					}
					else
					{
						if (c == '\'' or c == '"')
						{
							if (not escape)
							{
								if (withinString == c)
									withinString = '\0'; // end of literal
								else if (withinString == '\0') // starts a new literal
									withinString = c;
							}
						}
						if (withinString == '\0')
						{
							if (c == ' ' or c == '\t' or c == '\r' or c == '\n')
								break;
						}
					}
					++next;
				}
				while (next < cmd.size());

				if (next > offset)
				{
					AnyString arg;
					if (next - offset > 1 and cmd[next - 1] == cmd[offset] and (cmd[offset] == '"' or cmd[offset] == '\''))
						arg.adapt(cmd.c_str() + offset + 1, next - offset - 2);
					else
						arg.adapt(cmd.c_str() + offset, next - offset);

					if (not foundExecutable)
						env.executable = arg;
					else
						env.arguments.push_back(arg);
					foundExecutable = true;
				}
				offset = next;
			}
			while (offset < cmd.size());
		}
	}


	void Program::workingDirectory(const AnyString& directory)
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		env.workingDirectory = directory;
	}


	String Program::workingDirectory() const
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return nullptr;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.workingDirectory;
	}


	bool Program::redirectToConsole() const
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return true;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.redirectToConsole;

	}


	void Program::redirectToConsole(bool flag)
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			if (flag)
				return; // default is true, useless to instanciate something

			envptr = new ProcessSharedInfo();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		env.redirectToConsole = flag;
	}



	String Program::program() const
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return nullptr;
		ProcessSharedInfo& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.executable;
	}


	void Program::program(const AnyString& prgm)
	{
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
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
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
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
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
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
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
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
		ProcessSharedInfo::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessSharedInfo();
			pEnv = envptr;
		}
		ProcessSharedInfo& env = *envptr;

		MutexLocker locker(env.mutex);
		assert((not env.running) and "a precision can not be updated while the process is running");
		if (not env.running)
			env.durationPrecision = precision;
	}






} // namespace Process
} // namespace Yuni

