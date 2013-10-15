
#include "process.h"
#include "../../thread/thread.h"
#ifndef YUNI_OS_MSVC
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# ifndef YUNI_OS_WINDOWS
#	include <sys/wait.h>
# else
#	define SIGKILL SIGINT
# endif
#else
#endif
#include "../../datetime/timestamp.h"

// http://msdn.microsoft.com/en-us/library/ms682499%28v=vs.85%29.aspx


namespace Yuni
{
namespace Private
{
namespace Process
{


	class SubProcess final : public Yuni::Thread::IThread
	{
	private:
		struct Data
		{
			# ifndef YUNI_OS_WINDOWS
			int outfd[2];
			int infd[2];
			int errd[2];
			int oldstdin, oldstdout, oldstderr;
			pid_t pid;
			# endif
		};
		Data pData;

	public:
		SubProcess(Yuni::Process& process) :
			pProcess(process),
			pArguments(nullptr),
			pRedirectToConsole(true)
		{
		}

		virtual ~SubProcess()
		{
			deleteAllArguments();
		}

		void command(const String& program, const String::Vector& args)
		{
			# ifndef YUNI_OS_WINDOWS
			pProgram = program;
			deleteAllArguments();

			uint count = (uint) args.size();
			if (count != 0)
			{
				pArguments = (CharPtr*) ::malloc(sizeof(CharPtr) * (count + 2));
				{
					uint csize = pProgram.size();
					const char* source = pProgram.c_str();

					char* target = (char*) ::malloc(sizeof(char) * (csize + 1));
					YUNI_MEMCPY(target, csize + 1, source, csize);
					target[csize] = '\0';
					pArguments[0] = target;
				}
				for (uint i = 0; i != count; ++i)
				{
					uint csize = args[i].size();
					const char* source = args[i].c_str();

					char* target = (char*) ::malloc(sizeof(char) * (csize + 1));
					YUNI_MEMCPY(target, csize + 1, source, csize);
					target[csize] = '\0';
					pArguments[i + 1] = target;
				}
				pArguments[count + 1] = nullptr;
			}
			# endif
		}


		bool launch()
		{
			Yuni::Process::ProcessEnvironment::Ptr envptr = pProcess.pEnv;
			if (not envptr)
			{
				assert(false and "Launching a new process with an invalid thread environment");
				return false; // should never happen
			}
			Yuni::Process::ProcessEnvironment& env = *envptr;

			if (pProgram.empty())
			{
				env.mutex.unlock();
				theProcessHasStopped(false, 0, -1);
				std::cerr << "empty program";
				return true;
			}

			# ifndef YUNI_OS_WINDOWS

			// Getting the start time of execution
			env.startTime = Yuni::DateTime::Now();
			// unlock mutex
			env.mutex.unlock();

			// The parent is going to write into
			// pipe(pData.outfd)
			// The parent is going to read from
			// pipe(pData.infd)
			// The parent is going to read from (cerr)
			// pipe(pData.errd)
			if (/*input*/pipe(pData.outfd) or /*output*/pipe(pData.infd) or pipe(pData.errd))
			{
				switch (errno)
				{
					case EFAULT:
						std::cerr << "pipe failed: invalid file descriptor\n";
						break;
					case EINVAL:
						std::cerr << "pipe failed: invalid flags\n";
						break;
					case EMFILE:
						std::cerr << "pipe failed: Too many file descriptors are in use by the process\n";
						break;
					case ENFILE:
						std::cerr << "pipe failed: The system limit on the total number of open files has been reached\n";
						break;
				}
				return false;
			}

			if (-1 == (pData.pid = fork()))
			{
				std::cerr << "fork failed\n";
				return false;
			}
			if (0 == pData.pid) // CHILD
			{
				close(pData.infd[0]);
				close(pData.errd[0]);
				close(pData.outfd[1]);
				close(0);
				close(1);
				close(2);

				dup2(pData.outfd[0], 0); // stdin  is pipeout
				dup2(pData.infd[1], 1);  // stdout is pipeout
				dup2(pData.errd[1], 2);  // stderr is pipeout

				close(pData.outfd[0]);
				close(pData.infd[1]);
				close(pData.errd[1]);

				// restoring signal handlers
				signal(SIGTERM, SIG_DFL);
				signal(SIGINT,  SIG_DFL);
				signal(SIGPIPE, SIG_DFL);
				signal(SIGCHLD, SIG_DFL);

				if (not env.workingDirectory.empty())
				{
					// ignoring return value
					if (0 != chdir(env.workingDirectory.c_str()))
						std::cerr << "invalid working directory: " << env.workingDirectory << std::endl;
				}

				// Should never returns
				execvp(pProgram.c_str(), pArguments);
				_exit(0);
			}
			else // PARENT
			{
				close(pData.outfd[0]);
				close(pData.infd[1]);
				close(pData.errd[1]);

				// make the standard output non-blocking
				fcntl(pData.infd[0], F_SETFL, O_NONBLOCK);
				fcntl(pData.errd[0], F_SETFL, O_NONBLOCK);

				// cleanup
				deleteAllArguments();

				//write(outfd[1], "2^32\n",5); // Write to child’s stdin

				// The mutex has already been locked and should be unlocked to let the main
				// pProcess.env.mutex.lock();
				env.mutex.lock();
				env.processInput = pData.outfd[1];
				env.processID = pData.pid;
				env.running = true;
				pRedirectToConsole = env.redirectToConsole;
				env.mutex.unlock();

				// really start the thread
				start();
			}

			return true;

			# else

			assert(false and "not implemented on Windows");
			env.mutex.unlock();
			return false;
			# endif
		}


