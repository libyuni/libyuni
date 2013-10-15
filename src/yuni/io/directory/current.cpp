
#include "../io.h"
#include "../directory.h"
#ifndef YUNI_OS_WINDOWS
# include <unistd.h>
#else
# include "../../core/system/windows.hdr.h"
# include <direct.h>
# include <stdlib.h>
# include <stdio.h>
#endif



namespace Yuni
{
namespace Private
{
namespace IO
{
namespace Directory
{


	char* CurrentDirectory()
	{

		# ifdef YUNI_OS_WINDOWS


		char* ret = NULL;
		const wchar_t* c = _wgetcwd(NULL, 0 /* Arbitrary value */);

		const int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, c, -1, NULL, 0,  NULL, NULL);
		if (sizeRequired > 0)
		{
			ret = (char*) ::malloc(sizeRequired * sizeof(char) + 1);
			if (ret)
			{
				if (WideCharToMultiByte(CP_UTF8, 0, c, -1, ret, sizeRequired,  NULL, NULL) > 0)
				{
					ret[sizeRequired] = '\0';
				}
				else
				{
					::free(ret);
					ret = NULL;
				}
			}
		}

		return ret;

		# else

		return ::getcwd(NULL, 0 /* arbitrary value */);

		# endif

	}



} // namespace Directory
} // namespace IO
} // namespace Private
} // namespace Yuni




namespace Yuni
{
namespace IO
{
namespace Directory
{
namespace Current
{

	String Get()
	{
		char* c = Yuni::Private::IO::Directory::CurrentDirectory();
		String current = c;
		::free(c);
		return current;
	}


	bool Set(const AnyString& path)
	{
		# ifdef YUNI_OS_WINDOWS
		wchar_t* fsource = new wchar_t[path.size() + 4];
		int n = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), path.size(), fsource, path.size());
		if (n <= 0)
			return false;
		fsource[n]     = L'\0'; // This string must be double-null terminated

		const bool r = (0 == _wchdir(fsource));
		delete[] fsource;

		return r;

		# else
		return (0 == ::chdir(path.c_str()));
		# endif
	}





} // namespace Current
} // namespace Directory
} // namespace IO
} // namespace Yuni

