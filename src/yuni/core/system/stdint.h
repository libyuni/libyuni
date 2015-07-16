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

/* !!! "C compatibility" header !!! */

/*!
** \brief Standard types used by the Yuni Library
*/

#ifdef YUNI_HAS_STDINT_H
#	include <stdint.h>
#else
#	ifdef YUNI_OS_MSVC
#		include "windows/msinttypes/stdint.h"
#		include "windows/msinttypes/inttypes.h"
#	else
#		error "stdint.h is missing for this platform"
#	endif
#endif

#ifdef YUNI_OS_MSVC
# define YUNI_MSVC_SECURE_VSPRINTF
#endif

#ifdef YUNI_HAS_SYS_TYPES_H
# include <sys/types.h>
#endif




#ifdef __cplusplus /* Only with a C++ Compiler */

#ifndef YUNI_OS_CLANG
# include <cstddef>
#endif




namespace Yuni
{

	/* \todo Fix support for int128 on Visual Studio 10 */
	# if defined(YUNI_HAS_INT128_T) && !defined(__DISABLED_SUPPORT_FOR_INT128)
	/* 128-bit unsigned int */
	typedef unsigned __int128  uint128;
	typedef __int128           sint128;
	# endif

	/*! 64-bit unsigned int, guaranteed to be 8 bytes in size */
	typedef uint64_t       uint64;
	/*! 64-bit unsigned int, guaranteed to be 8 bytes in size */
	typedef int64_t        sint64;
	/*! 32-bit unsigned int, guaranteed to be 4 bytes in size */
	typedef uint32_t       uint32;
	/*! 32-bit unsigned int, guaranteed to be 4 bytes in size */
	typedef int32_t        sint32;
	/*! 16-bit unsigned int, guaranteed to be 2 bytes in size */
	typedef uint16_t       uint16;
	/*! 16-bit unsigned int, guaranteed to be 2 bytes in size */
	typedef int16_t        sint16;
	/*! 8-bit unsigned int, guaranteed to be 1 byte in size */
	typedef unsigned char  uint8;
	/*! 8-bit unsigned int, guaranteed to be 1 byte in size */
	typedef char           sint8;

	/*! unsigned char */
	typedef unsigned char  uchar;

	/*!
	** \brief Constant int64 value to indicate that the size must be autodetected by the location
	** of the terminating null character
	*/
	static const uint64 AutoDetectNullChar = (uint64)(-1);

	# ifndef YUNI_HAS_SSIZE_T
	/*! ssize_t */
	typedef Yuni::sint64 ssize_t;
	# endif


	/*! Decimal floating-point (32 bits) */
	typedef float   float32;
	/*! Decimal floating-point (32 bits) */
	typedef double  float64;


} /* namespace Yuni */





# else /* Actually we have a C Compiler */

# include "stddef.h"

# endif /* C++ Compiler */


/* C types */

/* \todo Fix support for int128 on Visual Studio 10 */
# if defined(YUNI_HAS_INT128_T) && !defined(__DISABLED_SUPPORT_FOR_INT128)
/*! 128-bit unsigned int */
typedef unsigned __int128  yuint128;
typedef __int128           yint128;
# endif

/*! 64-bit unsigned int, guaranteed to be 8 bytes in size */
typedef uint64_t       yuint64;
/*! 64-bit unsigned int, guaranteed to be 8 bytes in size */
typedef int64_t        yint64;
/*! 32-bit unsigned int, guaranteed to be 4 bytes in size */
typedef uint32_t       yuint32;
/*! 32-bit unsigned int, guaranteed to be 4 bytes in size */
typedef int32_t        yint32;
/*! 16-bit unsigned int, guaranteed to be 2 bytes in size */
typedef uint16_t       yuint16;
/*! 16-bit unsigned int, guaranteed to be 2 bytes in size */
typedef int16_t        yint16;
/*! 8-bit unsigned int, guaranteed to be 1 byte in size */
typedef unsigned char  yuint8;
/*! 8-bit unsigned int, guaranteed to be 1 byte in size */
typedef char           yint8;

/*! Decimal floating-point (32 bits) */
typedef float          yfloat32;
/*! Decimal floating-point (32 bits) */
typedef double         yfloat64;




/* Both C / C++ compiler */

# ifndef YUNI_HAS_UINT
/*! Convenient typedef around unsigned int */
typedef unsigned int  uint;
# endif


#ifdef YUNI_OS_CLANG
/* Try to fix issues with clang which does not yet support __float128, and produces
** compilation error in the STL */
typedef struct { double x, y; }  __float128;
#endif

