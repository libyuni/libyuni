
#include "suspend.h"
#ifdef YUNI_OS_WINDOWS
# include "windows.hdr.h"
#else
# ifdef YUNI_HAS_UNISTD_H
#	include <unistd.h>
# endif
#endif



namespace Yuni
{

	void Suspend(uint seconds)
	{
		# if defined(YUNI_OS_WINDOWS)
		Sleep(1000 * seconds);
		# else
		#	ifdef YUNI_HAS_UNISTD_H
		::sleep(seconds);
		#	else
		#	warning function sleep() missing
		#	endif
		# endif
	}


	void SuspendMilliSeconds(uint milliseconds)
	{
		# if defined(YUNI_OS_WINDOWS)
		Sleep(milliseconds);
		# else
		#	ifdef YUNI_HAS_UNISTD_H
		::usleep(1000 * milliseconds);
		#	else
		#	warning function usleep() missing
		#	endif
		# endif
	}



} // namespace Yuni

