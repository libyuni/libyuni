#ifndef __YUNI_THREADS_PTHREAD_H__
# define __YUNI_THREADS_PTHREAD_H__


# ifndef YUNI_NO_THREAD_SAFE
#	ifndef YUNI_OS_WINDOWS
#		ifndef _MULTI_THREADED
# 			define _MULTI_THREADED
#		endif
#		include <pthread.h>
#	endif
# endif


#endif // __YUNI_THREADS_PTHREAD_H__
