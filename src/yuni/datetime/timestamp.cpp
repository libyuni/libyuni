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
#include "timestamp.h"
#include <time.h>
#include <cassert>
#include "../core/system/gettimeofday.h"


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


	Timestamp NowMilliSeconds()
	{
		timeval now;
		YUNI_SYSTEM_GETTIMEOFDAY(&now, nullptr);
		return now.tv_sec * 1000 + now.tv_usec / 1000;
	}



} // namespace DateTime
} // namespace Yuni




namespace Yuni
{
namespace Private
{
namespace DateTime
{

	namespace // anonymous
	{

		static inline uint FormatString(char* buffer, uint size, const char* format, sint64 timestamp)
		{
			assert(format != nullptr and '\0' != *format and "invalid format");

			uint written = 0;

			// Note that unlike on (all?) POSIX systems, in the Microsoft
			// C library localtime() and gmtime() are multi-thread-safe, as the
			// returned pointer points to a thread-local variable. So there is no
			// need for localtime_r() and gmtime_r().

			// \note The variable stdtimestamp is used to ensure the compilation on
			//  32bits platforms

			#ifdef YUNI_OS_MINGW
			{
				// MinGW
				time_t stdtimestamp = (time_t) timestamp;
				written = (uint)::strftime(buffer, size, format, ::localtime(&stdtimestamp));
			}
			#else
			{
				struct tm timeinfo;

				#ifdef YUNI_OS_MSVC
				{
					// Microsoft Visual Studio
					_localtime64_s(&timeinfo, &timestamp);
					written = (uint)::strftime(buffer, size, format, &timeinfo);
				}
				#else
				{
					// Unixes
					time_t stdtimestamp = (time_t) timestamp;
					::localtime_r(&stdtimestamp, &timeinfo);
					written = (uint)::strftime(buffer, size, format, &timeinfo);
				}
				#endif
			}
			#endif

			return (written and written < size) ? written : 0;
		}

	} // anonymous namespace




	char* FormatTimestampToString(const AnyString& format, sint64 timestamp)
	{
		assert(not format.empty() and "this routine must not be called if the format is empty");

		if (timestamp <= 0)
		{
			#ifdef YUNI_OS_MSVC
			timestamp = (sint64)::_time64(NULL);
			#else
			timestamp = (sint64)::time(NULL);
			#endif
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
			buffer = (char*)::realloc(buffer, size * sizeof(char));
			if (FormatString(buffer, size, format.c_str(), timestamp))
				return buffer;

			// there was not enough room for storing the formatted string
			// trying again with more rooms
			size += 256;
		}
		while (0 != --tick);

		::free(buffer);
		return nullptr;
	}





} // namespace DateTime
} // namespace Private
} // namespace Yuni