	protected:
		virtual bool onExecute() override;

		virtual void onPause();

		virtual void onStop();

		# ifdef YUNI_OS_WINDOWS
		void onExecuteWindows();
		# endif
		# ifndef YUNI_OS_WINDOWS
		void onExecuteUnix();
		# endif


		virtual void onKill() override;

		void theProcessHasStopped(bool killed, int exitstatus, sint64 endTime);

		void deleteAllArguments();


	private:
		typedef char* CharPtr;

	private:
		Yuni::Process pProcess;
		String pProgram;
		CharPtr* pArguments;
		bool pRedirectToConsole;

		int pExitStatus;
		//! Flag to determine whether the process was killed or not
		bool pKilled;
		sint64 pEndTime;

	}; // class SubProcess



	bool SubProcess::onExecute()
	{
		pExitStatus = 0;
		pKilled     = false;
		pEndTime    = 0;

		# ifdef YUNI_OS_WINDOWS
		onExecuteWindows();
		# else
		onExecuteUnix();
		# endif

		// prevent against invalid code
		if (pEndTime == 0)
		{
			assert(false and "endTime is invalid");
			pEndTime = Yuni::DateTime::Now();
		}

		theProcessHasStopped(pKilled, pExitStatus, pEndTime);
		return false;
	}


	inline void SubProcess::onPause()
	{
	}


	inline void SubProcess::onStop()
	{
	}


	void SubProcess::onKill()
	{
		bool killed = false;
		# ifndef YUNI_OS_WINDOWS
		// try to kill the attached child process if any
		{
			Yuni::Process::ProcessEnvironment::Ptr envptr = pProcess.pEnv;
			if (not envptr)
				return;
			Yuni::Process::ProcessEnvironment& env = *envptr;

			MutexLocker locker(env.mutex);
			if (env.processID > 0)
			{
				kill(env.processID, SIGKILL);
				killed = true;
			}
		}
		# endif
		sint64 endTime = Yuni::DateTime::Now();
		theProcessHasStopped(killed, -127, endTime);
	}


	void SubProcess::theProcessHasStopped(bool killed, int exitstatus, sint64 endTime)
	{
		Yuni::Process::ProcessEnvironment::Ptr envptr = pProcess.pEnv;
		if (not envptr)
			return;
		Yuni::Process::ProcessEnvironment& env = *envptr;

		// Making sure that the process ID is invalid
		sint64 duration;
		{
			MutexLocker locker(env.mutex);
			if (!env.running) // already stopped - should never happen
				return;
			env.running      = 0;
			env.processInput = -1;
			env.exitstatus   = exitstatus;
			duration         = (endTime >= env.startTime) ? (endTime - env.startTime) : 0;
			env.duration     = duration;
		}

		if (!(!pProcess.pStream))
			pProcess.pStream->onStop(killed, exitstatus, duration);
	}


	void SubProcess::deleteAllArguments()
	{
		if (pArguments)
		{
			for (uint i = 0; pArguments[i]; ++i)
				::free(pArguments[i]);
			::free(pArguments);
			pArguments = nullptr;
		}
	}



