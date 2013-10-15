#ifndef __YUNI_SYSTEM_WINDOWS_HEADERS_H__
# define __YUNI_SYSTEM_WINDOWS_HEADERS_H__

# include "../../yuni.h"

/* This header must remain compatible with C compilers */


# ifdef YUNI_OS_WINDOWS
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
# endif


/* On some compiler, the macro min() and max() are defined... (Visual Studio for example...) */
# ifdef min
#   undef min
# endif
# ifdef max
#   undef max
# endif


#endif /* __YUNI_SYSTEM_WINDOWS_HEADERS_H__ */
