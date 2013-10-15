#ifndef __YUNI_CORE_BIT_BIT_ARRAY_HXX__
# define __YUNI_CORE_BIT_BIT_ARRAY_HXX__


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
		(void)::memset(pBuffer.data(), 0, (size_t)pBuffer.sizeInBytes());
	}


	inline void Array::reset()
	{
		(void)::memset(pBuffer.data(), 0, (size_t)pBuffer.sizeInBytes());
	}


	inline void Array::reset(bool value)
	{
		(void)::memset(pBuffer.data(), (value ? 0xFF : 0), (size_t)pBuffer.sizeInBytes());
	}


	inline void Array::set(uint i)
	{
		YUNI_BIT_SET(pBuffer.data(), i);
	}


	inline bool Array::get(uint i) const
	{
		// note: true/false for Visual Studio Warning
		return (YUNI_BIT_GET(pBuffer.data(), i)) ? true : false;
	}


	inline bool Array::test(uint i) const
	{
		// note: true/false for Visual Studio Warning
		return (YUNI_BIT_GET(pBuffer.data(), i)) ? true : false;
	}


	inline void Array::set(uint i, bool value)
	{
		if (value)
			YUNI_BIT_SET(pBuffer.data(), i);
		else
			YUNI_BIT_UNSET(pBuffer.data(), i);
	}


	inline void Array::unset(uint i)
	{
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
		return (uint) pBuffer.sizeInBytes();
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


	template<class StringT>
	inline void Array::loadFromBuffer(const StringT& u)
	{
		// Assert, if a C* container can not be found at compile time
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, BitArray_InvalidTypeForBuffer);
		// Assert, if the length of the container can not be found at compile time
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  BitArray_InvalidTypeForBufferSize);

		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a C* for looking for a single char.

			// The value to find is actually empty, nothing to do
			if (0 == Traits::Length<StringT,Size>::fixedLength)
			{
				pBuffer.clear();
				pCount = 0;
				return;
			}
		}
		pCount = Traits::Length<StringT,Size>::Value(u);
		pBuffer.assign(Traits::CString<StringT>::Perform(u), pCount);
	}


	template<class StringT>
	inline void Array::loadFromBuffer(const StringT& u, uint size)
	{
		// Assert, if a C* container can not be found at compile time
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, BitArray_InvalidTypeForBuffer);

		pCount = size;
		pBuffer.assign(Traits::CString<StringT>::Perform(u), pCount);
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


	template<class StringT> inline Array& Array::operator = (const StringT& rhs)
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


	template<bool ValueT>
	inline uint Array::find(uint offset) const
	{
		return ValueT ? findFirstSet(offset) : findFirstUnset(offset);
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


#endif // __YUNI_CORE_BIT_BIT_ARRAY_HXX__
