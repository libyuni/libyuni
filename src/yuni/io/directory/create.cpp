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
#include "../directory.h"
#include "commons.h"
#include "../file.h"
#ifdef YUNI_OS_WINDOWS
# include "../../core/string/wstring.h"
#endif


namespace Yuni
{
namespace IO
{
namespace Directory
{


	# ifdef YUNI_OS_WINDOWS

	static bool WindowsMake(const AnyString& path)
	{
		String norm;
		Yuni::IO::Normalize(norm, path);

		WString wstr(norm, true);
		if (wstr.size() < 4)
			return false;
		wchar_t* t = wstr.data() + 4;

		while (*t != L'\0')
		{
			if ((*t == L'\\' or *t == L'/') and (*(t-1) != ':'))
			{
				*t = L'\0';
				if (!CreateDirectoryW(wstr.c_str(), nullptr))
				{
					if (GetLastError() != ERROR_ALREADY_EXISTS)
						return false;
				}
				*t = L'\\';
			}
			++t;
		}

		if (not CreateDirectoryW(wstr.c_str(), nullptr))
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
				return false;
		}
		return true;
	}


	# else



	static bool UnixMake(const AnyString& path, uint mode)
	{
		const uint len = path.size();
		char* buffer = new char[len + 1];
		YUNI_MEMCPY(buffer, len, path.c_str(), len);
		buffer[len] = '\0';
		char* pt = buffer;
		char tmp;

		do
		{
			if ('\\' == *pt or '/' == *pt or '\0' == *pt)
			{
				tmp = *pt;
				*pt = '\0';
				if ('\0' != buffer[0] and '\0' != buffer[1] and '\0' != buffer[2])
				{
					if (mkdir(buffer, static_cast<mode_t>(mode)) < 0)
					{
						if (errno != EEXIST and errno != EISDIR and errno != ENOSYS)
						{
							delete[] buffer;
							return false;
						}
					}
				}
				if ('\0' == tmp)
					break;
				*pt = tmp;
			}
			++pt;
		}
		while (true);

		delete[] buffer;
		return true;
	}

	# endif






	bool Create(const AnyString& path, uint mode)
	{
		if (not path.empty() and not Yuni::IO::Exists(path))
		{
			# ifdef YUNI_OS_WINDOWS
			// `mode` is not used on Windows
			(void) mode;
			return WindowsMake(path);
			# else
			return UnixMake(path, mode);
			# endif
		}
		return true;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

