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
#include "windows.hdr.h"



#define YUNI_HAS_GETTIMEOFDAY

#ifndef YUNI_OS_MSVC
# include <sys/time.h>
#else // YUNI_OS_MSVC

#	ifdef YUNI_OS_WINDOWS
#		include <time.h>
#		if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#			define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#		else
#			define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#		endif
#	endif

#	ifdef YUNI_HAS_GETTIMEOFDAY
#		undef YUNI_HAS_GETTIMEOFDAY
#	endif

#endif // YUNI_OS_MSVC



namespace Yuni
{

	# ifndef YUNI_HAS_GETTIMEOFDAY

	struct timezone
	{
		int  tz_minuteswest; // minutes W of Greenwich
		int  tz_dsttime;	 // type of dst correction
	};

	struct timeval
	{
		sint64 tv_sec;
		sint64 tv_usec;
	};

	YUNI_DECL int gettimeofday(struct timeval *tv, struct timezone *tz);

	# define YUNI_SYSTEM_GETTIMEOFDAY  ::Yuni::gettimeofday

	# else

	typedef struct timezone timezone;
	typedef struct timeval timeval;
	#	ifndef YUNI_SYSTEM_GETTIMEOFDAY
	#		define YUNI_SYSTEM_GETTIMEOFDAY  ::gettimeofday
	#	endif

	# endif

} // namespace Yuni

