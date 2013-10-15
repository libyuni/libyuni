#ifndef __YUNI_CORE_STRING_TRAITS_TRAITS_H__
# define __YUNI_CORE_STRING_TRAITS_TRAITS_H__

# include "../../../yuni.h"
# ifdef YUNI_HAS_STDLIB_H
#	include <stdlib.h>
# endif
# include <string.h>
# include "../../math/math.h"



namespace Yuni
{
namespace Private
{
namespace CStringImpl
{

	// Const qualifier from the adapter mode
	template<bool AdapterT, class C> struct QualifierFromAdapterMode { typedef C* Type; };
	template<class C> struct QualifierFromAdapterMode<true, C> { typedef const C* Type; };



	int Compare(const char* const s1, uint l1, const char* const s2, uint l2);
	int CompareInsensitive(const char* const s1, uint l1, const char* const s2, uint l2);

	bool Equals(const char* const s1, const char* const s2, uint len);
	bool EqualsInsensitive(const char* const s1, const char* const s2, uint len);

	bool Glob(const char* const s, uint l1, const char* const pattern, uint patternlen);



	template<bool AdapterT>
	struct FinalZero
	{
		static inline void Set(const char*, uint) {}
	};

	template<>
	struct FinalZero<false>
	{
		static inline void Set(char* buffer, uint offset)
		{
			buffer[offset] = '\0';
		}
	};



	template<class StringT, bool Adapter>
	struct AdapterAssign
	{
		static inline void Perform(StringT& out, const char* const cstring, typename StringT::Size size)
		{
			out.adaptWithoutChecking(cstring, size);
		}
	};

	template<class StringT>
	struct AdapterAssign<StringT, false>
	{
		static inline void Perform(StringT& out, const char* const cstring, typename StringT::Size size)
		{
			// fallback
			out.assign(cstring, size);
		}
	};


	template<class StringT, bool Adapter>
	struct Consume
	{
		static inline void Perform(StringT& out, typename StringT::Size count)
		{
			if (count >= out.size())
				out.clear();
			else
				out.decalOffset(count);
		}
	};

	template<class StringT>
	struct Consume<StringT, false>
	{
		static inline void Perform(StringT& out, typename StringT::Size count)
		{
			out.erase(0, count);
		}
	};







	template<uint ChunkSizeT, bool ExpandableT>
	struct Data
	{
	public:
		typedef char C;
		typedef uint Size;
		enum
		{
			chunkSize = ChunkSizeT,
			zeroTerminated = 1, //(ZeroTerminatedT ? 1 : 0),
			expandable = 1,
			adapter  = (!chunkSize and expandable and !(uint) zeroTerminated),
		};

	public:
		//! \name Constructors & Destructors
		//@{
		/*!
		** \brief Default Constructor
		*/
		Data();

		/*!
		** \brief Copy constructor
		*/
		Data(const Data& rhs);

		//! Destructor
		~Data();
		//@}

		void adapt(const char* const cstring);

		void adapt(const char* const cstring, Size length);

		void clear();


		Size assignWithoutChecking(const C* const block, const Size blockSize)
		{
			// Making sure that we have enough space
			reserve(blockSize + (uint) zeroTerminated);
			// Raw copy
			YUNI_MEMCPY(const_cast<char*>(data), (uint) capacity, block, sizeof(C) * blockSize);
			// New size
			size = blockSize;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
			return blockSize;
		}


		Size appendWithoutChecking(const C* const block, const Size blockSize)
		{
			// Making sure that we have enough space
			reserve(size + blockSize + (uint) zeroTerminated);
			// Raw copy
			YUNI_MEMCPY(const_cast<char*>(data) + size * sizeof(C), (uint) capacity, block, blockSize * sizeof(C));
			// New size
			size += blockSize;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
			return blockSize;
		}

		Size assignWithoutChecking(const C c)
		{
			// Making sure that we have enough space
			reserve(1 + (uint) zeroTerminated);
			// Raw copy
			(const_cast<char*>(data))[0] = c;
			// New size
			size = 1;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[1] = C();
			return 1;
		}


		Size appendWithoutChecking(const C c)
		{
			// Making sure that we have enough space
			reserve(size + 1 + (uint) zeroTerminated);
			// Raw copy
			(const_cast<char*>(data))[size] = c;
			// New size
			++size;
			if ((uint) zeroTerminated)
				(const_cast<char*>(data))[size] = C();
			return 1;
		}


