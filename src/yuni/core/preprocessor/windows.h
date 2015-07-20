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


/* Microsoft Windows */
#ifndef YUNI_OS_WINDOWS
#	define YUNI_OS_WINDOWS
#endif // YUNI_OS_WINDOWS
#define YUNI_OS_NAME "Microsoft Windows"

#undef  YUNI_OS_FLAG_WINDOWS
#define YUNI_OS_FLAG_WINDOWS 1


/* Windows 64 */
#ifdef _WIN64
#	define YUNI_OS_WIN64
#	define YUNI_OS_64
#else
#	define YUNI_OS_WIN32
#endif

/* Windows CE */
#ifdef _WIN32_WCE
#	define YUNI_OS_WINCE
#endif

/* Visual Studio */
#ifdef _MSC_VER
#	define YUNI_OS_MSVC
#	define YUNI_MSVC
#	define YUNI_COMPILER_NAME "Microsoft Visual C++"
#endif

/* Borland Compiler */
#ifdef __BORLANDC__
#	define YUNI_OS_BORLAND
#	define YUNI_COMPILER_NAME "Borland C++ Compiler"
#endif

/* MinGW */
#if defined(__MINGW32__) || defined(__MINGW64__)
#	define YUNI_OS_MINGW
#	undef YUNI_OS_32
#	undef YUNI_OS_64
#	ifdef __MINGW64__
#		define YUNI_OS_MINGW64
#		define YUNI_OS_64
#		define YUNI_COMPILER_NAME "MinGW64"
#	else
#		define YUNI_OS_MINGW32
#		define YUNI_OS_32
#		define YUNI_COMPILER_NAME "MinGW32"
#	endif
#endif
