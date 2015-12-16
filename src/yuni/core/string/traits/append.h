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
#include "../../traits/length.h"
#include "integer.h"
#include <stdio.h>
#include <cassert>

#ifdef YUNI_OS_MSVC
#	define YUNI_PRIVATE_MEMBUF_SPTRINF(BUFFER,SIZE, F, V)  ::sprintf_s(BUFFER,SIZE,F,V)
#else
#	define YUNI_PRIVATE_MEMBUF_SPTRINF(BUFFER,SIZE, F, V)  ::snprintf(BUFFER,SIZE,F,V)
#endif


namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT, class C>
	class Append; // specialization required


	// C*
	template<class CStringT, class T>
	class Append<CStringT, T*> final
	{
	public:
		static void Perform(CStringT& s, const T* rhs)
		{
			s += reinterpret_cast<void*>(rhs);
		}
	};


	// char*
	template<class CStringT>
	class Append<CStringT, char*> final
	{
	public:
		typedef typename CStringT::Type TypeC;
		typedef typename Static::Remove::Const<TypeC>::Type C;
		static void Perform(CStringT& s, const C* rhs)
		{
			if (rhs)
				s.appendWithoutChecking(rhs, Yuni::Traits::Length<C*,typename CStringT::Size>::Value(rhs));
		}
	};

	// C[N]
	template<class CStringT, int N>
	class Append<CStringT, char[N]> final
	{
	public:
		typedef typename CStringT::Type C;
		static void Perform(CStringT& s, const C rhs[N])
		{
			if (N > 0 and rhs)
			{
				// The calculation with `N` is required to properly handle
				// both a zero-terminated buffer and a simple array
				s.appendWithoutChecking(rhs, N - ((rhs[N-1] == C()) ? 1 : 0));
			}
		}
	};

	// C
	template<class CStringT>
	class Append<CStringT, char> final
	{
	public:
		typedef char C;
		static void Perform(CStringT& s, const C rhs)
		{
			s.appendWithoutChecking(rhs);
		}
	};


	// C
	template<class CStringT>
	class Append<CStringT, unsigned char> final
	{
	public:
		typedef unsigned char C;
		static void Perform(CStringT& s, const C rhs)
		{
			s.appendWithoutChecking(static_cast<char>(rhs));
		}
	};

	// nullptr
	template<class CStringT>
	class Append<CStringT, YuniNullPtr> final
	{
	public:
		static void Perform(CStringT&, const YuniNullPtr&)
		{ /* Do nothing */ }
	};


	// bool
	template<class CStringT>
	class Append<CStringT, bool> final
	{
	public:
		static void Perform(CStringT& s, const bool rhs)
		{
			if (rhs)
				s.appendWithoutChecking("true", 4);
			else
				s.appendWithoutChecking("false", 5);
		}
	};


	// wchar_t
	template<class CStringT>
	class Append<CStringT, wchar_t*> final
	{
	public:
		static void Perform(CStringT& string, const wchar_t* rhs)
		{
			string.append(rhs, Yuni::Traits::Length<wchar_t*, typename CStringT::Size>::Value(rhs));
		}
	};

	// wchar_t
	template<class CStringT, int N>
	class Append<CStringT, wchar_t[N]> final
	{
	public:
		static void Perform(CStringT& string, const wchar_t rhs[N])
		{
			if (N > 0 and rhs)
				string.append(rhs, N - ((rhs[N-1] == wchar_t()) ? 1 : 0));
		}
	};


	// void*
	template<class CStringT>
	class Append<CStringT, void*> final
	{
	public:
		static void Perform(CStringT& s, const void* rhs)
		{
			if (not rhs)
			{
				s.appendWithoutChecking("0x0", 3);
			}
			else
			{
				# ifdef YUNI_OS_MSVC
				{
					// With Visual Studio, the option %p does not provide the prefix 0x
					typename CStringT::Type buffer[32];
					buffer[0] = '0';
					buffer[1] = 'x';

					// On Windows, it may return a negative value
					if (YUNI_PRIVATE_MEMBUF_SPTRINF(buffer + 2, sizeof(buffer)-2, "%p", rhs) >= 0)
					{
						s.appendWithoutChecking(buffer,
							Yuni::Traits::Length<typename CStringT::Type*, typename CStringT::Size>::Value(buffer));
					}
					else
						s.appendWithoutChecking("0x0", 3);
				}
				# else
				{
					typename CStringT::Type buffer[32];
					// On Windows, it may return a negative value
					if (YUNI_PRIVATE_MEMBUF_SPTRINF(buffer, sizeof(buffer), "%p", rhs) >= 0)
					{
						s.appendWithoutChecking(buffer,
							Yuni::Traits::Length<typename CStringT::Type*, typename CStringT::Size>::Value(buffer));
					}
					else
						s.appendWithoutChecking("0x0", 3);
				}
				# endif
			}
		}
	};


	// void*
	template<class CStringT>
	class Append<CStringT, Yuni::UTF8::Char> final
	{
	public:
		static void Perform(CStringT& s, const Yuni::UTF8::Char& rhs)
		{
			assert(&rhs != NULL);
			rhs.write(s);
		}
	};




