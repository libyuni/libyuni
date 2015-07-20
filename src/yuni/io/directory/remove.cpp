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
#include "../io.h"
#include "../filename-manipulation.h"
#include "../directory.h"

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <errno.h>
#ifdef YUNI_HAS_STDLIB_H
# include <stdlib.h>
#endif
#ifndef YUNI_OS_MSVC
# include <dirent.h>
# include <unistd.h>
#endif
#ifdef YUNI_OS_WINDOWS
# include "../../core/system/windows.hdr.h"
# include <shellapi.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include "../../core/string.h"
#include "../../core/string/wstring.h"
#include <stdio.h>

#include <fstream>
#define SEP Yuni::IO::Separator



namespace Yuni
{
namespace IO
{
namespace Directory
{

	# ifndef YUNI_OS_WINDOWS


	namespace // Anonymous namespace
	{

		static bool RmDirRecursiveInternal(const AnyString& path)
		{
			DIR* dp = ::opendir(path.c_str());
			if (dp)
			{
				struct dirent* ep = ::readdir(dp);
				if (ep)
				{
					CString<1024> buffer;
					do
					{
						if (ep->d_type == DT_DIR)
						{
							const char* const p = ep->d_name;
							bool sysfolder = (p[0] == '.' and (p[1] == '\0' or (p[1] == '.' and p[2] == '\0')));
							if (not sysfolder)
							{
								buffer.clear() << path << SEP << (const char*) ep->d_name;
								RmDirRecursiveInternal(buffer.c_str());
							}
						}
						else
						{
							buffer.clear() << path << SEP << (const char*) ep->d_name;
							::unlink(buffer.c_str());
						}
					}
					while (NULL != (ep = ::readdir(dp)));
				}
				(void)::closedir(dp);
			}
			return (0 == rmdir(path.c_str()));
		}

	} // anonymous namespace


	# endif


	# ifdef YUNI_OS_WINDOWS

	namespace // anonymous
	{

		static bool RecursiveDeleteWindow(const wchar_t* path, uint size)
		{
			enum { maxLen = (MAX_PATH < 4096) ? 4096 : MAX_PATH };

			if (size >= maxLen)
				return false;

			// temporary buffer for filename manipulation
			wchar_t* filename = new (std::nothrow) wchar_t[maxLen];
			if (nullptr == filename)
				return false;
			::wcsncpy_s(filename, maxLen, path, size);
			if (size + 2 < maxLen)
			{
				filename[size++] = L'\\';
				filename[size++] = L'*';
				filename[size]   = L'\0';
			}

			// temporary buffer
			WIN32_FIND_DATAW filedata;
			HANDLE handle = ::FindFirstFileW(filename, &filedata);
			if (handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					// Dots folders are meaningless (`.` and `..`)
					if (filedata.cFileName[0] == L'.')
					{
						if (not ::wcscmp(filedata.cFileName, L".") or not ::wcscmp(filedata.cFileName, L".."))
							continue;
					}

					// Overwrite the '*'
					int newSize = size - 1;
					// Prepare the new filename
					{
						int written = 0;
						for (; newSize < maxLen && filedata.cFileName[written]; ++newSize, ++written)
							filename[newSize] = filedata.cFileName[written];
						if (written <= 0 or newSize >= maxLen)
						{
							::FindClose(handle);
							delete[] filename;
							return false;
						}
						filename[newSize] = L'\0';
					}

					// Recursively delete the sub-folder
					if (0 != (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						if (not RecursiveDeleteWindow(filename, newSize))
						{
							::FindClose(handle);
							delete[] filename;
							return false;
						}
					}
					else
					{
						// Simply delete the file
						// If the file has the read-only attribute, trying to
						// remove it first.
						if (0 != (filedata.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
							::SetFileAttributesW(filename, FILE_ATTRIBUTE_NORMAL);

						// Trying to delete the file
						if (not ::DeleteFileW(filename))
						{
							::FindClose(handle);
							delete[] filename;
							return false;
						}
					}
				}
				while (::FindNextFileW(handle, &filedata));

				// resource cleanup
				::FindClose(handle);
			}

			delete[] filename;

			// Remove the directory itself
			return (0 != ::RemoveDirectoryW(path));
		}

	} // anonymous

	# endif



	bool Remove(const AnyString& path)
	{
		if (path.empty())
			return true;

		# ifdef YUNI_OS_WINDOWS
		{
			String canon;
			Canonicalize(canon, path);

			Private::WString<true /*false*/, false> fsource(canon);
			if (fsource.empty())
				return false;

			// SHFILEOPSTRUCT operation;
			// operation.hwnd = nullptr;
			// operation.wFunc = FO_DELETE;
			// operation.fFlags |= FOF_NOCONFIRMATION;
			// // Requires double '\0' termination
			// auto doubleTerminated = new wchar_t[fsource.size() + 2];
			// ::wcsncpy(doubleTerminated, fsource.c_str(), fsource.size() + 1);
			// doubleTerminated[fsource.size() + 1] = L'\0';
			// operation.pFrom = doubleTerminated;
			// return 0 == ::SHFileOperation(&operation);

			return RecursiveDeleteWindow(fsource.c_str(), fsource.size());
		}
		# else
		{
			String p(path);
			return RmDirRecursiveInternal(p);
		}
		# endif
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

