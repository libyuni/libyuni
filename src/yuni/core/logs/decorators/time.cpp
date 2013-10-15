
#include "../../system/windows.hdr.h"
#include <time.h>
#ifdef YUNI_OS_WINDOWS
# include <wchar.h>
#endif
#include "time.h"



namespace Yuni
{
namespace Private
{
namespace LogsDecorator
{

	# ifdef YUNI_OS_MINGW

	char* WriteCurrentTimestampToBufferMinGW()
	{
		time_t rawtime;
		::time(&rawtime);

		/* MinGW */
		// Note that unlike on (all?) POSIX systems, in the Microsoft
		// C library locatime() and gmtime() are multi-thread-safe, as the
		// returned pointer points to a thread-local variable. So there is no
		// need for localtime_r() and gmtime_r().
		return ::asctime(::localtime(&rawtime));
	}

	# else

	void WriteCurrentTimestampToBuffer(char buffer[32])
	{
		# ifdef YUNI_OS_MSVC
		__time64_t rawtime = ::_time64(nullptr);
		# else
		time_t rawtime = ::time(nullptr);
		# endif

		struct tm timeinfo;

		# if defined(YUNI_OS_MSVC)
		// Microsoft Visual Studio
		_localtime64_s(&timeinfo, &rawtime);
		// MSDN specifies that the buffer length value must be >= 26 for validity.
		asctime_s(buffer, 32, &timeinfo);
		# else
		// Unixes
		::localtime_r(&rawtime, &timeinfo);
		::asctime_r(&timeinfo, buffer);
		# endif
	}

	# endif





} // namespace LogsDecorator
} // namespace Private
} // namespace Yuni


