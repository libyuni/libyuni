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
#include <string.h> // memset
#include <cassert>



namespace Yuni
{
namespace Bit
{

	inline Array::Array() :
		pCount(0)
	{}


	inline Array::Array(uint n)
	{
		resize(n);
		unset();
	}


	inline Array::Array(uint n, bool value)
	{
		resize(n);
		reset(value);
	}


	inline void Array::unset()
	{
		(void)::memset(pBuffer.data(), 0, pBuffer.sizeInBytes());
	}


	inline void Array::reset()
	{
		(void)::memset(pBuffer.data(), 0, pBuffer.sizeInBytes());
	}


	inline void Array::reset(bool value)
	{
		(void)::memset(pBuffer.data(), (value ? 0xFF : 0), pBuffer.sizeInBytes());
	}


	inline void Array::set(uint i)
	{
		assert(i < pCount and "index out of range");
		YUNI_BIT_SET(pBuffer.data(), i);
	}


	inline bool Array::get(uint i) const
	{
		assert(i < pCount and "index out of range");
		// note: true/false for Visual Studio Warning
		return (YUNI_BIT_GET(pBuffer.data(), i)) ? true : false;
	}


	inline bool Array::test(uint i) const
	{
		assert(i < pCount and "index out of range");
		// note: true/false for Visual Studio Warning
		return (YUNI_BIT_GET(pBuffer.data(), i)) ? true : false;
	}


	inline void Array::set(uint i, bool value)
	{
		assert(i < pCount and "index out of range");
		if (value)
			YUNI_BIT_SET(pBuffer.data(), i);
		else
			YUNI_BIT_UNSET(pBuffer.data(), i);
	}


	inline void Array::unset(uint i)
	{
		assert(i < pCount and "index out of range");
		YUNI_BIT_UNSET(pBuffer.data(), i);
	}


	inline void Array::reserve(uint n)
	{
		pBuffer.reserve((n >> 3) + 1);
	}


	inline void Array::truncate(uint n)
	{
		pBuffer.truncate(((pCount = n) >> 3) + 1);
	}


	inline void Array::resize(uint n)
	{
		pBuffer.resize(((pCount = n) >> 3) + 1);
	}


	inline uint Array::sizeInBytes() const
	{
		return static_cast<uint>(pBuffer.sizeInBytes());
	}


	inline uint Array::size() const
	{
		return pCount;
	}


	inline uint Array::count() const
	{
		return pCount;
	}


	inline const char* Array::c_str() const
	{
		return pBuffer.c_str();
	}


	inline const char* Array::data() const
	{
		return pBuffer.data();
	}


	inline char* Array::data()
	{
		return pBuffer.data();
	}


	inline void Array::loadFromBuffer(const AnyString& buffer)
	{
		pBuffer = buffer;
		pCount  = buffer.size();
	}



	template<class AnyBufferT>
	inline void Array::saveToBuffer(AnyBufferT& u)
	{
		if (pCount)
			u.assign(pBuffer.c_str(), pBuffer.sizeInBytes());
		else
			u.clear();
	}


	inline Array& Array::operator = (const Array& rhs)
	{
		pBuffer = rhs.pBuffer;
		pCount  = rhs.pCount;
		return *this;
	}


	inline Array& Array::operator = (const AnyString& rhs)
	{
		loadFromBuffer(rhs);
		return *this;
	}


	template<class U>
	inline void Array::print(U& out) const
	{
		for (uint i = 0; i != pCount; ++i)
			out.put((YUNI_BIT_GET(pBuffer.data(), i)) ? '1' : '0');
	}


	template<bool ValueT>
	uint Array::findN(uint count, uint offset) const
	{
		while (npos != (offset = find<ValueT>(offset)))
		{
			if (offset + count > pCount)
				return npos;

			bool ok = true;

			// Checking if the block is large enough for our needs
			// The first block is already valid
			for (uint j = 1; j < count; ++j)
			{
				if (ValueT != get(offset + j))
				{
					ok = false;
					break;
				}
			}
			if (ok)
				return offset;
			++offset;
		}
		return npos;
	}





} // namespace Bit
} // namespace Yuni



//! \name Operator overload for stream printing
//@{
inline std::ostream& operator << (std::ostream& out, const Yuni::Bit::Array& rhs)
{
	rhs.print(out);
	return out;
}
//@}
