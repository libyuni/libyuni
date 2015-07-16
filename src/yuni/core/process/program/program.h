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
#include "../../../yuni.h"
#include "../../../core/string.h"
#include "../../atomic/int.h"
#include "../../bind.h"
#include "stream.h"



namespace Yuni
{
namespace Process
{


	/*!
	** \brief Execute a command and wait for it
	**
	** \param commandline An arbitrary command line (example: "ls -l")
	** \param timeout Maximum execution time allowed for the command (in seconds - 0 means infinite)
	** \return True if the command has been executed and if the exit status is equal to 0
	*/
	bool Execute(const AnyString& commandline, uint timeout = 0u);


	/*!
	** \brief Execute a command into a given loop and wait for it
	**
	** \param mainloop A mainloop from where the program should be launched
	** \param commandline An arbitrary command line (example: "ls -l")
	** \param timeout Maximum execution time allowed for the command (in seconds - 0 means infinite)
	** \return True if the command has been executed and if the exit status is equal to 0
	*/
	template<class MainLoopT>
	bool Execute(MainLoopT& mainloop, const AnyString& commandline, uint timeout = 0u);







	/*!
	** \brief Start external programs and communicate with them
	**
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/core/process/program.h>
	**
	** int main()
	** {
	**	Process::Program process;
	**	process.program("ls");
	**	process.argumentAdd("-l");
	**	process.argumentAdd("/tmp");
	**	process.execute();
	**	return process.wait();
	** }
	** \endcode
	**
	** By default, the output of the underlying process is not redirected to the
	** current console, even if a stream handler is provided. `redirectToConsole()` can be
	** use to modify this behavior.
	**
	** This class (and all its public methods) is thread-safe.
	**
	** \internal The 'Stream' object may be shared and reused
	** \internal `std::cout` and `std::cerr` should be used instead of `write` when redirecting
	**  the outputs in order to share the same buffer and to have cleaner output
	*/
	class YUNI_DECL Program final
	{
	public:
		//! Callback for main loop
		typedef Bind<bool ()>  Callback;

		//! Precision used for calculating the time spent during the execution of the underlying process
		enum DurationPrecision
		{
			//! In seconds (the default)
			dpSeconds,
			//! In milliseconds
			dpMilliseconds,
			//! None (disabled, the duration will always be equal to zero)
			dpNone,
		};


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Program();
		//! Destructor
		~Program();
		//@}

		//! \name Options & Arguments
		//@{
		/*!
		** \brief Reset all arguments from a complete command line (ex: `ls -l /somewhere`)
		*/
		void commandLine(const AnyString& cmd);

		//! Get the program that will be executed
		String program() const;
		//! Set the program that will be executed
		void program(const AnyString& prgm);

		//! Remove all arguments
		void argumentClear();
		//! Add a new argument for the program to execute
		void argumentAdd(const AnyString& arg);

		/*!
		** \brief Set the working directory when executing the process
		**
		** Empty will use the current directory.
		*/
		void workingDirectory(const AnyString& directory);
		//! Get the working directory that will be used for executing the process
		String workingDirectory() const;
		//@}


		//! \name Process Execution
		//@{
		/*!
		** \brief Execute the process
		**
		** Try to execute the process from the current thread.
		**
		** \param timeout Maximum execution time allowed for the command (in seconds - 0 means infinite)
		** \return True if the command has been launched
		*/
		bool execute(uint timeout = 0u);

		/*!
		** \brief Execute the command with the cooperation of a main loop
		**
		** The execution of the process will be done from the main thread
		** (actually the thread associated to the main loop, which should be
		** the main thread). Then you can wait for your process from any thread
		** with the method `wait()`
		**
		** \param timeout Maximum execution time allowed for the command (in seconds - 0 means infinite)
		** \return True if the command has been launched
		*/
		template<class MainLoopT> bool execute(MainLoopT& mainloop, uint timeout = 0u);
		//@}


		//! \name Process control
		//@{
		/*!
		** \brief Wait for the end of the sub-process
		**
		** This method can be called from any thread.
		**
		** \param[out] duration An output variable for storing the execution time
		**    (in seconds or milliseconds, according the precision required) [optional]
		** \return The exit status of the process
		** \see durationPrecision()
		*/
		int wait(sint64* duration = nullptr);

		/*!
		** \brief Terminate the execution of the process
		**
		** This is the prefered way to terminate a process. On Unix, SIGTERM
		** will be sent, which can be intercepted.
		*/
		void terminate();

		/*!
		** \brief Kill the process (cause immediate program termination)
		**
		** The method `terminate()` should be prefered for simply stopping a process.
		*/
		void kill();


		/*!
		** \brief Get if the process is currently running
		**
		** The returned value is only a hint, since the status of the sub-process
		** may change anytime.
		** \see Program::wait()
		*/
		bool running() const;


		/*!
		** \brief Send a signal to the sub-process (UNIX only)
		**
		** \note This routine has no effect on systems where signals are not supported
		** \see man 3 signal
		** \see Program::terminate()
		** \param sig The signal to send (SIGINT, SIGTERM, ...)
		*/
		void signal(int sig);
		//@}


		//! \name Stream - capturing output
		//@{
		//! Get if stdcout/stderr are redirected to the console (default: false)
		bool redirectToConsole() const;
		//! Set if stdcout/stderr are redirected to the console, even if a stream handler is provided
		// (only if the process is not running)
		void redirectToConsole(bool flag);

		//! Set the stream handler used for capturing events
		void stream(Stream::Ptr newstream);
		//! Get the stream used for capturing events
		Stream::Ptr stream() const;
		//@}


		//! \name Precision used for calculating the duration of the execution of the process
		//@{
		//! Get the precision currently in use
		DurationPrecision  durationPrecision() const;
		//! Set the precision used for calculating a duration (only if the process is not running)
		void durationPrecision(DurationPrecision precision);
		//@}


	private:
		//! Dispatch the execution of the process to an event loop
		bool dispatchExecution(const Bind<void (const Callback&)>& dispatcher, uint timeout);

	private:
		// forward declaration
		class ProcessSharedInfo;
		class ThreadMonitor;
		//! Alias to the thread monitor's smartptr
		typedef SmartPtr<Yuni::Process::Program::ThreadMonitor, Yuni::Policy::Ownership::COMReferenceCounted>  ThreadPtr;

		//! Information on the program currently executed
		// \note This class may be shared by several threads
		SmartPtr<ProcessSharedInfo> pEnv;
		//! Stream
		Stream::Ptr pStream;

	}; // class Program







} // namespace Process
} // namespace Yuni

# include "program.hxx"