		Size assign(const C* const block, const Size blockSize)
		{
			if (block and blockSize)
				return assignWithoutChecking(block, blockSize);
			clear();
			return 0;
		}


		Size append(const C* const block, const Size blockSize)
		{
			return (block and blockSize) ? appendWithoutChecking(block, blockSize) : 0;
		}

		void put(const C rhs);

		void reserve(Size mincapacity);

		void insert(Size offset, const C* const buffer, const Size len);

		void shrink();


	protected:
		Size size;
		Size capacity;
		//! Our buffer
		typename QualifierFromAdapterMode<(0 != adapter), C>::Type data;
		// Friend
		template<uint SizeT, bool ExpT> friend class Yuni::CString;

	}; // class Data





	template<uint ChunkSizeT>
	struct Data<ChunkSizeT, false>
	{
	public:
		typedef char C;
		typedef uint Size;
		enum
		{
			chunkSize = ChunkSizeT,
			capacity = ChunkSizeT,
			zeroTerminated = 1, // (ZeroTerminatedT ? 1 : 0),
			expandable = 0,
		};

	public:
		Data() :
			size(0)
		{
			// The buffer must be properly initialized
			if ((uint) zeroTerminated)
				data[0] = C();
		}

		Data(const Data& rhs) :
			size(rhs.size)
		{
			YUNI_MEMCPY(data, (uint) capacity, rhs.data, sizeof(C) * (size + (uint) zeroTerminated));
		}

		void clear()
		{
			size = 0;
			if ((uint) zeroTerminated)
				data[0] = C();
		}

		Size assignWithoutChecking(const C* const block, Size blockSize);

		Size appendWithoutChecking(const C* const block, Size blockSize);

		Size assignWithoutChecking(const C c);

		Size appendWithoutChecking(const C c);


		Size assign(const C* const block, const Size blockSize)
		{
			if (block and blockSize)
				return assignWithoutChecking(block, blockSize);
			clear();
			return 0;
		}

		Size append(const C* const block, const Size blockSize)
		{
			return (block and blockSize) ? appendWithoutChecking(block, blockSize) : 0;
		}

		void put(const C rhs);

		static void reserve(const Size)
		{
			// Do nothing
		}


		void insert(Size offset, const C* const buffer, Size len)
		{
			if (offset + len >= (uint) capacity)
			{
				// The new buffer will take the whole space
				YUNI_MEMCPY(data + sizeof(C) * (offset), (uint) capacity - offset, buffer,
					sizeof(C) * ((uint) capacity - offset));
				size = (uint) capacity;
				if ((uint) zeroTerminated)
					data[(uint) capacity] = C();
				return;
			}
			if (size + len <= (uint) capacity)
			{
				// Move the existing block of data
				(void)::memmove(data + sizeof(C) * (offset + len), data + sizeof(C) * (offset),
					sizeof(C) * (size - offset));
				// Copying the given buffer
				YUNI_MEMCPY (data + sizeof(C) * (offset), (uint) capacity, buffer, sizeof(C) * len);
				// Updating the size
				size += len;
				// zero-terminated
				if ((uint) zeroTerminated)
					data[size] = C();
			}
			else
			{
				// Move the existing block of data
				(void)::memmove(data + sizeof(C) * (offset + len), data + sizeof(C) * (offset),
					sizeof(C) * ((uint) capacity - offset - len));
				// Copying the given buffer
				YUNI_MEMCPY(data + sizeof(C) * (offset), (uint) capacity, buffer, sizeof(C) * len);
				// Updating the size
				size = (uint) capacity;
				// zero-terminated
				if ((uint) zeroTerminated)
					data[(uint) capacity] = C();
			}
		}

		static void shrink()
		{
			// Do nothing
		}

	protected:
		Size size;
		/*!
		** \brief Static buffer, fixed (uint) capacity
		**
		** We always add +1 to allow standard routine write a final 0, and we
		** may need it for zero-terminated strings any way.
		*/
		C data[(uint) capacity + 1];

		// Friend
		template<uint SizeT, bool ExpT> friend class Yuni::CString;

	}; // class Data;







} // namespace CStringImpl
} // namespace Private
} // namespace Yuni

# include "traits.hxx"

#endif // __YUNI_CORE_STRING_TRAITS_TRAITS_H__
