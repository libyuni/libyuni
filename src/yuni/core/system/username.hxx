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
#include <stdlib.h>
#include "../traits/cstring.h"
#ifndef YUNI_OS_WINDOWS
# include "environment.h"
#endif



namespace Yuni
{
namespace Private
{
namespace System
{

	# ifdef YUNI_OS_WINDOWS
	/*!
	** \brief Retrieves the calling user's name into a mere C-String buffer (Windows only)
	*/
	YUNI_DECL uint WindowsUsername(char* cstring, uint size);
	# endif



} // namespace System
} // namespace Private
} // namespace Yuni



namespace Yuni
{
namespace System
{


	template<class StringT>
	bool Username(StringT& out, bool emptyBefore)
	{
		// Assert, if a C* container can not be found at compile time
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, SystemUsername_InvalidTypeForString);

		# ifdef YUNI_OS_WINDOWS
		{
			if (emptyBefore)
				out.clear();

			// The maximum size for a username is 256 on any platform
			// We will reserve enough space for that size
			out.reserve(out.size() + 256 + 1 /* zero-terminated */);

			// The target buffer
			char* target = const_cast<char*>(out.c_str()) + out.size();
			// Since it may be any string (like a static one), we may have less than 256 chars
			uint size = out.capacity() - out.size();
			if (!size)
				return false; // not enough rooms

			// Appending the username to our buffer and retrieving the size of
			// the username
			uint written = Yuni::Private::System::WindowsUsername(target, size);
			if (written)
			{
				// The username has been written, we have to properly resize the string
				// (absolutely required for zero-terminated strings)
				out.resize(out.size() + written);
				return true;
			}
			return false;
		}
		# else
		{
			return System::Environment::Read("LOGNAME", out, emptyBefore);
		}
		# endif
	}





} // namespace System
} // namespace Yuni

