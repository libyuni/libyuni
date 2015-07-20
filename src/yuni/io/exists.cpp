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
#include "io.h"
#include "../core/system/windows.hdr.h"
#include "../core/string.h"
#include "../core/string/wstring.h"
#include <sys/stat.h>

#ifndef YUNI_OS_WINDOWS
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
#endif

#ifdef YUNI_HAS_STRING_H
# include <string.h>
#endif





namespace Yuni
{
namespace IO
{

	namespace // anonymous
	{

		static inline NodeType Stat(const AnyString& filename, yuint64& outSize, yint64& lastModified, bool followSymLink)
		{
			assert(not filename.empty());

			# ifdef YUNI_OS_WINDOWS
			{
				(void) followSymLink; // unused on windows

				const char* p = filename.c_str();
				AnyString::Size len = filename.size();

				// remove the final slash if any
				if (p[len - 1] == '\\' or p[len - 1] == '/')
				{
					if (0 == --len)
						return Yuni::IO::typeUnknown;
				}

				// Driver letters
				if (len == 2 and p[1] == ':' and String::IsAlpha(p[0]))
					return Yuni::IO::typeFolder;

				String norm;
				Yuni::IO::Normalize(norm, AnyString(p, len));

				// Conversion into wchar_t
				Private::WString<true> wstr(norm);
				if (not wstr.empty())
				{
					WIN32_FILE_ATTRIBUTE_DATA infoFile;
					if (0 != GetFileAttributesExW(wstr.c_str(), GetFileExInfoStandard, &infoFile))
					{
						LARGE_INTEGER size;
						size.HighPart = infoFile.nFileSizeHigh;
						size.LowPart  = infoFile.nFileSizeLow;
						outSize = size.QuadPart;
						lastModified = Yuni::Windows::FILETIMEToTimestamp(infoFile.ftLastWriteTime);

						return ((infoFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
							? Yuni::IO::typeFolder
							: Yuni::IO::typeFile;
					}
				}
			}
			# else // WINDOWS
			{
				struct stat s;
				int ret = (followSymLink) ? stat(filename.c_str(), &s) : lstat(filename.c_str(), &s);
				if (0 == ret)
				{
					lastModified = s.st_mtime;
					outSize = (uint64) s.st_size;

					if (0 != S_ISREG(s.st_mode))
						return Yuni::IO::typeFile;
					if (0 != S_ISDIR(s.st_mode))
						return Yuni::IO::typeFolder;
					if (0 != S_ISLNK(s.st_mode))
						return Yuni::IO::typeSymlink;
					if (0 != S_ISSOCK(s.st_mode))
						return Yuni::IO::typeSocket;

					return Yuni::IO::typeSpecial;
				}
			}
			# endif

			return Yuni::IO::typeUnknown;
		}


	} // anonymous namespace





	NodeType TypeOf(const AnyString& filename, bool followSymLink)
	{
		yuint64 size; // useless
		yint64 lastModified;
		return (YUNI_LIKELY(not filename.empty()))
			? Stat(filename, size, lastModified, followSymLink) : IO::typeUnknown;
	}


	NodeType FetchFileStatus(const AnyString& filename, yuint64& size, yint64& lastModified, bool followSymLink)
	{
		size = 0u;
		lastModified = 0;
		return (YUNI_LIKELY(not filename.empty()))
			? Stat(filename, size, lastModified, followSymLink) : IO::typeUnknown;
	}





} // namespace IO
} // namespace Yuni

