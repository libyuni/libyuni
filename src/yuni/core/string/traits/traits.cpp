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
#include "../../../yuni.h"
#include "traits.h"
#include <ctype.h>
#ifndef YUNI_OS_WINDOWS
#include <wchar.h>
#else
#include "../../../core/system/windows.hdr.h"
#endif



namespace Yuni
{
namespace Private
{
namespace CStringImpl
{


	bool Equals(const char* const s1, const char* const s2, uint len)
	{
		for (uint i = 0; i != len; ++i)
		{
			if (s1[i] != s2[i])
				return false;
		}
		return true;
	}


	bool EqualsInsensitive(const char* const s1, const char* const s2, uint len)
	{
		for (uint i = 0; i != len; ++i)
		{
			if (tolower(s1[i]) != tolower(s2[i]))
				return false;
		}
		return true;
	}


	int Compare(const char* const s1, uint l1, const char* const s2, uint l2)
	{
		const uint l = (l1 < l2) ? l1 : l2;

		for (uint i = 0; i != l; ++i)
		{
			if (s1[i] != s2[i])
				return (((unsigned char)s1[i] < (unsigned char)s2[i]) ? -1 : +1);
		}
		return (l1 == l2) ? 0 : ((l1 < l2) ? -1 : +1);
	}


	int CompareInsensitive(const char* const s1, uint l1, const char* const s2, uint l2)
	{
		const uint l = (l1 < l2) ? l1 : l2;

		for (uint i = 0; i != l; ++i)
		{
			if (tolower(s1[i]) != tolower(s2[i]))
				return ((tolower((unsigned char)s1[i]) < tolower((unsigned char)s2[i])) ? -1 : +1);
		}
		return (l1 == l2) ? 0 : ((l1 < l2) ? -1 : +1);
	}


	bool Glob(const char* const s, uint l1, const char* const pattern, uint patternlen)
	{
		if (patternlen)
		{
			if (l1)
			{
				uint e = 0;
				uint prev = ((uint) -1);
				for (uint i = 0 ; i < l1; ++i)
				{
					if ('*' == pattern[e])
					{
						if (e + 1 == patternlen)
							return true;
						while (pattern[e+1] == '*')
							++e;
						if (e + 1 == patternlen)
							return true;

						prev = e;
						if (pattern[e + 1] == s[i])
							e += 2;
					}
					else
					{
						if (pattern[e] == s[i])
							++e;
						else
						{
							if (prev != ((uint) -1))
								e = prev;
							else
								return false;
						}
					}
				}
				return (e == patternlen);
			}
			return false;
		}
		return (l1 == 0);
	}



	size_t WCharToUTF8SizeNeeded(const wchar_t* wbuffer, size_t length)
	{
		#ifndef YUNI_OS_WINDOWS
		{
			mbstate_t state;
			memset (&state, '\0', sizeof (state));
			size_t written = wcsnrtombs(nullptr, &wbuffer, length, 0, &state);
			return (written != (size_t) -1) ? written : 0;
		}
		#else
		{
			int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wbuffer, length, nullptr, 0,  nullptr, nullptr);
			return sizeRequired > 0 ? (size_t) sizeRequired : 0;
		}
		#endif
	}


	size_t WCharToUTF8(char*& out, size_t maxlength, const wchar_t* wbuffer, size_t length)
	{
		#ifndef YUNI_OS_WINDOWS
		{
			mbstate_t state;
			memset (&state, '\0', sizeof (state));
			size_t written = wcsnrtombs(out, &wbuffer, length, maxlength, &state);
			return (written != (size_t) -1) ? written : 0;
		}
		#else
		{
			int written = WideCharToMultiByte(CP_UTF8, 0, wbuffer, length, out, maxlength,  nullptr, nullptr);
			return (written > 0) ? (size_t) written : 0;
		}
		#endif
	}





} // namespace CStringImpl
} // namespace Private
} // namespace Yuni
