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
#include "environment.h"
#include "windows.hdr.h"
#include "../traits/cstring.h"
#ifdef YUNI_HAS_STDLIB_H
#	include <stdlib.h>
#endif
#include "../string/wstring.h"



namespace Yuni
{
namespace System
{
namespace Environment
{

	namespace // anonymous
	{

		template<class StringT>
		inline bool ReadImpl(const AnyString& name, StringT& out, bool emptyBefore)
		{
			if (emptyBefore)
				out.clear();

			#ifdef YUNI_OS_WINDOWS
			{
				WString nameUTF16(name);

				DWORD size = GetEnvironmentVariableW(nameUTF16.c_str(), nullptr, 0);
				if (size > 0 and size <= 32767) // windows hard-coded value
				{
					// allocating a new buffer for receiving the value
					wchar_t* buffer = (wchar_t*)::malloc(sizeof(wchar_t) * size);
					if (buffer)
					{
						size = GetEnvironmentVariableW(nameUTF16.c_str(), buffer, size);
						if (size != 0)
						{
							int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, buffer, (int) size - 1, NULL, 0, NULL, NULL);
							if (sizeRequired > 0)
							{
								out.reserve(out.size() + sizeRequired);
								int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, buffer, (int) size - 1, out.data() + out.size(), size, NULL, NULL);
								::free(buffer);
								out.resize(out.size() + (uint) sizeRequired);
								return true;
							}
						}
						::free(buffer);
					}
				}
			}
			#else
			{
				# ifdef YUNI_HAS_STDLIB_H
				const char* e = ::getenv(name.c_str());
				if (e and '\0' != *e)
				{
					out += e;
					return true;
				}
				return false;

				# else
				# error missing implementation
				assert(false and "missing implementation");
				# endif
			}
			#endif // windows

			return false; // fallback
		}

	} // anonymous namespace





	String Read(const AnyString& name)
	{
		#ifdef YUNI_OS_WINDOWS
		{
			String out;
			ReadImpl(name, out, false);
			return out;
		}
		#else
		{
			# ifdef YUNI_HAS_STDLIB_H
			return ::getenv(name.c_str());
			# else
			#error not implemented			
			# endif
		}
		#endif
		return String(); // fallback
	}


	bool Read(const AnyString& name, Clob& out, bool emptyBefore)
	{
		return ReadImpl(name, out, emptyBefore);
	}


	bool Read(const AnyString& name, String& out, bool emptyBefore)
	{
		return ReadImpl(name, out, emptyBefore);
	}






} // namespace Environment
} // namespace System
} // namespace Yuni

