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
#include "../../yuni.h"

#ifdef YUNI_OS_WINDOWS
# include "username.h"
# include "windows.hdr.h"


namespace Yuni
{
namespace Private
{
namespace System
{

	uint WindowsUsername(char* cstring, uint size)
	{
		enum
		{
			// The maximum length, (see UCLEN)
			defaultSize = 256,
		};
		DWORD unwsize = defaultSize;

		wchar_t unw[defaultSize];
		if (GetUserNameW(unw, &unwsize))
		{
			if (unwsize > 0)
			{
				// The variable `unwsize` contains the final zero
				--unwsize;
				// Getting the size of the buffer into UTF8
				int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, unw, unwsize, NULL, 0,  NULL, NULL);
				if (sizeRequired > 0)
				{
					if (static_cast<uint>(sizeRequired) > size)
						sizeRequired = size;
					WideCharToMultiByte(CP_UTF8, 0, unw, unwsize, cstring, sizeRequired,  NULL, NULL);
					return static_cast<uint>(sizeRequired);
				}
			}
		}
		return 0;
	}




} // namespace System
} // namespace Private
} // namespace Yuni

#endif
