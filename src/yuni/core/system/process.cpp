
#include "process.h"
#ifndef YUNI_OS_WINDOWS
#	ifndef YUNI_OS_HAIKU
#		include <sys/errno.h>
#	endif
#	include <unistd.h>
#else
#	include "windows.hdr.h"
#endif




namespace Yuni
{

	uint64 ProcessID()
	{
		#if defined(YUNI_OS_WINDOWS)
		return (uint64) GetCurrentProcessId();
		#else
		return (uint64) getpid();
		#endif
	}



} // namespace Yuni
