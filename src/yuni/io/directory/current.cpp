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
namespace IO
{
namespace Directory
{
namespace Current
{

	namespace // anonymous
	{

		template<class StringT>
		static inline void AppendCurrentDirectory(StringT& string)
		{
			#ifdef YUNI_OS_WINDOWS
			{
				const wchar_t* c = _wgetcwd(nullptr, 0 /* Arbitrary value */);

				const int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, c, -1, nullptr, 0,  nullptr, nullptr);
				if (sizeRequired > 0)
				{
					char* buffer = new (std::nothrow) char[sizeRequired + 1];
					if (nullptr != buffer)
					{
						if (WideCharToMultiByte(CP_UTF8, 0, c, -1, buffer, sizeRequired,  nullptr, nullptr) > 0)
						{
							buffer[sizeRequired] = '\0'; // just in case
							string.append(buffer, sizeRequired);
						}
						delete[] buffer;
					}
				}
			}
			#else
			{
				string.append(::getcwd(nullptr, 0 /* arbitrary value */));
			}
			#endif
		}

	} // anonymous namespace



	String Get()
	{
		String current;
		AppendCurrentDirectory(current);
		return current;
	}


	void Get(String& out, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		AppendCurrentDirectory(out);
	}


	void Get(Clob& out, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		AppendCurrentDirectory(out);
	}



	bool Set(const AnyString& path)
	{
		if (path.empty())
			return false;

		# ifdef YUNI_OS_WINDOWS
		{
			bool success = false;
			wchar_t* fsource = new (std::nothrow) wchar_t[path.size() + 4];
			if (fsource)
			{
				int n = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), path.size(), fsource, path.size());
				if (n > 0)
				{
					fsource[n] = L'\0'; // This string must be double-null terminated
					success = (0 == _wchdir(fsource));
				}
				delete[] fsource;
			}
			return success;
		}
		# else
		{
			return (0 == ::chdir(path.c_str()));
		}
		# endif
	}





} // namespace Current
} // namespace Directory
} // namespace IO
} // namespace Yuni
