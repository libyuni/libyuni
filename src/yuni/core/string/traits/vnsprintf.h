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
#include "../../traits/length.h"
#include "integer.h"



namespace Yuni
{
namespace Private
{
namespace CStringImpl
{


	/*!
	** \brief Generic implementation of vsnprintf
	**
	** \return Number of characters written, -1 if the string was truncated
	*/
	template<class C>
	int vnsprintf(C* buffer, size_t bufferSize, const C* format, va_list argptr);


	template<>
	inline int
	vnsprintf<char>(char* buffer, size_t bufferSize, const char* format, va_list argptr)
	{
		// Behavior: -1 when the string was truncated

		#if defined YUNI_OS_MSVC
		{
			// From http://msdn.microsoft.com/en-us/library/d3xd30zz(VS.80).aspx
			//
			// If count is _TRUNCATE, then these functions write as much of the string as will
			// fit in buffer while leaving room for a terminating null. If the entire string
			// (with terminating null) fits in buffer, then these functions return the number
			// of characters written (not including the terminating null); otherwise, these
			// functions return -1 to indicate that truncation occurred.
			return ::_vsnprintf_s(buffer, bufferSize, _TRUNCATE, format, argptr);
		}
		#else // YUNI_OS_MSVC
		{
			// man 3 vnsprintf
			//
			// Upon  successful return, these functions return the number of characters printed
			// (not including the trailing ’\0’ used to end output to strings).
			// The functions snprintf() and vsnprintf() do not write more than size bytes
			// (including the trailing ’\0’).  If the output was truncated due to this limit then
			// the return value is the number of characters (not including the trailing ’\0’)
			// which would  have been  written  to  the  final  string if enough space had been
			// available. Thus, a return value of size or more means that the output was truncated.
			// (See also below under NOTES.)  If an output error is encountered, a negative
			// value is returned.
			int r = ::vsnprintf(buffer, bufferSize, format, argptr);
			return (((size_t) r >= bufferSize) ? -1 : r);
		}
		#endif
	}




} // namespace CStringImpl
} // namespace Private
} // namespace Yuni
