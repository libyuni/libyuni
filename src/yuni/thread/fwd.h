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
#ifndef YUNI_NO_THREAD_SAFE
#	ifdef YUNI_OS_WINDOWS
#		include "../core/system/windows.hdr.h"
#		define YUNI_THREAD_FNC_RETURN   DWORD WINAPI
#	else
#		define YUNI_THREAD_FNC_RETURN   void*
#	endif
#endif


namespace Yuni
{
namespace Thread
{

	//! Return error status
	enum Error
	{
		//! No error, the operation succeeded
		errNone = 0,
		//! A timeout occured
		errTimeout,
		//! Impossible to create the new thread
		errThreadCreation,
		//! The onStarting handler returned false.
		errAborted,
		//! The operation failed for an unkown reason
		errUnkown
	};


	enum
	{
		//! The default timeout for stopping a thread
		defaultTimeout = 5000, // 5 seconds
	};




} // namespace Thread
} // namespace Yuni





namespace Yuni
{
namespace Job
{

	// forward declaration
	class YUNI_DECL IJob;

} // namespace Job
} // namespace Yuni






#ifndef YUNI_NO_THREAD_SAFE

namespace Yuni
{
namespace Private
{
namespace Thread
{

	/*!
	** \brief This procedure will be run in a separate thread and will run IThread::onExecute()
	*/
	extern "C"  YUNI_THREAD_FNC_RETURN  threadCallbackExecute(void* arg);



} // namespace Yuni
} // namespace Private
} // namespace Thread

#endif // YUNI_NO_THREAD_SAFE
