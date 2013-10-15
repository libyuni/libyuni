#ifndef __YUNI_CORE_PROCESS_PROCESS_H__
# define __YUNI_CORE_PROCESS_PROCESS_H__

# include "../../yuni.h"
# include "../../core/string.h"
# include "../atomic/int.h"
# include "fwd.h"
# include "../bind.h"


namespace Yuni
{

	/*!
	** \brief Class to run external programs
	**
	** \code
	** Process process;
	** process.program("ls");
	** process.argumentAdd("-l");
	** process.argumentAdd("/tmp");
	** process.execute();
	** process.wait();
	*/
	class Process final
	{
	public:
		/*!
		** \brief Directly execute a command and wait for the execution
		*/
		template<class MainLoopT>
		static bool Execute(MainLoopT& mainloop, const AnyString& commandline);

	public:
		//! Callback for main loop
		typedef Bind<bool ()>  Callback;

		/*!
		** \brief Process Stream
		**
		** The lifetime of a process stream is guarantee to be at least
		** the whole execution of the underlying process
		*/
		class Stream
		{
		public:
			typedef SmartPtr<Stream> Ptr;

		public:
			Stream() {}
			virtual ~Stream() {}

			//! Some data from the standard output are ready
			virtual void onRead(const AnyString& /*buffer*/) {}

			//! Some data from the error output are ready
			virtual void onErrorRead(const AnyString& /*buffer*/) {}

			//! The execution has finished
			virtual void onStop(bool /*killed*/, int /*exitstatus*/, sint64 /*duration*/) {}
		};

		class CaptureOutput : public Process::Stream
		{
		public:
			typedef SmartPtr<CaptureOutput>  Ptr;

		public:
			CaptureOutput() {}
			virtual ~CaptureOutput() {}

			//! Some data from the standard output are ready
			virtual void onRead(const AnyString& buffer) override;

			//! Some data from the error output are ready
			virtual void onErrorRead(const AnyString& buffer) override;

		public:
			Clob cout;
			Clob cerr;
		};


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Process();
		//! Destructor
		~Process();
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
		** \param A variable for storing the execution time (in seconds)
		** \return The exit status of the process
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


		//! Get if the process is currently running
		bool running() const;


		//! \name Stream - capturing output
		//@{
		//! Get if stdcout/stderr are redirected to the console (default: true)
		bool redirectToConsole() const;
		//! Set if stdcout/stderr are redirected to the console
		void redirectToConsole(bool flag);

		//! Set the stream used for capturing events
		void stream(Stream::Ptr newstream);
		//! Get the stream used for capturing events
		Stream::Ptr stream() const;
		//@}


	private:
		typedef SmartPtr<Yuni::Private::Process::SubProcess>  ThreadPtr;

	private:
		//! Dispatch the execution of the process to an event loop
		bool dispatchExecution(const Bind<void (const Callback&)>& dispatcher, uint timeout);

	private:
		class ProcessEnvironment final
		{
		public:
			//! Smart pointer
			typedef SmartPtr<ProcessEnvironment> Ptr;

		public:
			ProcessEnvironment();

			void sendSignal(int value);

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
			//! Start time
			sint64 startTime;
			//! Duration in seconds
			sint64 duration;
			//! Timeout
			uint timeout;
			//! Exit status
			int exitstatus;
			//! Console
			bool redirectToConsole;
			//! Mutex
			mutable Mutex mutex;
		};
		ProcessEnvironment::Ptr pEnv;

		//! Stream
		Stream::Ptr pStream;

		// friend !
		friend class Yuni::Private::Process::SubProcess;

	}; // class Process





} // namespace Yuni

# include "process.hxx"

#endif // __YUNI_CORE_PROCESS_PROCESS_H__
