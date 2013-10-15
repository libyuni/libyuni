
#include "timestamp.h"
#include <time.h>
#include <cassert>



namespace Yuni
{
namespace DateTime
{

	Timestamp Now()
	{
		# ifdef YUNI_OS_MSVC
		return (sint64) ::_time64(nullptr);
		# else
		return (sint64) ::time(nullptr);
		# endif
	}

} // namespace DateTime
} // namespace Yuni




namespace Yuni
{
namespace Private
{
namespace DateTime
{

	static inline uint FormatString(char* buffer, uint size, const char* format, sint64 timestamp)
	{
		assert(format and '\0' != *format and "invalid format");

		uint written;

		// Note that unlike on (all?) POSIX systems, in the Microsoft
		// C library localtime() and gmtime() are multi-thread-safe, as the
		// returned pointer points to a thread-local variable. So there is no
		// need for localtime_r() and gmtime_r().

		// \note The variable stdtimestamp is used to ensure the compilation on
		//  32bits platforms

		# ifdef YUNI_OS_MINGW
		// MinGW
		time_t stdtimestamp = (time_t) timestamp;
		written = (uint)::strftime(buffer, size, format, ::localtime(&stdtimestamp));
		# else
		struct tm timeinfo;
		#	ifdef YUNI_OS_MSVC
		// Microsoft Visual Studio
		_localtime64_s(&timeinfo, &timestamp);
		written = (uint)::strftime(buffer, size, format, &timeinfo);
		#	else
		// Unixes
		time_t stdtimestamp = (time_t) timestamp;
		::localtime_r(&stdtimestamp, &timeinfo);
		written = (uint)::strftime(buffer, size, format, &timeinfo);
		#	endif
		# endif

		return (written and written < size) ? written : 0;
	}


	char* FormatTimestampToString(const AnyString& format, sint64 timestamp)
	{
		assert(not format.empty() and "this routine must not be called if the format is empty");

		if (timestamp <= 0)
		{
			# ifdef YUNI_OS_MSVC
			timestamp = (sint64) ::_time64(NULL);
			# else
			timestamp = (sint64) ::time(NULL);
			# endif
		}

		// trying to guess the future size of the formatted string to reduce memory allocation
		uint size = format.size();
		// valgrind / assert...
		assert(format.c_str()[format.size()] == '\0' and "format must be zero-terminated");
		size += 128; // arbitrary value

		char* buffer = nullptr;
		uint tick = 10;
		do
		{
			buffer  = (char*)::realloc(buffer, size * sizeof(char));
			if (FormatString(buffer, size, format.c_str(), timestamp))
				return buffer;

			// there was not enough room for storing the formatted string
			// trying again with more rooms
			size += 256;
		}
		while (--tick);

		::free(buffer);
		return nullptr;
	}





} // namespace DateTime
} // namespace Private
} // namespace Yuni


