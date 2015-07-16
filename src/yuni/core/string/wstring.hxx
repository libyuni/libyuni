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
#pragma once
#include "wstring.h"
#ifdef YUNI_OS_WINDOWS


namespace Yuni
{
namespace Private
{

	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::WString(const AnyString& string)
	{
		prepareWString(string.c_str(), string.size());
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::WString(const char* cstring, uint size)
	{
		prepareWString(cstring, size);
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::~WString()
	{
		delete[] pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline uint WString<UNCPrefix, AppendSeparatorT>::size() const
	{
		return pSize;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline void WString<UNCPrefix, AppendSeparatorT>::replace(char from, char to)
	{
		for (uint i = 0; i != pSize; ++i)
		{
			if (pWString[i] == from)
				pWString[i] = to;
		}
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline bool WString<UNCPrefix, AppendSeparatorT>::empty() const
	{
		return (0 == pSize);
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline const wchar_t* WString<UNCPrefix, AppendSeparatorT>::c_str() const
	{
		return pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline wchar_t* WString<UNCPrefix, AppendSeparatorT>::c_str()
	{
		return pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::operator const wchar_t* () const
	{
		return pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	void WString<UNCPrefix, AppendSeparatorT>::prepareWString(const char* const cstring, uint size)
	{
		if (!size)
		{
			if (UNCPrefix)
			{
				pSize = 4;
				pWString = new wchar_t[5];
				pWString[0] = L'\\';
				pWString[1] = L'\\';
				pWString[2] = L'?';
				pWString[3] = L'\\';
				pWString[4] = L'\0';
			}
			else
			{
				pSize = 0;
				pWString = NULL;
			}
			return;
		}

		// Offset according to the presence of the UNC prefix
		const uint offset = UNCPrefix ? 4 : 0;

		// Size of the wide string. This value may be modified if there is not
		// enough room for converting the C-String
		pSize = size + offset + (AppendSeparatorT ? 1 : 0);

		// Allocate and convert the C-String. Several iterations may be required
		// for allocating enough room for the conversion.
		do
		{
			pWString = new wchar_t[pSize + (AppendSeparatorT ? 1 : 0) + 1];
			if (!pWString)
			{
				// Impossible to allocate the buffer. Aborting.
				pWString = NULL;
				pSize = 0;
				return;
			}

			// Appending the Windows UNC prefix
			if (UNCPrefix)
			{
				pWString[0] = L'\\';
				pWString[1] = L'\\';
				pWString[2] = L'?';
				pWString[3] = L'\\';
			}
			// Converting into Wide String
			const int n = MultiByteToWideChar(CP_UTF8, 0, cstring, size, pWString + offset, pSize + 1);
			if (n <= 0)
			{
				// An error has occured
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					// There is not enough room for the conversion
					// Trying again with more rooms
					pSize += (pSize > 2) ? pSize / 2 : 2;
					delete[] pWString;
					continue;
				}
				// This is a real error. Aborting.
				delete[] pWString;
				pWString = NULL;
				pSize = 0;
				return;
			}
			// Adding the final zero
			if (!AppendSeparatorT)
			{
				pWString[n + offset] = L'\0';
				pSize = n + offset;
			}
			else
			{
				pWString[n + offset] = L'\\';
				pWString[n + offset + 1] = L'\0';
				pSize = n + offset + 1;
			}
			return;
		}
		while (true);
	}





} // namespace Private
} // namespace Yuni

#endif // YUNI_OS_WINDOWS
