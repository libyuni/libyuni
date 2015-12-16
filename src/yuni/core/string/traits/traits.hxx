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
#include "traits.h"



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
				capacity += static_cast<uint>(zeroTerminated);
				data = reinterpret_cast<C*>(::malloc(sizeof(C) * static_cast<uint>(capacity)));
				YUNI_MEMCPY(const_cast<void*>(static_cast<const void*>(data)), static_cast<uint>(capacity), rhs.data, sizeof(C) * size);
				if (static_cast<uint>(zeroTerminated))
					(const_cast<char*>(data))[size] = C();
			}
			else
			{
				// this string is a string adapter
				data = rhs.data;
			}
		}
	}


	# ifdef YUNI_HAS_CPP_MOVE
	template<uint ChunkSizeT, bool ExpandableT>
	inline Data<ChunkSizeT,ExpandableT>::Data(Data&& rhs) :
		size(rhs.size),
		capacity(rhs.size),
		data(rhs.data)
	{
		rhs.size = 0;
		rhs.capacity = 0;
		rhs.data = nullptr;
	}
	# endif


	template<uint ChunkSizeT, bool ExpandableT>
	inline Data<ChunkSizeT,ExpandableT>::~Data()
	{
		// Release the internal buffer if allocated
		// The string is a string adapter only if the chunk size if null
		// When the string is an adapter, the variable is const
		if (chunkSize != 0)
			::free(const_cast<void*>(static_cast<const void*>(data)));
	}


	# ifdef YUNI_HAS_CPP_MOVE
	template<uint ChunkSizeT, bool ExpandableT>
	inline Data<ChunkSizeT,ExpandableT>& Data<ChunkSizeT,ExpandableT>::operator = (Data&& rhs)
	{
		// Release the internal buffer if allocated
		// The string is a string adapter only if the chunk size if null
		// When the string is an adapter, the variable is const
		if (chunkSize != 0)
			::free(const_cast<void*>(static_cast<const void*>(data)));

		size = rhs.size;
		capacity = rhs.capacity;
		data = rhs.data;

		rhs.size = 0;
		rhs.capacity = 0;
		rhs.data = nullptr;
		return *this;
	}
	# endif


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::adapt(const char* const cstring)
	{
		data = const_cast<char*>(cstring);
		capacity = size = (data ? static_cast<Size>(::strlen(data)) : 0);
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
		if (static_cast<uint>(zeroTerminated))
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
				capacity = size + static_cast<uint>(zeroTerminated);
				data = reinterpret_cast<char*>(::realloc(const_cast<char*>(data), static_cast<uint>(capacity)));
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
		reserve(len + size + static_cast<uint>(zeroTerminated));
		// Move the existing block of data
		(void)::memmove(const_cast<char*>(data) + sizeof(C) * (offset + len),
			const_cast<char*>(data) + sizeof(C) * (offset), sizeof(C) * (size - offset));
		// Copying the given buffer
		YUNI_MEMCPY(const_cast<char*>(data) + sizeof(C) * (offset), static_cast<uint>(capacity), buffer, sizeof(C) * len);
		// Updating the size
		size += len;
		// zero-terminated
		if (static_cast<uint>(zeroTerminated))
			(const_cast<char*>(data))[size] = C();
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline void
	Data<ChunkSizeT,ExpandableT>::put(const C rhs)
	{
		// Making sure that we have enough space
		reserve(size + 1 + static_cast<uint>(zeroTerminated));
		// Raw copy
		(const_cast<char*>(data))[size] = rhs;
		// New size
		++size;
		if (static_cast<uint>(zeroTerminated))
			(const_cast<char*>(data))[size] = C();
	}


	template<uint ChunkSizeT, bool ExpandableT>
	void
	Data<ChunkSizeT,ExpandableT>::reserve(Size mincapacity)
	{
		if (adapter)
			return;
		mincapacity += static_cast<uint>(zeroTerminated);
		if (static_cast<uint>(capacity) < mincapacity)
		{
			// This loop may be a little faster than the following replacement code :
			// static_cast<uint>(capacity) = minstatic_cast<uint>(capacity) + minstatic_cast<uint>(capacity) % chunkSize;
			// Especially when chunkSize is not a power of 2
			Size newcapacity = static_cast<uint>(capacity);
			do
			{
				// Increase the static_cast<uint>(capacity) until we have enough space
				newcapacity += chunkSize;
			}
			while (newcapacity < mincapacity);

			// Realloc the internal buffer
			C* newdata = reinterpret_cast<C*>(::realloc(const_cast<char*>(data), (sizeof(C) * newcapacity)));
			// The returned value can be NULL
			if (!newdata)
				throw "Yuni::CString: Impossible to realloc";
			{
				data = newdata;
				capacity = newcapacity;
				if (static_cast<uint>(zeroTerminated))
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
		// blockSize = Math::Min<Size>(blockSize, static_cast<uint>(capacity) - size);
		//
		// We have better performance if the two cases have their own code block
		// (perhaps because of the constant values)
		//
		if (blockSize > static_cast<uint>(capacity))
		{
			// The new blocksize is actually the static_cast<uint>(capacity) itself
			// The static_cast<uint>(capacity) can not be null
			// Raw copy
			YUNI_MEMCPY(const_cast<char*>(data), static_cast<uint>(capacity), block, sizeof(C) * static_cast<uint>(capacity));
			// New size
			size = static_cast<uint>(capacity);
			if (static_cast<uint>(zeroTerminated))
				(const_cast<char*>(data))[static_cast<uint>(capacity)] = C();
			return static_cast<uint>(capacity);
		}
		// else
		{
			// Raw copy
			YUNI_MEMCPY(const_cast<char*>(data), static_cast<uint>(capacity), block, sizeof(C) * blockSize);
			// New size
			size = blockSize;
			if (static_cast<uint>(zeroTerminated))
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
		// blockSize = Math::Min<Size>(blockSize, static_cast<uint>(capacity) - size);
		//
		// We have better performance if the two cases have their own code block
		// (perhaps because of the constant values)
		//
		if (blockSize + size > static_cast<uint>(capacity))
		{
			// Computing the new block size to copy
			blockSize = static_cast<uint>(capacity) - size;
			// The performance are a bit better if we interupt the process sooner
			if (!blockSize)
				return 0;
			// Raw copy
			YUNI_MEMCPY(data + size * sizeof(C), static_cast<uint>(capacity), block, sizeof(C) * blockSize);
			// New size
			size = static_cast<uint>(capacity);
			if (static_cast<uint>(zeroTerminated))
				(const_cast<char*>(data))[static_cast<uint>(capacity)] = C();
			return blockSize;
		}
		// else
		{
			// Raw copy
			YUNI_MEMCPY(data + size * sizeof(C), static_cast<uint>(capacity), block, sizeof(C) * blockSize);
			// New size
			size += blockSize;
			if (static_cast<uint>(zeroTerminated))
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
		if (static_cast<uint>(zeroTerminated))
			(const_cast<char*>(data))[1] = C();
		return 1;
	}


	template<uint ChunkSizeT>
	typename Data<ChunkSizeT,false>::Size
	Data<ChunkSizeT,false>::appendWithoutChecking(const C c)
	{
		if (YUNI_UNLIKELY(size == static_cast<uint>(capacity)))
			return 0;

		data[size] = c;
		++size;
		if (static_cast<uint>(zeroTerminated))
			(const_cast<char*>(data))[size] = C();
		return 1;
	}


	template<uint ChunkSizeT>
	void
	Data<ChunkSizeT,false>::put(const C rhs)
	{
		// Making sure that we have enough space
		if (size != static_cast<uint>(capacity))
		{
			// Raw copy
			data[size] = rhs;
			// New size
			++size;
			if (static_cast<uint>(zeroTerminated))
				(const_cast<char*>(data))[size] = C();
		}
	}





} // namespace CStringImpl
} // namespace Private
} // namespace Yuni
