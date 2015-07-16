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
#include "program.h"
#include "../../noncopyable.h"
#include "../../../thread/thread.h"
#ifdef YUNI_OS_UNIX
# include <sys/types.h>
#endif
#include <signal.h>




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
	class Program::ProcessSharedInfo final : public Yuni::NonCopyable<Program::ProcessSharedInfo>
	{
	public:
		//! Smart pointer
		// \note This type must match the definition of \p pEnv
		typedef SmartPtr<ProcessSharedInfo> Ptr;

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ProcessSharedInfo();
		//! Destructor
		~ProcessSharedInfo();
		//@}


		/*!
		** \return True if the signal has been delivered
		*/
		template<bool WithLock> bool sendSignal(int value);

		/*!
		** \brief Create a thread dedicated to handle the execution timeout
		*/
		void createThreadForTimeoutWL();


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

		//! Optional thread for timeout
		Yuni::Thread::IThread* timeoutThread;

	}; // class Program::ProcessSharedInfo









} // namespace Process
} // namespace Yuni

# include "process-info.hxx"

