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
#include "../../../yuni.h"
#include <string>
#include <cstring>
#include "../../smartptr.h"
#include "../length.h"



namespace Yuni
{
namespace Extension
{


	// C{N}
	template<int N, class SizeT>
	class Length<char[N], SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	public:
		static SizeT Value(const char* const container)
		{
			// This value can not really be known at compile time
			// We may encounter literal strings :
			// "abc" -> N = 4 but the real length is 3
			// or a static buffer  char v[42] where the real length is 42
			return (N == 0) ? 0 : ('\0' == container[N-1] ? N-1 : N);
		}
	};


	// A mere CString (zero-terminated)
	template<class SizeT>
	class Length<char*, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	public:
		static SizeT Value(const char* const container)
		{
			return container ? (SizeT)::strlen(container) : 0;
		}
	};


	// A mere wide string (zero-terminated)
	template<class SizeT>
	class Length<wchar_t*, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	public:
		static SizeT Value(const wchar_t* const container)
		{
			return container ? (SizeT)::wcslen(container) : 0;
		}
	};



	// single char

	template<class SizeT>
	class Length<char, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 1, fixedLength = 1, };

	public:
		static SizeT Value(const char) {return (SizeT) 1;}
	};


	// A single wide char
	template<class SizeT>
	class Length<wchar_t, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 1, fixedLength = 1, };

	public:
		static SizeT Value(const wchar_t) {return (SizeT) 2;}
	};





	// CString
	template<uint ChunkSizeT, bool ExpandableT, class SizeT>
	class Length<Yuni::CString<ChunkSizeT, ExpandableT>, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	private:
		typedef Yuni::CString<ChunkSizeT, ExpandableT> CStringType;

	public:
		static SizeT Value(const CStringType& container)
		{
			return (SizeT) container.size();
		}
	};


	template<uint ChunkSizeT, bool ExpandableT,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP, class SizeT>
	class Length<Yuni::SmartPtr<Yuni::CString<ChunkSizeT, ExpandableT>, OwspP, ChckP, ConvP, StorP, ConsP>, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	private:
		typedef Yuni::CString<ChunkSizeT, ExpandableT> CStringType;
		typedef Yuni::SmartPtr<Yuni::CString<ChunkSizeT, ExpandableT>, OwspP,ChckP,ConvP,StorP,ConsP> CStringTypePtr;

	public:
		static SizeT Value(const CStringTypePtr& container)
		{
			return (!container) ? 0 : (SizeT) container->size();
		}
	};


	template<uint ChunkSizeT, bool ExpandableT, class SizeT>
	class Length<Yuni::CString<ChunkSizeT, ExpandableT>*, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	public:
		static SizeT Value(const Yuni::CString<ChunkSizeT, ExpandableT>* const container)
		{
			return (container) ? (SizeT) container->size() : 0;
		}
	};



	// std::string
	template<class C, class T, class Alloc, class SizeT>
	class Length<std::basic_string<C, T, Alloc>, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	private:
		typedef std::basic_string<C,T,Alloc> StringType;

	public:
		static SizeT Value(const StringType& container)
		{
			return (SizeT)container.size();
		}
	};


	template<class C, class T, class Alloc, class SizeT,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	class Length<Yuni::SmartPtr<std::basic_string<C, T, Alloc>, OwspP, ChckP, ConvP, StorP, ConsP>, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	private:
		typedef std::basic_string<C,T,Alloc> StringType;
		typedef Yuni::SmartPtr<std::basic_string<C,T,Alloc>,OwspP,ChckP,ConvP,StorP,ConsP> StringTypePtr;

	public:
		static SizeT Value(const StringTypePtr& container)
		{
			return (!container) ? 0 : (SizeT) container->size();
		}
	};


	template<class C, class T, class Alloc, class SizeT>
	class Length<std::basic_string<C, T, Alloc>*, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 0, fixedLength = 0, };

	private:
		typedef std::basic_string<C,T,Alloc> StringType;

	public:
		static SizeT Value(const StringType* const container)
		{
			return container ? (SizeT) container->size() : 0;
		}
	};




	// nulptr

	template<class SizeT>
	class Length<YuniNullPtr, SizeT> final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 1, isFixed = 1, fixedLength = 1, };

	public:
		static SizeT Value(const YuniNullPtr&)
		{
			return 0;
		}
	};




} // namespace Extension
} // namespace Yuni
