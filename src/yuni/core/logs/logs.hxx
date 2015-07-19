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



namespace Yuni
{
namespace Logs
{


	template<class Handlers, class Decorators, template<class> class TP>
	inline Logger<Handlers,Decorators,TP>::Logger() :
		verbosityLevel(Logger<Handlers,Decorators,TP>::defaultVerbosityLevel)
	{}


	template<class Handlers, class Decorators, template<class> class TP>
	inline Logger<Handlers,Decorators,TP>::~Logger()
	{}


	template<class Handlers, class Decorators, template<class> class TP>
	template<class VerbosityType>
	inline void
	Logger<Handlers,Decorators,TP>::dispatchMessageToHandlers(const AnyString& message) const
	{
		// Locking the operation, according to the threading policy
		typename ThreadingPolicy::MutexLocker locker(*this);

		// Filtering the verbosity level
		// 'verbosityLevel' is a public variable
		if (VerbosityType::level <= verbosityLevel)
		{
			Logger<Handlers,Decorators,TP>&	self = const_cast<Logger<Handlers,Decorators,TP>&>(*this);
			// Ask to all handlers to internalDecoratorWriteWL the message
			Handlers::template
				internalDecoratorWriteWL<LoggerType,VerbosityType>(self, message);
		}
	}


	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::NoticeBuffer
	Logger<Handlers,Decorators,TP>::notice() const
	{
		return NoticeBuffer(*this);
	}

	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::InfoBuffer
	Logger<Handlers,Decorators,TP>::info() const
	{
		return InfoBuffer(*this);
	}

	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::CompatibilityBuffer
	Logger<Handlers,Decorators,TP>::compatibility() const
	{
		return CompatibilityBuffer(*this);
	}



	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::CheckpointBuffer
	Logger<Handlers,Decorators,TP>::checkpoint() const
	{
		return typename Logger<Handlers,Decorators,TP>::CheckpointBuffer(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::WarningBuffer
	Logger<Handlers,Decorators,TP>::warning() const
	{
		return WarningBuffer(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::ErrorBuffer
	Logger<Handlers,Decorators,TP>::error() const
	{
		return ErrorBuffer(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::ProgressBuffer
	Logger<Handlers,Decorators,TP>::progress() const
	{
		return ProgressBuffer(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::FatalBuffer
	Logger<Handlers,Decorators,TP>::fatal() const
	{
		return FatalBuffer(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	inline typename Logger<Handlers,Decorators,TP>::DebugBuffer
	Logger<Handlers,Decorators,TP>::debug() const
	{
		return DebugBuffer(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::NoticeBuffer
	Logger<Handlers,Decorators,TP>::notice(const U& u) const
	{
		return NoticeBuffer(*this, u);
	}

	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::InfoBuffer
	Logger<Handlers,Decorators,TP>::info(const U& u) const
	{
		return InfoBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::CheckpointBuffer
	Logger<Handlers,Decorators,TP>::checkpoint(const U& u) const
	{
		return CheckpointBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::WarningBuffer
	Logger<Handlers,Decorators,TP>::warning(const U& u) const
	{
		return WarningBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::ErrorBuffer
	Logger<Handlers,Decorators,TP>::error(const U& u) const
	{
		return ErrorBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::ProgressBuffer
	Logger<Handlers,Decorators,TP>::progress(const U& u) const
	{
		return ProgressBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::CompatibilityBuffer
	Logger<Handlers,Decorators,TP>::compatibility(const U& u) const
	{
		return CompatibilityBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::FatalBuffer
	Logger<Handlers,Decorators,TP>::fatal(const U& u) const
	{
		return FatalBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::DebugBuffer
	Logger<Handlers,Decorators,TP>::debug(const U& u) const
	{
		return DebugBuffer(*this, u);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<class C>
	inline Private::LogImpl::Buffer<Logger<Handlers,Decorators,TP>, C, C::enabled>
	Logger<Handlers,Decorators,TP>::custom() const
	{
		return Private::LogImpl::Buffer<LoggerType, C, C::enabled>(*this);
	}


	template<class Handlers, class Decorators, template<class> class TP>
	template<typename U>
	inline typename Logger<Handlers,Decorators,TP>::UnknownBuffer
	Logger<Handlers,Decorators,TP>::operator << (const U& u) const
	{
		return UnknownBuffer(*this, u);
	}





} // namespace Logs
} // namespace Yuni
