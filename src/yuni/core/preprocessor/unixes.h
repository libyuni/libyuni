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

/*
** Information about Unix capabilities
*/

/* Unix compliant */
#define YUNI_OS_UNIX

#undef  YUNI_OS_FLAG_UNIX
#define YUNI_OS_FLAG_UNIX 1


#if defined(__HAIKU) || defined(__HAIKU__) || defined(_HAIKU)
/* Haiku */
#	define YUNI_OS_HAIKU
#	define YUNI_OS_NAME "Haiku"
#else
/* BeOS */
#	if defined(__BeOS) || defined(__BEOS__) || defined(_BEOS)
#		define YUNI_OS_BEOS
#		define YUNI_OS_NAME "BeOS"
#	endif
#endif



/* Linux */
#if defined(__linux) || defined(linux) || defined(__linux__)
#	define YUNI_OS_LINUX
#	define YUNI_OS_NAME "GNU/Linux"
#	undef  YUNI_OS_FLAG_LINUX
#	define YUNI_OS_FLAG_LINUX 1
#else

/* AIX */
#	if defined(_AIX)
#		define YUNI_OS_AIX
#		define YUNI_OS_NAME "AIX"
#	endif

/* DragonFly */
#	if defined(__DragonFly__)
#		define YUNI_OS_DRAGONFLY
#		define YUNI_OS_NAME "DragonFly"
#	endif

/* HP-UX */
#	if defined(_hpux) || defined(__hpux) || defined(__hpux__)
#		define YUNI_OS_HPUX
#		define YUNI_OS_NAME "HP-UX"
#	endif

/* Mac OS */
#	if defined(__APPLE__) || defined(__MACH__)
#		ifdef YUNI_OS_FLAG_LINUX
#			undef  YUNI_OS_FLAG_LINUX
#			define  YUNI_OS_FLAG_LINUX 0
#		endif
#		ifdef YUNI_OS_LINUX
#			undef  YUNI_OS_LINUX // we may have those macro already defined
#		endif
#		define YUNI_OS_MAC
#		define YUNI_OS_MACOS
#		define YUNI_OS_DARWIN
#		define YUNI_OS_NAME "MacOS X"
#		undef  YUNI_OS_FLAG_MACOS
#		define YUNI_OS_FLAG_MACOS 1
#	else
/* FreeBSD */
#		if defined(__FreeBSD__)
#			define YUNI_OS_FREEBSD __FreeBSD__
#			define YUNI_OS_NAME "FreeBSD"
#		endif
#	endif

/* NetBSD */
#	if defined(__NetBSD__)
#		define YUNI_OS_NETBSD
#		define YUNI_OS_NAME "NetBSD"
#	endif

/* OpenBSD */
#	if defined(__OpenBSD__)
#		define YUNI_OS_OPENBSD
#		define YUNI_OS_NAME "OpenBSD"
#	endif
#	if defined(sun) || defined(__sun)

/* Solaris */
#		if defined(__SVR4) || defined(__svr4__)
#			 define YUNI_OS_SOLARIS
#			define YUNI_OS_NAME "Solaris"
#		else
/* SunOS */
#			define YUNI_OS_SUNOS
#			define YUNI_OS_NAME "SunOS"
#		endif
#		if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#			define YUNI_OS_SUNSTUDIO
#			define YUNI_COMPILER_NAME "SunCC"
#		endif
#	endif /* Sun */
/* Lynx */
#	ifdef __Lynx__
#		define YUNI_OS_LYNX
#		define YUNI_OS_NAME "Lynx"
#	endif
/* Cygwin */
#	ifdef __CYGWIN__
#	   define YUNI_OS_CYGWIN
#	   define YUNI_OS_NAME "Windows Cygwin"
#	endif


#endif /* Linux */


#if defined(VMS) || defined(__VMS)
#	define YUNI_OS_VMS
#   define YUNI_OS_NAME "VMS"
#endif
