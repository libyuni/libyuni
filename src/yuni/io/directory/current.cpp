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
		static inline bool FetchAndAppendCurrentDirectory(StringT& string)
		{
			#ifdef YUNI_OS_WINDOWS
			{
				wchar_t* cwd = _wgetcwd(nullptr, 0); // length: Arbitrary value
				if (cwd != nullptr)
				{
					const int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, cwd, -1, nullptr, 0,  nullptr, nullptr);
					if (sizeRequired > 0)
					{
						string.reserve(string.size() + sizeRequired + 1);
						char* buffer = string.data();
						if (nullptr != buffer) // just in case
						{
							buffer += string.size();

							if (WideCharToMultiByte(CP_UTF8, 0, cwd, -1, buffer, sizeRequired,  nullptr, nullptr) > 0)
							{
								string.resize(string.size() + sizeRequired);
								free(cwd);
								return true;
							}
						}
					}

					free(cwd);
				}
			}
			#else
			{
				// arbitrary - good enough for most cases
				uint more = 512 - 1;
				do
				{
					string.reserve(string.size() + more);
					char* buffer = string.data();
					// making sure that the buffer has been allocated
					if (YUNI_UNLIKELY(nullptr == buffer or string.capacity() <= string.size()))
						return false;

					buffer += string.size();
					size_t length = string.capacity() - string.size() - 1;

					char* path = ::getcwd(buffer, length);
					if (YUNI_LIKELY(path))
					{
						string.resize(string.size() + (uint)strlen(path));
						return true;
					}
					more += 2096; // already have -1
				}
				while (more < 65535);
			}
			#endif

			// just in case the inner content has been modified
			string.resize(string.size());
			return false;
		}

	} // anonymous namespace



	String Get()
	{
		String current;
		FetchAndAppendCurrentDirectory(current);
		return current;
	}


	bool Get(String& out, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		return FetchAndAppendCurrentDirectory(out);
	}


	bool Get(Clob& out, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		return FetchAndAppendCurrentDirectory(out);
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
