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
	class Assign final
	{
	public:
		static void Perform(CStringT& s, const C& rhs)
		{
			// By Default, we will clear the buffer and then append the new content
			// Which is the standard behavior but may not the most efficient way
			s.clear();
			Yuni::Extension::CString::Append<CStringT, C>::Perform(s, rhs);
		}
	};


	// T*
	template<class CStringT, class T>
	class Assign<CStringT, T*> final
	{
	public:
		static void Perform(CStringT& s, const T* rhs)
		{
			s = (void*) rhs;
		}
	};


	// char*
	template<class CStringT>
	class Assign<CStringT, void*> final
	{
	public:
		static void Perform(CStringT& s, const void* rhs)
		{
			if (!rhs)
				s.assignWithoutChecking("0x0", 3);
			else
			{
				# ifdef YUNI_OS_MSVC
				// With Visual Studio, the option %p does not provide the prefix 0x
				typename CStringT::Type buffer[32];
				buffer[0] = '0';
				buffer[1] = 'x';
				// On Windows, it may return a negative value
				if (YUNI_PRIVATE_MEMBUF_SPTRINF(buffer + 2, sizeof(buffer) - 2, "%p", rhs) >= 0)
				{
					s.appendWithoutChecking(buffer,
						Yuni::Traits::Length<typename CStringT::Type*, typename CStringT::Size>::Value(buffer));
				}
				else
					s.assignWithoutChecking("0x0", 3);
				# else
				typename CStringT::Type buffer[32];
				// On Windows, it may return a negative value
				if (YUNI_PRIVATE_MEMBUF_SPTRINF(buffer, sizeof(buffer), "%p", rhs) >= 0)
				{
					s.assignWithoutChecking(buffer,
						Yuni::Traits::Length<typename CStringT::Type*, typename CStringT::Size>::Value(buffer));
				}
				else
					s.assignWithoutChecking("0x0", 3);
				# endif
			}
		}
	};



	// char*
	template<class CStringT>
	class Assign<CStringT, char*> final
	{
	public:
		static void Perform(CStringT& s, const char* rhs)
		{
			if (rhs)
			{
				s.assignWithoutChecking(rhs,
					Yuni::Traits::Length<char*, typename CStringT::Size>::Value(rhs));
			}
			else
				s.clear();
		}
	};


	// C[N]
	template<class CStringT, int N>
	class Assign<CStringT, char[N]> final
	{
	public:
		typedef char C;
		static void Perform(CStringT& s, const C rhs[N])
		{
			if (N > 0)
			{
				// The calculation with `N` is required to properly handle
				// both a zero-terminated buffer and a simple array
				s.assignWithoutChecking(rhs, N - ((rhs[N-1] == C()) ? 1 : 0));
			}
			else
				s.clear();
		}
	};


	// C
	template<class CStringT>
	class Assign<CStringT, char> final
	{
	public:
		static void Perform(CStringT& s, const char rhs)
		{
			s.assignWithoutChecking(rhs);
		}
	};


	// nullptr
	template<class CStringT>
	class Assign<CStringT, YuniNullPtr> final
	{
	public:
		static void Perform(CStringT& s, const YuniNullPtr&)
		{
			s.clear();
		}
	};



	// bool
	template<class CStringT>
	class Assign<CStringT, bool> final
	{
	public:
		static void Perform(CStringT& s, const bool rhs)
		{
			if (rhs)
				s.assignWithoutChecking("true", 4);
			else
				s.assignWithoutChecking("false", 5);
		}
	};






} // namespace CString
} // namespace Extension
} // namespace Yuni

#undef YUNI_PRIVATE_MEMBUF_SPTRINF