	# ifdef YUNI_OS_WINDOWS
	void SubProcess::onExecuteWindows()
	{
		pEndTime = Yuni::DateTime::Now();
	}
	# endif


	# ifndef YUNI_OS_WINDOWS
	void SubProcess::onExecuteUnix()
	{
		Yuni::Process::Stream::Ptr stream = pProcess.pStream;
		bool hasStream = !(!stream);

		enum {bufferSize = 4096 * 2};
		char* buffer    = new char[bufferSize];
		char* buffererr = new char[bufferSize];

		do
		{
			// Read data from the standard output
			// standard output
			ssize_t stdcoutsize = ::read(pData.infd[0], buffer,    bufferSize - 1);
			ssize_t stdcerrsize = ::read(pData.errd[0], buffererr, bufferSize - 1);

			assert(stdcoutsize < bufferSize);
			assert(stdcerrsize < bufferSize);
			if (stdcoutsize > 0)
			{
				buffer[stdcoutsize] = '\0';
				if (hasStream)
					stream->onRead(AnyString(buffer, (uint)stdcoutsize));
				if (pRedirectToConsole)
					std::cout.write(buffer, stdcoutsize);
			}

			if (stdcerrsize > 0)
			{
				buffererr[stdcerrsize] = '\0';
				if (hasStream)
					stream->onErrorRead(AnyString(buffererr, (uint)stdcerrsize));
				if (pRedirectToConsole)
					std::cerr.write(buffererr, stdcerrsize);
				continue;
			}

			// direct call to `continue` if something has been read
			if (stdcoutsize > 0)
				continue;

			// Check PID status
			{
				int status;
				int wpid = waitpid(pData.pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
				if (wpid > 0)
				{
					if (WIFEXITED(status))
						pExitStatus = WEXITSTATUS(status);
					else if (WIFSIGNALED(status))
					{
						pExitStatus = -127;
						pKilled = true;
					}
					break;
				}
				else
				{
					if (wpid == -1) // the process is already dead
						break;
				}
			}
			suspend(50); // arbitrary
		}
		while (true);

		pEndTime = Yuni::DateTime::Now();

		// close all remaining fd
		int ca = close(pData.infd[0]);
		int cb = close(pData.errd[0]);
		int cc = close(pData.outfd[1]);
		if (ca or cb or cc)
			std::cerr << "close file descriptor failed\n";

		// release the buffer
		delete[] buffer;
		delete[] buffererr;
	}
	# endif




} // namespace Process
} // namespace Private
} // namespace Yuni





namespace Yuni
{

	Process::Process()
	{}


	Process::~Process()
	{
	}


	void Process::terminate()
	{
		#ifndef YUNI_OS_MSVC
		ProcessEnvironment::Ptr envptr = pEnv;
		if (!(!envptr))
			envptr->sendSignal(SIGTERM);
		#endif
	}


	void Process::kill()
	{
		#ifndef YUNI_OS_MSVC
		ProcessEnvironment::Ptr envptr = pEnv;
		if (!(!envptr))
			envptr->sendSignal(SIGKILL);
		#endif
	}


	bool Process::execute(uint timeout)
	{
		// new environment
		ProcessEnvironment::Ptr envptr = pEnv;
		if (!envptr)
		{
			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}
		ProcessEnvironment& env = *envptr;

		// The mutex will be unlocked by the new thread
		env.mutex.lock();
		if (env.running)
		{
			env.mutex.unlock();
			return false;
		}

		env.running      = true;
		env.processID    = 0;
		env.processInput = -1;
		env.exitstatus   = -128;
		env.timeout      = timeout;
		env.startTime    = 0;
		env.duration     = 0;

		if (env.executable.empty())
		{
			env.mutex.unlock();
			return true;
		}

		// starting a new thread
		// prepare commands
		Yuni::Private::Process::SubProcess* newthread = new Yuni::Private::Process::SubProcess(*this);
		newthread->command(env.executable, env.arguments);
		// keep somewhere
		env.thread = newthread;

		return newthread->launch();
	}


	int Process::wait(sint64* duration)
	{
		ProcessEnvironment::Ptr envptr = pEnv;
		if (!envptr)
			return 0;
		ProcessEnvironment& env = *envptr;

		// thread operation
		{
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
				thread = env.thread;
			}

			// wait for the end of the thread
			thread->wait();

			// since the thread has finished, we can safely destroy it to not let
			// a process alive for nothing
			env.thread = nullptr;
		}

