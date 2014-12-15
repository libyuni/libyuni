#ifndef __YUNI_CORE_PROCESS_PROGRAM_PROCESSINFO_H__
# define __YUNI_CORE_PROCESS_PROGRAM_PROCESSINFO_H__

# include "program.h"



namespace Yuni
{
namespace Process
{




	/*!
	** \brief Helper class for storing shared information on the program which is currently
	** launched by Process::Program
	**
	** \note This class may be shared by several threads
	*/
	class Program::ProcessSharedInfo final
	{
	public:
		//! Smart pointer
		// \note This type must match the definition of \p pEnv
		typedef SmartPtr<ProcessSharedInfo> Ptr;

	public:
		ProcessSharedInfo()
			: running(false)
			, processID(-1)
			, processInput(-1)
			, duration(0)
			, durationPrecision(dpSeconds)
			, timeout()
			, exitstatus(-1)
			, redirectToConsole(true)
		{}

		~ProcessSharedInfo() {}


		/*!
		** \return True if the signal has been delivered
		*/
		template<bool WithLock> bool sendSignal(int value);



	public:
		String executable;
		//! The command
		String::Vector arguments;
		//! The working directory
		String workingDirectory;
		//! Flag to know if the process is running
		bool running;
		//! PID
		int processID;
		//! input file descriptors
		int processInput;
		//! Thread
		ThreadPtr thread;
		//! Duration in seconds
		sint64 duration;
		//! Duration precision
		DurationPrecision durationPrecision;
		//! Timeout
		uint timeout;
		//! Exit status
		int exitstatus;
		//! Console
		bool redirectToConsole;
		//! Mutex
		mutable Mutex mutex;

	}; // class Program::ProcessSharedInfo






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
			# warning missing implementation on windows
			switch (sigvalue)
			{
				case SIGKILL:
					break;
			}
			(void) sigvalue;

			if (WithLock)
				mutex.unlock();
		}
		# endif

		return false;
	}







} // namespace Process
} // namespace Yuni

#endif // __YUNI_CORE_PROCESS_PROGRAM_PROCESSINFO_H__
