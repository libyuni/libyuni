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

/* This header must remain compatible with C compilers */


#ifdef YUNI_OS_WINDOWS
#	ifdef YUNI_OS_MSVC
#		pragma warning(push)
#		pragma warning(disable : 4995)
#		pragma warning(push, 0)
#	endif
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN 1
#	endif
#	ifndef _WIN32_WINNT /* currently mingw does not define, mingw64 does */
#		define _WIN32_WINNT 0x0500 /* At least windows 2000 */
#	endif
#	include <winsock2.h>
#	include <windows.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <time.h>
#	define __PRETTY_FUNCTION__ __FUNCDNAME__
#	ifdef YUNI_OS_MSVC
#		pragma warning(pop)
#	endif
#endif


/* On some compiler, the macro min() and max() are defined... (Visual Studio for example...) */
# ifdef min
#   undef min
# endif
# ifdef max
#   undef max
# endif





#ifdef YUNI_OS_WINDOWS
namespace Yuni
{
namespace Windows
{

	inline yint64 FILETIMEToTimestamp(const FILETIME& filetime)
	{
		LARGE_INTEGER date, adjust;
		date.HighPart = filetime.dwHighDateTime;
		date.LowPart = filetime.dwLowDateTime;

		// 100-nanoseconds = milliseconds * 10000
		adjust.QuadPart = 11644473600000 * 10000;

		// removes the diff between 1970 and 1601
		date.QuadPart -= adjust.QuadPart;

		// converts back from 100-nanoseconds to seconds
		return date.QuadPart / 10000000;
	}





} // namespace Windows
} // namespace Yuni
#endif
