#ifndef __YUNI_CORE_BIT_BIT_ARRAY_HXX__
# define __YUNI_CORE_BIT_BIT_ARRAY_HXX__

# include <string.h> // memset
# include <cassert>



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


#endif // __YUNI_CORE_BIT_BIT_ARRAY_HXX__
