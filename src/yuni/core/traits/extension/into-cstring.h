#ifndef __YUNI_CORE_TRAITS_EXTENSION_INTO_CSTRING_H__
# define __YUNI_CORE_TRAITS_EXTENSION_INTO_CSTRING_H__

# include "../../../yuni.h"
# include <string>
# include "../../smartptr.h"
# include "../cstring.h"


namespace Yuni
{
namespace Extension
{


	template<>
	class IntoCString<char*>
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const char* const container)
		{
			return container; /* identity */
		}
	};


	template<int N>
	class IntoCString<char[N]>
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const char* container)
		{
			return container; /* identity */
		}
	};


	template<uint ChunkSizeT, bool ExpandableT>
	class IntoCString<Yuni::CString<ChunkSizeT, ExpandableT> >
	{
	public:
		typedef Yuni::CString<ChunkSizeT, ExpandableT> CStringType;
		enum { valid = 1, converted = 0, zeroTerminated = CStringType::zeroTerminated, };

	public:
		static const char* Perform(const CStringType& container)
		{
			return container.c_str();
		}
	};

	template<uint ChunkSizeT, bool ExpandableT,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	class IntoCString<Yuni::SmartPtr<Yuni::CString<ChunkSizeT, ExpandableT>, OwspP,ChckP,ConvP,StorP,ConsP> >
	{
	public:
		typedef Yuni::CString<ChunkSizeT, ExpandableT> CStringType;
		typedef Yuni::SmartPtr<Yuni::CString<ChunkSizeT, ExpandableT>, OwspP,ChckP,ConvP,StorP,ConsP> CStringTypePtr;
		enum { valid = 1, converted = 0, zeroTerminated = CStringType::zeroTerminated, };

	public:
		static const char* Perform(const CStringTypePtr& container)
		{
			return (!container) ? NULL : container->c_str();
		}
	};



	template<uint ChunkSizeT, bool ExpandableT>
	class IntoCString<Yuni::CString<ChunkSizeT, ExpandableT>* >
	{
	public:
		typedef typename Yuni::CString<ChunkSizeT, ExpandableT> CStringType;
		enum { valid = 1, converted = 0, zeroTerminated = CStringType::zeroTerminated, };

	public:
		static const char* Perform(const CStringType* const container)
		{
			return container ? container->data() : NULL;
		}
	};



	template<class T, class Alloc>
	class IntoCString<std::basic_string<char,T,Alloc> >
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const std::basic_string<char,T,Alloc>& container)
		{
			return container.c_str();
		}
	};


	template<class T, class Alloc,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	class IntoCString<Yuni::SmartPtr<std::basic_string<char, T,Alloc>, OwspP,ChckP,ConvP,StorP,ConsP> >
	{
	public:
		typedef std::basic_string<char, T,Alloc> StringType;
		typedef Yuni::SmartPtr<std::basic_string<char,T,Alloc>, OwspP,ChckP,ConvP,StorP,ConsP> StringTypePtr;
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const StringTypePtr& container)
		{
			return (!container) ? NULL : container->c_str();
		}
	};



	template<class T, class Alloc>
	class IntoCString<std::basic_string<char,T,Alloc>* >
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const std::basic_string<char,T,Alloc>* const container)
		{
			return container ? container->c_str() : NULL;
		}
	};


	template<>
	class IntoCString<YuniNullPtr>
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const YuniNullPtr&)
		{
			return NULL;
		}
	};






} // namespace Extension
} // namespace Yuni

#endif // __YUNI_CORE_TRAITS_EXTENSION_INTO_CSTRING_H__
