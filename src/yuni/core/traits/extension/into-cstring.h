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
#include "../../smartptr.h"
#include "../cstring.h"



namespace Yuni
{
namespace Extension
{


	template<>
	class IntoCString<char*> final
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
	class IntoCString<char[N]> final
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
	class IntoCString<Yuni::CString<ChunkSizeT, ExpandableT> > final
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
	class IntoCString<Yuni::SmartPtr<Yuni::CString<ChunkSizeT, ExpandableT>, OwspP, ChckP, ConvP, StorP, ConsP> > final
	{
	public:
		typedef Yuni::CString<ChunkSizeT, ExpandableT> CStringType;
		typedef Yuni::SmartPtr<Yuni::CString<ChunkSizeT, ExpandableT>, OwspP,ChckP,ConvP,StorP,ConsP> CStringTypePtr;
		enum { valid = 1, converted = 0, zeroTerminated = CStringType::zeroTerminated, };

	public:
		static const char* Perform(const CStringTypePtr& container)
		{
			return (!container) ? nullptr : container->c_str();
		}
	};



	template<uint ChunkSizeT, bool ExpandableT>
	class IntoCString<Yuni::CString<ChunkSizeT, ExpandableT>* > final
	{
	public:
		typedef typename Yuni::CString<ChunkSizeT, ExpandableT> CStringType;
		enum { valid = 1, converted = 0, zeroTerminated = CStringType::zeroTerminated, };

	public:
		static const char* Perform(const CStringType* const container)
		{
			return container ? container->data() : nullptr;
		}
	};



	template<class T, class Alloc>
	class IntoCString<std::basic_string<char, T, Alloc> > final
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
	class IntoCString<Yuni::SmartPtr<std::basic_string<char, T, Alloc>, OwspP, ChckP, ConvP, StorP, ConsP> > final
	{
	public:
		typedef std::basic_string<char, T,Alloc> StringType;
		typedef Yuni::SmartPtr<std::basic_string<char,T,Alloc>, OwspP,ChckP,ConvP,StorP,ConsP> StringTypePtr;
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const StringTypePtr& container)
		{
			return (!container) ? nullptr : container->c_str();
		}
	};



	template<class T, class Alloc>
	class IntoCString<std::basic_string<char, T, Alloc>* > final
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const std::basic_string<char,T,Alloc>* const container)
		{
			return container ? container->c_str() : nullptr;
		}
	};


	template<>
	class IntoCString<YuniNullPtr> final
	{
	public:
		enum { valid = 1, converted = 0, zeroTerminated = 1, };

	public:
		static const char* Perform(const YuniNullPtr&)
		{
			return nullptr;
		}
	};






} // namespace Extension
} // namespace Yuni
