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
#include "wstring.h"
#ifdef YUNI_OS_WINDOWS
#include "../system/windows.hdr.h"
#else
#include <wchar.h>
#endif




namespace Yuni
{

	void WString::prepareWString(const AnyString& string, bool uncprefix)
	{
		if (string.empty())
		{
			if (uncprefix)
			{
				pSize = 4;
				pWString = (wchar_t*)::realloc(pWString, sizeof(wchar_t) * 5);
				pWString[0] = L'\\';
				pWString[1] = L'\\';
				pWString[2] = L'?';
				pWString[3] = L'\\';
				pWString[4] = L'\0';
			}
			else
				clear();
			return;
		}

		if (string.size() > INT_MAX)
		{
			clear();
			return;
		}

		// Offset according to the presence of the UNC prefix
		const uint offset = (not uncprefix) ? 0 : 4;

		#ifdef YUNI_OS_WINDOWS
		{
			// Allocate and convert the C-String. Several iterations may be required
			// for allocating enough room for the conversion.
			const int sizeRequired = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string.size(), nullptr, 0);
			if (sizeRequired <= 0)
			{
				clear();
				return;
			}

			pSize = sizeRequired + offset;

			pWString = (wchar_t*) realloc(pWString, sizeof(wchar_t) * (pSize + 1));
			if (nullptr == pWString) // Impossible to allocate the buffer. Aborting.
			{
				clear();
				return;
			}

			// Converting into Wide String
			const int n = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()), pWString + offset, static_cast<int>(pSize - offset));
			if (n != sizeRequired)
			{
				assert(false and "most likely an error");
				clear();
				return;
			}
		}
		#else
		{
			const char* wcstr = string.c_str();

			mbstate_t state;
			memset (&state, '\0', sizeof (state));

			size_t sizeRequired = mbsnrtowcs(nullptr, &wcstr, string.size(), 0, &state);
			if (0 == sizeRequired or (size_t) -1 == sizeRequired)
			{
				clear();
				return;
			}

			pSize = sizeRequired + offset;

			pWString = (wchar_t*) realloc(pWString, sizeof(wchar_t) * (pSize + 1));
			if (nullptr == pWString) // Impossible to allocate the buffer. Aborting.
			{
				clear();
				return;
			}

			memset (&state, '\0', sizeof (state));
			size_t written = mbsnrtowcs(pWString + offset, &wcstr, static_cast<int>(string.size()), pSize - offset, &state);
			if (0 == written or (size_t) -1 == written)
			{
				clear();
				return;
			}
		}
		#endif


		// prepend the Windows UNC prefix
		if (uncprefix)
		{
			pWString[0] = L'\\';
			pWString[1] = L'\\';
			pWString[2] = L'?';
			pWString[3] = L'\\';
		}

		// always ensure that the string is zero terminated
		pWString[pSize] = L'\0';
	}




} // namespace Yuni
