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
#include "uuid.h"




namespace Yuni
{

	template<class StringT>
	inline UUID::UUID(const StringT& string)
	{
		if (not assign(string))
			clear();
	}


	inline void UUID::clear()
	{
		pValue.n64[0] = 0;
		pValue.n64[1] = 0;
	}


	inline UUID::UUID()
	{
		pValue.n64[0] = 0;
		pValue.n64[1] = 0;
	}


	inline UUID::UUID(const UUID& rhs)
	{
		pValue.n64[0] = rhs.pValue.n64[0];
		pValue.n64[1] = rhs.pValue.n64[1];
	}


	inline UUID::UUID(Flag flag)
	{
		switch (flag)
		{
			case fGenerate: generate(); break;
			case fNull: clear(); break;
		}
	}


	inline bool UUID::operator ! () const
	{
		return null();
	}


	template<class StringT>
	inline UUID& UUID::operator = (const StringT& string)
	{
		if (not assign(string))
			clear();
		return *this;
	}


	inline bool UUID::null() const
	{
		return  (0 == pValue.n64[0]) and (0 == pValue.n64[1]);
	}


	inline UUID& UUID::operator = (const UUID& rhs)
	{
		pValue.n64[0] = rhs.pValue.n64[0];
		pValue.n64[1] = rhs.pValue.n64[1];
		return *this;
	}


	inline bool UUID::operator == (const UUID& rhs) const
	{
		return  (pValue.n64[0] == rhs.pValue.n64[0])
			and (pValue.n64[1] == rhs.pValue.n64[1]);
	}


	inline bool UUID::operator != (const UUID& rhs) const
	{
		return (not operator == (rhs));
	}


	inline bool UUID::operator < (const UUID& rhs) const
	{
		return (pValue.n64[0] == rhs.pValue.n64[0])
			? (pValue.n64[1] < rhs.pValue.n64[1])
			: (pValue.n64[0] < rhs.pValue.n64[0]);
	}


	inline bool UUID::operator > (const UUID& rhs) const
	{
		return (pValue.n64[0] == rhs.pValue.n64[0])
			? (pValue.n64[1] > rhs.pValue.n64[1])
			: (pValue.n64[0] > rhs.pValue.n64[0]);
	}


	inline bool UUID::operator <= (const UUID& rhs) const
	{
		return ((*this < rhs) or (*this == rhs));
	}


	inline bool UUID::operator >= (const UUID& rhs) const
	{
		return ((*this > rhs) or (*this == rhs));
	}





} // namespace Yuni







namespace Yuni
{
namespace Private
{
namespace UUID
{

	class Helper final
	{
	public:
		static void WriteToCString(char* cstr, const Yuni::UUID& uuid)
		{
			uuid.writeToCString(cstr);
		}
	};


} // namespace UUID
} // namespace Private
} // namespace Yuni







namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT>
	class Append<CStringT, Yuni::UUID> final
	{
	public:
		static void Perform(CStringT& s, const Yuni::UUID& rhs)
		{
			const uint currentLength = s.size();
			// writeToCString is guarantee to have 42 chars
			s.reserve(currentLength + 42); // at least 36 + 1 zero-terminated
			Yuni::Private::UUID::Helper::WriteToCString((char*)s.data() + currentLength, rhs);
			s.resize(currentLength + 36); // guid is 36-bytes length
			//s.removeLast();
		}

	}; // class Append


	template<>
	class Into<Yuni::UUID> final
	{
	public:
		typedef Yuni::UUID TargetType;
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, TargetType& out)
		{
			return out.assign(s);
		}

		template<class StringT> static TargetType Perform(const StringT& s)
		{
			return TargetType(s);
		}

	}; // class Into



} // namespace CString
} // namespace Extension
} // namespace Yuni


// ostream
std::ostream& operator << (std::ostream& out, const Yuni::UUID& rhs);

