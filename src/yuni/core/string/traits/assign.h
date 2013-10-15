#ifndef __YUNI_CORE_CUSTOM_STRING_TRAITS_ASSIGN_H__
# define __YUNI_CORE_CUSTOM_STRING_TRAITS_ASSIGN_H__

# include "../../traits/length.h"


# ifdef YUNI_OS_MSVC
#	define YUNI_PRIVATE_MEMBUF_SPTRINF(BUFFER,SIZE, F, V)  ::sprintf_s(BUFFER,SIZE,F,V)
# else
#	define YUNI_PRIVATE_MEMBUF_SPTRINF(BUFFER,SIZE, F, V)  ::snprintf(BUFFER,SIZE,F,V)
# endif


namespace Yuni
{
namespace Extension
{
namespace CString
{


	template<class CStringT, class C>
	class Assign
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
	class Assign<CStringT, T*>
	{
	public:
		static void Perform(CStringT& s, const T* rhs)
		{
			s = (void*) rhs;
		}
	};


	// char*
	template<class CStringT>
	class Assign<CStringT, void*>
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
	class Assign<CStringT, char*>
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
	class Assign<CStringT, char[N]>
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
	class Assign<CStringT, char>
	{
	public:
		static void Perform(CStringT& s, const char rhs)
		{
			s.assignWithoutChecking(rhs);
		}
	};


	// nullptr
	template<class CStringT>
	class Assign<CStringT, YuniNullPtr>
	{
	public:
		static void Perform(CStringT& s, const YuniNullPtr&)
		{
			s.clear();
		}
	};



	// bool
	template<class CStringT>
	class Assign<CStringT, bool>
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

# undef YUNI_PRIVATE_MEMBUF_SPTRINF

#endif // __YUNI_CORE_CUSTOM_STRING_TRAITS_ASSIGN_H__
