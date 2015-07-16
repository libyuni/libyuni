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
#include "../string.h"



namespace Yuni
{
namespace Private
{
namespace LogImpl
{

	// Forward declaration
	template<class LogT, class V, int E = V::enabled> class Buffer;




	/*!
	** \brief The buffer for the message
	**
	** \internal This is an intermediate class that handles a temporary buffer where
	** the message can be built. The message will be dispatched to the static list
	** of handlers when this class is destroyed. The method `internalFlush()` is called
	** , which ensures thread-safety (if required) while the message is passing through
	** the handlers.
	**
	** \tparam V The verbosity level of the message
	** \tparam E A non-zero value if the message must be managed
	*/
	template<class LogT, class V, int E>
	class YUNI_DECL Buffer final
	{
	public:
		//! Type of the calling logger
		typedef LogT LoggerType;

	public:
		//! \name Constructos & Destructor
		//@{
		inline Buffer(const LoggerType& l) :
			pLogger(l)
		{}

		template<typename U>
		inline Buffer(const LoggerType& l, U u) :
			pLogger(l)
		{
			pBuffer.append(u);
		}

		~Buffer()
		{
			// Dispatching the messages to the handlers
			// For example, the buffer will be written to the output
			pLogger.template dispatchMessageToHandlers<V>(pBuffer);
		}
		//@}

		template<typename U> Buffer& operator << (const U& u)
		{
			// Appending the piece of message to the buffer
			pBuffer.append(u);
			return *this;
		}

		void appendFormat(const char f[], ...)
		{
			va_list parg;
			va_start(parg, f);
			pBuffer.vappendFormat(f, parg);
			va_end(parg);
		}

		void vappendFormat(const char f[], va_list parg)
		{
			pBuffer.vappendFormat(f, parg);
		}


	private:
		//! Reference to the original logger
		const LoggerType& pLogger;

		/*!
		** \brief Buffer that contains the message
		**
		** The chunk size can not be merely the default one; Log entries often
		** contain path of filename for example.
		*/
		Yuni::CString<1024> pBuffer;

	}; // class Buffer






	// Specialization when a verbosty level is disabled
	template<class LogT, class V>
	class YUNI_DECL Buffer<LogT, V, 0> final
	{
	public:
		//! Type of the calling logger
		typedef LogT LoggerType;

	public:
		Buffer(const LoggerType&) {}

		template<typename U> Buffer(const LoggerType&, U) {}

		~Buffer()
		{}

		template<typename U> const Buffer& operator << (const U&) const
		{
			// Do nothing - Disabled
			return *this;
		}

		void appendFormat(const char [], ...)
		{
			// Do nothing
		}

		void vappendFormat(const char [], va_list)
		{
			// Do nothing
		}

	}; // class Buffer





} // namespace LogImpl
} // namespace Private
} // namespace Yuni

