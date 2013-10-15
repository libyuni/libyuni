
#include "../io.h"
#include "../directory.h"
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
			DIR* dp;
			struct dirent* ep;
			CString<2096> buffer;
			struct stat st;

			if (NULL != (dp = ::opendir(path.c_str())))
			{
				while (NULL != (ep = ::readdir(dp)))
				{
					buffer.clear() << path << SEP << (const char*) ep->d_name;
					if (0 == ::stat(buffer.c_str(), &st))
					{
						if (S_ISDIR(st.st_mode))
						{
							if (strcmp(".", (ep->d_name)) != 0 and strcmp("..", (ep->d_name)) != 0)
							{
								RmDirRecursiveInternal(buffer.c_str());
								::rmdir(buffer.c_str());
							}
						}
						else
							::unlink(buffer.c_str());
					}
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

		static bool RecursiveDeleteWindow(const wchar_t* path)
		{
			// WARNING This function requires a final backslash !
			enum
			{
				maxLen = (MAX_PATH < 1024) ? 1024 : MAX_PATH,
			};

			// temporary buffer
			WIN32_FIND_DATAW filedata;
			HANDLE handle = FindFirstFileW(path, &filedata);
			if (handle != INVALID_HANDLE_VALUE)
			{
				// temporary buffer for filename manipulation
				// The creation of this variable is delayed to avoid useless
				// memory allocation on empty folders
				wchar_t* filename = nullptr;
				do
				{
					// Dots folders are meaningless (`.` and `..`)
					if (filedata.cFileName[0] == L'.')
					{
						if (!wcscmp(filedata.cFileName, L".") or !wcscmp(filedata.cFileName, L".."))
							continue;
					}

					if (!filename)
						filename = new wchar_t[maxLen];

					// Prepare the new filename
					{
						#ifdef YUNI_OS_MINGW
						int written = snwprintf(filename, maxLen, L"%s\\%s", path, filedata.cFileName);
						#else
						int written = swprintf_s(filename, maxLen, L"%s\\%s", path, filedata.cFileName);
						#endif // YUNI_OS_MINGW
						if (written <= 0 or written == maxLen)
						{
							FindClose(handle);
							delete[] filename;
							return false;
						}
					}

					// Recursively delete the sub-folder
					if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						if (!RecursiveDeleteWindow(filename))
						{
							FindClose(handle);
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
							SetFileAttributesW(filename, FILE_ATTRIBUTE_NORMAL);
						// Trying to delete the file
						if (!DeleteFileW(filename))
						{
							FindClose(handle);
							delete[] filename;
							return false;
						}
					}
				}
				while (FindNextFileW(handle, &filedata));

				// resource cleanup
				delete[] filename;
				FindClose(handle);
			}

			// Remove the directory itself
			return (0 != RemoveDirectoryW(path));
		}

	} // anonymous

	# endif



	bool Remove(const AnyString& path)
	{
		if (path.empty())
			return true;

		# ifdef YUNI_OS_WINDOWS
		using namespace std;

		Private::WString<true, true> fsource(path);
		if (fsource.empty())
			return false;
		// In case we were given UNIX slashes
		fsource.replace(L'/', L'\\');
		return RecursiveDeleteWindow(fsource.c_str());
		# else
		String p(path);
		return RmDirRecursiveInternal(p);
		# endif
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

