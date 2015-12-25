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
#include "uuid.h"
#ifndef YUNI_OS_WINDOWS
# include <uuid/uuid.h>
#else
# include <objbase.h>
#endif
#include "../core/string.h"
#include <cassert>
#include <iostream>



namespace Yuni
{

	void UUID::generate()
	{
		assert(sizeof(StorageType) == 16 and "Invalid storage size for uuid");

		# ifndef YUNI_OS_WINDOWS
		assert(sizeof(uuid_t) == 16);
		uuid_generate(pValue.cstring);
		# else
		if (S_OK != ::CoCreateGuid((::GUID*)pValue.cstring)) // Sadly, the call can fail
			clear();
		# endif
	}


	void UUID::writeToCString(char cstring[42]) const
	{
		assert(cstring and "invalid pointer");

		# ifndef YUNI_OS_WINDOWS
		uuid_unparse(pValue.cstring, cstring);
		# else
		// StringFromGUID2 returns a string enclosed in braces
		// Anything less than 39 would make the call fail
		wchar_t buffer[39];
		::StringFromGUID2(*(::GUID*)pValue.cstring, buffer, 39);

		// Convert to non-wide string, and cut the prepended and appended braces
		#ifdef YUNI_OS_MINGW
		// we should have something like {000000000-0000-0000-0000-00000000000} in buffer
		if (::wcstombs(cstring, buffer + 1, 36) <= 0)
			::strncpy(cstring, "000000000-0000-0000-0000-00000000000", 36);
		#else
		size_t converted = 0;
		// we should have something like {000000000-0000-0000-0000-00000000000} in buffer
		if (::wcstombs_s(&converted, cstring, 42, buffer + 1, 36) != 0)
			::strcpy_s(cstring, 36, "000000000-0000-0000-0000-00000000000");
		#endif // YUNI_OS_MINGW
		else
		{
			// The guid produced on Windows is uppercase
			for (uint i = 0; i != 36; ++i)
				cstring[i] = static_cast<char>(String::ToLower(cstring[i]));
		}
		// Do not forget the null terminator
		cstring[36] = '\0';
		# endif
	}


	bool UUID::initializeFromCString(const char* cstring)
	{
		assert(cstring != NULL);

		# ifndef YUNI_OS_WINDOWS
		// Why uuid_parse takes a char* and not a const char* ??
		return (0 == uuid_parse(const_cast<char*>(cstring), pValue.cstring));
		# else
		// Stop complaining, the Windows implementation is way worse.
		char* cstring_noconst = const_cast<char*>(cstring);
		uchar* cstring_unsigned = (uchar*)(cstring_noconst);
		return (RPC_S_OK == ::UuidFromStringA(cstring_unsigned, (::GUID*)pValue.cstring));
		# endif
	}


	bool UUID::assign(AnyString string)
	{
		// remove useless whitespaces
		string.trim();

		if (string.size() >= 36)
		{
			char buffer[64]; // 8 Byte Stack Alignment
			::memcpy(buffer, string.c_str(), 36 * sizeof(char));
			buffer[36] = '\0';
			return initializeFromCString(buffer);
		}
		return false;
	}


	size_t UUID::hash() const
	{
		// TODO This hash may not be suitable for hashing guids,
		std::size_t r = 0;
		const uchar* p = pValue.cstring;
		for (uint i = 0; i != 16; ++i)
			r = static_cast<uint>(p[i]) + (r << 6) + (r << 16) - r;

		return r;
	}



} // namespace Yuni






std::ostream& operator << (std::ostream& out, const Yuni::UUID& rhs)
{
	// WriteToCString is guarantee to have 42 chars
	char cstring[42];
	Yuni::Private::UUID::Helper::WriteToCString(cstring, rhs);
	out.write(cstring, 36);
	return out;
}

