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
#include "../../yuni.h"
#include "../../thread/policy.h"
#include "null.h"
#include "verbosity.h"

// Default Handler
#include "handler/stdcout.h"
#include "handler/file.h"
// Default decorators
#include "decorators/verbositylevel.h"
#include "decorators/time.h"
#include "decorators/message.h"
#include "../noncopyable.h"
#include "buffer.h"


// The default verbosity level according the target mode (debug/release)
#ifdef NDEBUG
# define YUNI_LOGS_DEFAULT_VERBOSITY  Verbosity::Compatibility::level
#else
# define YUNI_LOGS_DEFAULT_VERBOSITY  Verbosity::Debug::level
#endif


/*!
** \brief Convenient macro for debugging
**
** \code
** logs.error() << YN_LOGENV << "my message";
** \endcode
*/
#define YN_LOGENV  __FILE__ << ':' << __LINE__ << ": "




namespace Yuni
{
namespace Logs
{

	/*!
	** \brief A customizable log facility
	**
	** A simple hello world :
	** \code
	** #include <yuni/core/logs.h>
	**
	** int main()
	** {
	**	// The logger
	**	Yuni::Logs::Logger<>  logs;
	**	// Hello world !
	**	logs.notice() << "Hello world !";
	**	return 0;
	** }
	** \endcode
	**
	** This class uses a static decorator pattern to modify its behavior. By default,
	** the output has the following format :
	** \code
	** [date][color][verbosity level][/color] <msg>
	** \endcode
	** The color output from the shell is available for both Unix (Terminal) and
	** Windows (cmd.exe).
	**
	**
	** \tparam TP The Threading Policy
	** \tparam Handlers A static list of all message handlers
	** \tparam Decorators A static list of all decorators
	*/
	template<
		class Handlers = StdCout<>,                             // List of all static handles
		class Decorators = Time< VerbosityLevel<Message<> > >,  // List of all static decorators
		template<class> class TP = Policy::ObjectLevelLockableNotRecursive // The Threading Policy
		>
	class YUNI_DECL Logger final :
		public TP<Logger<Handlers,Decorators,TP> >,             // inherits from the Threading Policy
		public Decorators,                                      // inherits from all decorators
		public Handlers,                                        // inherits from all handlers
		private NonCopyable<Logger<Handlers, Decorators, TP> >  // noncopyable
	{
	public:
		//! The full prototype of the logger
		typedef Logger<Handlers, Decorators, TP>  LoggerType;
		//! The Threading Policy
		typedef TP<LoggerType>  ThreadingPolicy;

		//! Handlers
		typedef Handlers HandlersType;
		//! Decorators
		typedef Decorators  DecoratorsType;

		/*!
		** \brief Settings for the logger
		*/
		enum Settings
		{
			//! The default verbose level
			defaultVerbosityLevel = YUNI_LOGS_DEFAULT_VERBOSITY,
		};

	private:
		// Aliases (for code clarity)
		//! Alias for the CheckpointBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Checkpoint>    CheckpointBuffer;
		//! Alias for the NoticeBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Notice>        NoticeBuffer;
		//! Alias for the NoticeBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Info>          InfoBuffer;
		//! Alias for the WarningBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Warning>       WarningBuffer;
		//! Alias for the ErrorBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Error>         ErrorBuffer;
		//! Alias for the ProgressBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Progress>      ProgressBuffer;
		//! Alias for the CompatibilityBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Compatibility> CompatibilityBuffer;
		//! Alias for the FatalBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Fatal>         FatalBuffer;
		//! Alias for the DebugBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Debug>         DebugBuffer;
		//! Alias for a dummy writer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Info, 0>       DummyBuffer;
		//! Alias for the UnknownBuffer
		typedef Private::LogImpl::Buffer<LoggerType, Verbosity::Unknown>       UnknownBuffer;

	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Default Constructor
		*/
		Logger();
		/*!
		** \brief Destructor
		*/
		~Logger();
		//@}


		//! \name Checkpoint
		//@{
		CheckpointBuffer checkpoint() const;
		template<class U> CheckpointBuffer checkpoint(const U& u) const;
		//@}

		//! \name Notice
		//@{
		NoticeBuffer notice() const;
		template<class U> NoticeBuffer notice(const U& u) const;
		//@}

		//! \name Info
		//@{
		InfoBuffer info() const;
		template<class U> InfoBuffer info(const U& u) const;
		//@}

		//! \name Warning
		//@{
		WarningBuffer warning() const;
		template<class U> WarningBuffer warning(const U& u) const;
		//@}

		//! \name Error
		//@{
		ErrorBuffer error() const;
		template<class U> ErrorBuffer error(const U& u) const;
		//@}

		//! \name Progress
		//@{
		ProgressBuffer progress() const;
		template<class U> ProgressBuffer progress(const U& u) const;
		//@}

		//! \name Fatal
		//@{
		FatalBuffer fatal() const;
		template<class U> FatalBuffer fatal(const U& u) const;
		//@}

		//! \name Compatibility notice
		//@{
		CompatibilityBuffer compatibility() const;
		template<class U> CompatibilityBuffer compatibility(const U& u) const;
		//@}

		//! \name Debug (disabled if NDEBUG defined)
		//@{
		DebugBuffer debug() const;
		template<class U> DebugBuffer debug(const U& u) const;
		//@}


		//! Start a custom verbosity level message
		template<class C> Private::LogImpl::Buffer<LoggerType,C,C::enabled> custom() const;

		//! Start a message with no verbosity level (always displayed)
		template<class U> UnknownBuffer operator << (const U& u) const;
		//@}


	public:
		/*!
		** \brief The current maximum verbosity level allowed to be displayed
		**
		** \code
		** Logs::Logger<> logs;
		**
		** // Starting with default verbosity level
		** // The following messages will be displayed
		** logs.error() << "An error";
		** logs.notice() << "Hello world";
		**
		** // Changing the verbosity level
		** logs.verbosityLevel = Logs::Verbosity::Error::level;
		** // Only the 'error' message will be displayed
		** logs.error() << "An error";
		** logs.notice() << "Hello world";
		** \endcode
		*/
		int verbosityLevel;

	private:
		/*!
		** \brief Transmit a message to all handlers
		*/
		template<class VerbosityType>
		void dispatchMessageToHandlers(const AnyString& message) const;

		// A friend !
		template<class, class, int> friend class Private::LogImpl::Buffer;

	}; // class Logger





} // namespace Logs
} // namespace Yuni

#include "logs.hxx"

