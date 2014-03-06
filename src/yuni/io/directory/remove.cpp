
#include "../io.h"
#include "../filename-manipulation.h"
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
			if (NULL != (dp = ::opendir(path.c_str())))
			{
				struct dirent* ep = nullptr;
				Clob buffer;

				while (NULL != (ep = ::readdir(dp)))
				{
					buffer.clear() << path << SEP << (const char*) ep->d_name;
					struct stat st;

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

		static bool RecursiveDeleteWindow(const wchar_t* path, uint size)
		{
			enum
			{
				maxLen = (MAX_PATH < 1024) ? 1024 : MAX_PATH,
			};

			// temporary buffer for filename manipulation
			wchar_t* filename = new wchar_t[maxLen];
			::wcsncpy(filename, path, size);
			if (size + 2 < maxLen)
			{
				filename[size++] = L'\\';
				filename[size++] = L'*';
				filename[size] = L'\0';
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
						if (not ::wcscmp(filedata.cFileName, L".") or
							not ::wcscmp(filedata.cFileName, L".."))
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
					if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						if (!RecursiveDeleteWindow(filename, newSize))
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
		using namespace std;

		String canon;
		Canonicalize(canon, path);

		Private::WString<true /*false*/, false> fsource(canon);
		if (fsource.empty())
			return false;

		/*
		SHFILEOPSTRUCT operation;
		operation.hwnd = nullptr;
		operation.wFunc = FO_DELETE;
		operation.fFlags |= FOF_NOCONFIRMATION;
		// Requires double '\0' termination
		auto doubleTerminated = new wchar_t[fsource.size() + 2];
		::wcsncpy(doubleTerminated, fsource.c_str(), fsource.size() + 1);
		doubleTerminated[fsource.size() + 1] = L'\0';
		operation.pFrom = doubleTerminated;
		return 0 == ::SHFileOperation(&operation);
		*/
		return RecursiveDeleteWindow(fsource.c_str(), fsource.size());
		# else
		String p(path);
		return RmDirRecursiveInternal(p);
		# endif
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

