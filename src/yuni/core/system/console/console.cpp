
#include "console.h"
#ifndef YUNI_OS_WINDOWS
# include <unistd.h>
#endif



namespace Yuni
{
namespace System
{
namespace Console
{

	bool IsStdoutTTY()
	{
		# ifdef YUNI_OS_WINDOWS
		return false;
		# else
		return isatty(1);
		# endif
	}


	bool IsStderrTTY()
	{
		# ifdef YUNI_OS_WINDOWS
		return false;
		# else
		return isatty(2);
		# endif
	}




} // namespace Console
} // namespace System
} // namespace Yuni

#