# define YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL(BUFSIZE, FORMAT, TYPE) \
	template<class CStringT> \
	class Append<CStringT, TYPE> final \
	{ \
	public: \
		static void Perform(CStringT& s, const TYPE rhs) \
		{ \
			typename CStringT::Type buffer[BUFSIZE]; \
			/* On Windows, it may return a negative value */ \
			if (YUNI_PRIVATE_MEMBUF_SPTRINF(buffer, BUFSIZE, FORMAT, rhs) >= 0) \
			{ \
				buffer[BUFSIZE - 1] = '\0'; /* making sure that it is zero-terminated */ \
				s.appendWithoutChecking(buffer, \
					Yuni::Traits::Length<typename CStringT::Type*, typename CStringT::Size>::Value(buffer)); \
			} \
		} \
	}

# define YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(TYPE) \
	template<class CStringT> \
	class Append<CStringT, TYPE> final \
	{ \
	public: \
		static void Perform(CStringT& s, const TYPE rhs) \
		{ \
			Yuni::Private::CStringImpl::From<Math::Base::Decimal, TYPE>::AppendTo(s, rhs); \
		} \
	}


	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(sint16);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(sint32);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(sint64);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(uint16);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(uint32);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(uint64);
	# ifdef YUNI_HAS_LONG
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(long);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL_INT(unsigned long);
	# endif

	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL(256, "%f",  float);
	YUNI_PRIVATE_MEMORY_BUFFER_APPEND_IMPL(256, "%f",  double);



	// std::vector<>
	template<class CStringT, class T>
	class Append<CStringT, std::vector<T> > final
	{
	public:
		typedef std::vector<T> ListType;
		static void Perform(CStringT& s, const ListType& rhs)
		{
			s += '[';
			if (not rhs.empty())
			{
				const typename ListType::const_iterator end = rhs.end();
				typename ListType::const_iterator i = rhs.begin();
				s += *i;
				++i;
				for (; i != end; ++i)
					s << ", " << *i;
			}
			s += ']';
		}
	};


	// std::vector<>
	template<class CStringT, class T>
	class Append<CStringT, std::list<T> > final
	{
	public:
		typedef std::list<T> ListType;
		static void Perform(CStringT& s, const ListType& rhs)
		{
			s += '[';
			if (not rhs.empty())
			{
				const typename ListType::const_iterator end = rhs.end();
				typename ListType::const_iterator i = rhs.begin();
				s += *i;
				++i;
				for (; i != end; ++i)
					s << ", " << *i;
			}
			s += ']';
		}
	};






} // namespace CString
} // namespace Extension
} // namespace Yuni

#undef YUNI_PRIVATE_MEMBUF_SPTRINF
