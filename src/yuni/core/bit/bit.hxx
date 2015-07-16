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
#include "bit.h"




namespace Yuni
{
namespace Bit
{

	inline yuint32 Count(yuint32 i)
	{
		#ifdef YUNI_HAS_GCC_BUILTIN_POPCOUNT
		return static_cast<yuint32>(__builtin_popcount(i));
		#else
		// variable-precision SWAR algorithm
		i = i - ((i >> 1) & 0x55555555);
		i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
		return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		#endif
	}


	template<class T>
	inline uint Count(T data)
	{
		uint c = 0;
		while (data)
		{
			c += c & 0x1u;
			data >>= 1;
		}
		return c;
	}


	inline bool Get(const char* data, uint index)
	{
		# ifdef YUNI_OS_MSVC
		return (YUNI_BIT_GET(data, index)) ? true : false;
		# else
		return YUNI_BIT_GET(data, index);
		# endif
	}


	inline void Set(char* data, uint index)
	{
		//data[index >> 3] |= 1 << ((ENDIANESS) ? (7 - (i) & 7) : (i) & 7);
		YUNI_BIT_SET(data, index);
	}


	inline void Unset(char* data, uint index)
	{
		//data[index >> 3] ~= (1 << ((ENDIANESS) ? (7 - (index) & 7) : (index) & 7));
		YUNI_BIT_UNSET(data, index);
	}




} // namespace Bit
} // namespace Yuni

