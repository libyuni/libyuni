#pragma once


# ifndef YUNI_NO_THREAD_SAFE
#	ifndef YUNI_OS_WINDOWS
#		ifndef _MULTI_THREADED
#			define _MULTI_THREADED
#		endif
#		include <pthread.h>
#	endif
# endif

