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
#ifndef YUNI_OS_WINDOWS
# include <unistd.h>
#else
# include "../../core/system/windows.hdr.h"
# include "../../core/string/wstring.h"
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
					string.append(cwd);
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

		String realpath;
		IO::Canonicalize(realpath, path);

		#ifdef YUNI_OS_WINDOWS
		{
			WString wstring(realpath);
			return (0 == _wchdir(wstring.c_str()));
		}
		#else
		{
			return (0 == ::chdir(realpath.c_str()));
		}
		#endif
	}





} // namespace Current
} // namespace Directory
} // namespace IO
} // namespace Yuni