		// results
		MutexLocker locker(env.mutex);
		if (duration)
			*duration = env.duration;
		return env.exitstatus;
	}















	class ExecutionHelper
	{
	public:
		//! The most suitable smart pointer for the class
		typedef SmartPtr<ExecutionHelper> Ptr;

	public:
		ExecutionHelper(Process process, Thread::Signal& signal, bool& result) :
			signal(signal),
			process(process),
			result(result)
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
		//! Process to execute (by copy thank to ref.counting)
		Process process;
		//!
		bool& result;

	private:
		# ifndef NDEBUG
		//! Flag to prevent against multiple launches (which must never happen)
		bool pAlreadyLaunched;
		# endif

	}; // class ExecutionHelper



	bool Process::dispatchExecution(const Bind<void (const Callback&)>& dispatcher, uint timeout)
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}

		Thread::Signal signal;
		if (not signal.valid())
		{
			std::cerr << "impossible to initialize signal\n";
			return false;
		}

		bool result = true;
		// Dispatch the message
		// The code is within a block to let the variable
		// `runner` destroyed, thus to notify the end of the execution
		// (via signal->notify())
		ExecutionHelper* runner = new ExecutionHelper(*this, signal, result);
		{
			Bind<bool ()>  callback;
			callback.bind(runner, &ExecutionHelper::perform, timeout);
			dispatcher(callback);
		}

		// waiting for the process startup from the main loop
		signal.wait();

		// we can now safely delete our helper
		delete runner;
		// ok ready
		return result;
	}


	bool Process::running() const
	{
		ProcessEnvironment::Ptr envptr = pEnv;
		return (!envptr) ? false : (envptr->running);
	}


	void Process::commandLine(const AnyString& cmd)
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}
		ProcessEnvironment& env = *envptr;

		MutexLocker locker(env.mutex);
		env.executable.clear();
		env.arguments.clear();
		bool foundExecutable = false;

		if (not cmd.empty())
		{
			uint offset = 0;
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


	void Process::workingDirectory(const AnyString& directory)
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}
		ProcessEnvironment& env = *envptr;

		MutexLocker locker(env.mutex);
		env.workingDirectory = directory;
	}


	String Process::workingDirectory() const
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return nullptr;
		ProcessEnvironment& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.workingDirectory;
	}


	bool Process::redirectToConsole() const
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return true;
		ProcessEnvironment& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.redirectToConsole;

	}


	void Process::redirectToConsole(bool flag)
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			if (flag)
				return; // default is true, useless to instanciate something

			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}
		ProcessEnvironment& env = *envptr;

		MutexLocker locker(env.mutex);
		env.redirectToConsole = flag;
	}


	Process::ProcessEnvironment::ProcessEnvironment() :
		running(false),
		timeout(0),
		redirectToConsole(true)
	{}



	String Process::program() const
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return nullptr;
		ProcessEnvironment& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.executable;
	}

	void Process::program(const AnyString& prgm)
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}
		ProcessEnvironment& env = *envptr;
		MutexLocker locker(env.mutex);
		env.executable = prgm;
	}


	void Process::argumentClear()
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
			return;
		ProcessEnvironment& env = *envptr;
		MutexLocker locker(env.mutex);
		return env.arguments.clear();
	}


	void Process::argumentAdd(const AnyString& arg)
	{
		ProcessEnvironment::Ptr envptr = pEnv; // keeping a reference to the current env
		if (!envptr)
		{
			envptr = new ProcessEnvironment();
			pEnv = envptr;
		}
		ProcessEnvironment& env = *envptr;
		MutexLocker locker(env.mutex);
		env.arguments.push_back(arg);
	}


	void Process::CaptureOutput::onRead(const AnyString& buffer)
	{
		cout += buffer;
	}


	void Process::CaptureOutput::onErrorRead(const AnyString& buffer)
	{
		cerr += buffer;
	}


	void Process::ProcessEnvironment::sendSignal(int sigvalue)
	{
		mutex.lock();
		if (0 == running)
		{
			mutex.unlock();
			return;
		}
		# ifndef YUNI_OS_WINDOWS
		const pid_t pid = static_cast<pid_t>(processID);
		mutex.unlock();
		if (pid)
			::kill(pid, sigvalue);
		# else
		// TODO missing implementation
		(void) sigvalue;
		mutex.unlock();
		# endif
	}






} // namespace Yuni

