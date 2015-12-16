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
#include "utf8char.h"



namespace Yuni
{
namespace UTF8
{

	inline Char::Char()
		: pValue()
	{}


	inline Char::Char(const Char& rhs)
		: pValue(rhs.pValue)
	{}


	inline Char::Char(uint c)
		: pValue(c)
	{}


	inline uint32 Char::value() const
	{
		return pValue;
	}


	inline void Char::reset()
	{
		pValue  = 0;
	}


	inline Char& Char::operator = (const Char& rhs)
	{
		pValue = rhs.pValue;
		return *this;
	}


	inline Char& Char::operator = (uint value)
	{
		pValue = static_cast<uint32>(value);
		return *this;
	}


	inline bool Char::operator == (uint value) const
	{
		return pValue == value;
	}


	inline bool Char::operator == (const Char& ch) const
	{
		return pValue == ch.pValue;
	}


	inline bool Char::operator == (const AnyString& str) const
	{
		return str == *this;
	}


	inline bool Char::operator != (uint value) const
	{
		return pValue != value;
	}


	inline bool Char::operator != (const Char& ch) const
	{
		return pValue != ch.pValue;
	}


	inline bool Char::operator != (const AnyString& str) const
	{
		return str != *this;
	}


	inline Char::operator char () const
	{
		return (pValue < 0x80) ? static_cast<char>(pValue) : '\0';
	}


	inline Char::operator unsigned char () const
	{
		return (pValue < 0x80) ? static_cast<uchar>(pValue) : static_cast<uchar>('\0');
	}


	inline uchar Char::Mask8Bits(const void* p)
	{
		return static_cast<uchar>(0xFF & *(static_cast<const char*>(p)));
	}

	inline uchar Char::Mask8Bits(const char p)
	{
		return static_cast<uchar>(0xFF & p);
	}


	inline uint Char::Size(const void* p)
	{
		// Char. number range  |        UTF-8 octet sequence
		//    (hexadecimal)    |              (binary)
		// --------------------+------------------------------------
		// 0000 0000-0000 007F | 0xxxxxxx
		// 0000 0080-0000 07FF | 110xxxxx 10xxxxxx
		// 0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
		// 0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

		const uchar lead = Mask8Bits(p);
		if (lead < 0x80) // the leas byte is zero, ascii
			return 1;
		if ((lead >> 5) == 0x6)  // 110x xxxx
			return 2;
		if ((lead >> 4) == 0xe)  // 1110 xxxx
			return 3;
		if ((lead >> 3) == 0x1e) // 1111 0xxx
			return 4;
		return 0;
	}


	inline uint Char::size() const
	{
		if (pValue < 0x80)
			return 1;
		if (pValue < 0x800)
			return 2;
		if (pValue < 0x10000)
			return 3;
		return 4;
	}


	inline bool Char::IsASCII(uchar c)
	{
		return c < 0x80;
	}


	inline bool Char::IsTrail(uchar c)
	{
		return ((c >> 6) == 0x2);
	}


	template<class StreamT>
	void Char::write(StreamT& out) const
	{
		if (pValue < 0x80)
		{
			out.put(static_cast<char>(static_cast<uchar>(pValue)));
		}
		else
		{
			if (pValue < 0x800)
			{
				out.put(static_cast<char>(static_cast<uchar>((pValue >> 6)   | 0xc0)));
				out.put(static_cast<char>(static_cast<uchar>((pValue & 0x3f) | 0x80)));
			}
			else
			{
				if (pValue < 0x10000)
				{
					out.put(static_cast<char>(static_cast<uchar>((pValue >> 12)         | 0xe0)));
					out.put(static_cast<char>(static_cast<uchar>(((pValue >> 6) & 0x3f) | 0x80)));
					out.put(static_cast<char>(static_cast<uchar>((pValue & 0x3f)        | 0x80)));
				}
				else
				{
					// four bytes
					out.put(static_cast<char>(static_cast<uchar>((pValue >> 18)         | 0xf0)));
					out.put(static_cast<char>(static_cast<uchar>(((pValue >> 12)& 0x3f) | 0x80)));
					out.put(static_cast<char>(static_cast<uchar>(((pValue >> 6) & 0x3f) | 0x80)));
					out.put(static_cast<char>(static_cast<uchar>((pValue & 0x3f)        | 0x80)));
				}
			}
		}
	}


	inline bool Char::isAscii() const
	{
		return pValue < static_cast<uint>(asciiLimit);
	}


	inline bool Char::operator < (uint value) const
	{
		return pValue < value;
	}


	inline bool Char::operator <= (uint value) const
	{
		return pValue <= value;
	}


	inline bool Char::operator > (uint value) const
	{
		return pValue > value;
	}


	inline bool Char::operator >= (uint value) const
	{
		return pValue >= value;
	}




	inline bool Char::operator < (const Char& ch) const
	{
		return pValue < ch.pValue;
	}


	inline bool Char::operator <= (const Char& ch) const
	{
		return pValue <= ch.pValue;
	}


	inline bool Char::operator > (const Char& ch) const
	{
		return pValue > ch.pValue;
	}


	inline bool Char::operator >= (const Char& ch) const
	{
		return pValue >= ch.pValue;
	}




} // namespace UTF8
} // namespace Yuni



#ifdef YUNI_HAS_STL_HASH

namespace std
{
	template<>
	struct YUNI_DECL hash<Yuni::UTF8::Char> final
	{
		inline size_t operator() (const Yuni::UTF8::Char& ch) const
		{
			return ch.value();
		}
	};

} // namespace std

#endif // YUNI_HAS_STL_HASH
