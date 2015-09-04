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
#include "string.h"



namespace Yuni
{

	//! \name Operator overload for stream printing
	//@{
	std::ostream& operator << (std::ostream& out, const AnyString& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::String& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::Clob& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::ShortString16& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::ShortString32& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::ShortString64& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::ShortString128& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::ShortString256& rhs);
	std::ostream& operator << (std::ostream& out, const Yuni::ShortString512& rhs);

	std::ostream& operator << (std::ostream& out, const Yuni::UTF8::Char& rhs);


	template<class CharT, class TraitsT, uint SizeT, bool ExpT>
	inline std::basic_istream<CharT, TraitsT>& operator >> (std::basic_istream<CharT, TraitsT>& stream, Yuni::CString<SizeT,ExpT>& string)
	{
		string.clear();
		while (stream.good())
		{
			char c;
			stream.get(c);
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
				break;
			string.append(c);
		}
		return stream;
	}


} // namespace Yuni






template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const char* const u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const wchar_t* u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const char u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const wchar_t u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}



template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const wchar_t* u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const char* const u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const char u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const wchar_t u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}




template<uint SizeT, bool ExpT, class TraitsT, class AllocT>
inline Yuni::CString<SizeT,ExpT>
operator + (const std::basic_string<char,TraitsT,AllocT>& u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}


template<uint SizeT, bool ExpT, class TraitsT, class AllocT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const std::basic_string<char,TraitsT,AllocT>& u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}


template<uint SizeT, bool ExpT>
inline bool
operator == (const char* lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs == lhs;
}

template<uint SizeT, bool ExpT>
inline bool
operator != (const char* lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs != lhs;
}

template<int N, uint SizeT, bool ExpT>
inline bool
operator == (const char lhs[N], const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs == lhs;
}

template<int N, uint SizeT, bool ExpT>
inline bool
operator != (const char lhs[N], const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs != lhs;
}

template<class TraitsT, class AllocT, uint SizeT, bool ExpT>
inline bool
operator == (const std::basic_string<char,TraitsT,AllocT>& lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs == lhs;
}

template<class TraitsT, class AllocT, uint SizeT, bool ExpT>
inline bool
operator != (const std::basic_string<char,TraitsT,AllocT>& lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs != lhs;
}


inline bool
operator == (const AnyString& mbs, Yuni::UTF8::Char c)
{
	return c.size() == mbs.size() && c == mbs.utf8begin().value();
}

inline bool
operator != (const AnyString& mbs, Yuni::UTF8::Char c)
{
	return c.size() != mbs.size() || c == mbs.utf8begin().value();
}

//@}







# ifdef YUNI_HAS_STL_HASH

namespace std
{
	template<uint SizeT, bool ExpT>
	struct YUNI_DECL hash<Yuni::CString<SizeT, ExpT> >
	{
		typedef Yuni::CString<SizeT, ExpT> argument_type;
		typedef std::size_t result_type;

		result_type operator() (const argument_type& string) const
		{
			return string.hash();
		}
	};

} // namespace std

# endif // YUNI_HAS_STL_HASH
