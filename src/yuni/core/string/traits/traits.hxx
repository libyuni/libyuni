#ifndef __YUNI_CORE_STRING_TRAITS_TRAITS_HXX__
# define __YUNI_CORE_STRING_TRAITS_TRAITS_HXX__



namespace Yuni
{
namespace Private
{
namespace CStringImpl
{

	template<uint ChunkSizeT, bool ExpandableT>
	inline Data<ChunkSizeT,ExpandableT>::Data() :
		size(),
		capacity(),
		data(NULL)
	{}


	template<uint ChunkSizeT, bool ExpandableT>
	Data<ChunkSizeT,ExpandableT>::Data(const Data<ChunkSizeT,ExpandableT>& rhs) :
		size(rhs.size),
		capacity(rhs.size),
		data(NULL)
	{
		if (size)
		{
			if (chunkSize != 0)
			{
				capacity += (uint) zeroTerminated;
				data = (C*)::malloc(sizeof(C) * (uint) capacity);
				YUNI_MEMCPY(const_cast<void*>(static_cast<const void*>(data)), (uint) capacity, rhs.data, sizeof(C) * size);
				if ((uint) zeroTerminated)
					(const_cast<char*>(data))[size] = C();
			}
			else
			{
				// this string is a string adapter
				data = rhs.data;
			}
		}
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline Data<ChunkSizeT,ExpandableT>::~Data()
	{
		// Release the internal buffer if allocated
		// The string is a string adapter only if the chunk size if null
		// When the string is an adapter, the variable is const
		if (chunkSize != 0)
			::free(const_cast<void*>(static_cast<const void*>(data)));
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::adapt(const char* const cstring)
	{
		data = const_cast<char*>(cstring);
		capacity = size = (data ? (Size)::strlen(data) : 0);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::adapt(const char* const cstring, Size length)
	{
		data = const_cast<char*>(cstring);
		capacity = size = length;
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::clear()
	{
		if ((uint) zeroTerminated)
		{
			if (size)
			{
				size = 0;
				*(const_cast<char*>(data)) = C();
			}
		}
		else
			size = 0;
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::shrink()
	{
		if (data)
		{
			if (size)
			{
				capacity = size + (uint) zeroTerminated;
				data = (char*)::realloc(const_cast<char*>(data), (uint) capacity);
			}
			else
			{
				capacity = 0;
				::free(const_cast<void*>(static_cast<const void*>(data)));
				data = nullptr;
			}
		}
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::insert(Size offset, const C* const buffer, const Size len)
	{
		// Reserving enough space to insert the buffer
		reserve(len + size + (uint) zeroTerminated);
		// Move the existing block of data
		(void)::memmove(const_cast<char*>(data) + sizeof(C) * (offset + len), const_cast<char*>(data) + sizeof(C) * (offset), sizeof(C) * (size - offset));
		// Copying the given buffer
		YUNI_MEMCPY(const_cast<char*>(data) + sizeof(C) * (offset), (uint) capacity, buffer, sizeof(C) * len);
		// Updating the size
		size += len;
		// zero-terminated
		if ((uint) zeroTerminated)
			(const_cast<char*>(data))[size] = C();
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::put(const C rhs)
	{
		// Making sure that we have enough space
		reserve(size + 1 + (uint) zeroTerminated);
		// Raw copy
		(const_cast<char*>(data))[size] = rhs;
		// New size
		++size;
		if ((uint) zeroTerminated)
			(const_cast<char*>(data))[size] = C();
	}


	template<uint ChunkSizeT, bool ExpandableT>
	void
	Data<ChunkSizeT,ExpandableT>::reserve(Size mincapacity)
	{
		if (adapter)
			return;
		mincapacity += (uint) zeroTerminated;
		if ((uint) capacity < mincapacity)
		{
			// This loop may be a little faster than the following replacement code :
			// (uint) capacity = min(uint) capacity + min(uint) capacity % chunkSize;
			// Especially when chunkSize is not a power of 2
			Size newcapacity = (uint) capacity;
			do
			{
				// Increase the (uint) capacity until we have enough space
				newcapacity += chunkSize;
			}
			while (newcapacity < mincapacity);

			// Realloc the internal buffer
			C* newdata = (C*)::realloc(const_cast<char*>(data), (size_t)(sizeof(C) * newcapacity));
			// The returned value can be NULL
			if (!newdata)
				throw "Yuni::CString: Impossible to realloc";
			{
				data = newdata;
				capacity = newcapacity;
				if ((uint) zeroTerminated)
					(const_cast<char*>(data))[size] = C();
			}
		}
	}


	template<uint ChunkSizeT>
	typename Data<ChunkSizeT,false>::Size
	Data<ChunkSizeT,false>::assignWithoutChecking(const C* const block,
		Size blockSize)
	{
		// We have to trunk the size if we are outer limits
		// This condition is a little faster than the folowing replacement code :
		// blockSize = Math::Min<Size>(blockSize, (uint) capacity - size);
		//
		// We have better performance if the two cases have their own code block
		// (perhaps because of the constant values)
		//
		if (blockSize > (uint) capacity)
		{
			// The new blocksize is actually the (uint) capacity itself
			// The (uint) capacity can not be null
			// Raw copy
			YUNI_MEMCPY(const_cast<char*>(data), (uint) capacity, block, sizeof(C) * (uint) capacity);
			// New size
			size = (uint) capacity;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[(uint) capacity] = C();
			return (uint) capacity;
		}
		// else
		{
			// Raw copy
			YUNI_MEMCPY(const_cast<char*>(data), (uint) capacity, block, sizeof(C) * blockSize);
			// New size
			size = blockSize;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
			return blockSize;
		}
	}


	template<uint ChunkSizeT>
	typename Data<ChunkSizeT,false>::Size
	Data<ChunkSizeT,false>::appendWithoutChecking(const C* const block, Size blockSize)
	{
		// We have to trunk the size if we are outer limits
		// This condition is a little faster than the folowing replacement code :
		// blockSize = Math::Min<Size>(blockSize, (uint) capacity - size);
		//
		// We have better performance if the two cases have their own code block
		// (perhaps because of the constant values)
		//
		if (blockSize + size > (uint) capacity)
		{
			// Computing the new block size to copy
			blockSize = (uint) capacity - size;
			// The performance are a bit better if we interupt the process sooner
			if (!blockSize)
				return 0;
			// Raw copy
			YUNI_MEMCPY(data + size * sizeof(C), (uint) capacity, block, sizeof(C) * blockSize);
			// New size
			size = (uint) capacity;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[(uint) capacity] = C();
			return blockSize;
		}
		// else
		{
			// Raw copy
			YUNI_MEMCPY(data + size * sizeof(C), (uint) capacity, block, sizeof(C) * blockSize);
			// New size
			size += blockSize;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
			return blockSize;
		}
	}


	template<uint ChunkSizeT>
	inline typename Data<ChunkSizeT,false>::Size
	Data<ChunkSizeT,false>::assignWithoutChecking(const C c)
	{
		data[0] = c;
		size = 1;
		if ((uint) zeroTerminated)
			(const_cast<char*>(data))[1] = C();
		return 1;
	}


	template<uint ChunkSizeT>
	typename Data<ChunkSizeT,false>::Size
	Data<ChunkSizeT,false>::appendWithoutChecking(const C c)
	{
		if (size == (uint) capacity)
			return 0;
		// else
		{
			data[size] = c;
			++size;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
			return 1;
		}
	}


	template<uint ChunkSizeT>
	void
	Data<ChunkSizeT,false>::put(const C rhs)
	{
		// Making sure that we have enough space
		if (size != (uint) capacity)
		{
			// Raw copy
			data[size] = rhs;
			// New size
			++size;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
		}
	}





} // namespace CStringImpl
} // namespace Private
} // namespace Yuni

#endif // __YUNI_CORE_STRING_TRAITS_TRAITS_HXX__
