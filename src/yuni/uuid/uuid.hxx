/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#pragma once
#include "uuid.h"
#include "../core/stl/hash-combine.h"




namespace Yuni
{

	inline UUID::UUID()
		: m_value{{0, 0}}
	{}


	inline UUID::UUID(const AnyString& string)
	{
		assign(string);
	}


	inline void UUID::clear()
	{
		m_value.u64[0] = 0;
		m_value.u64[1] = 0;
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


	inline UUID& UUID::operator = (const AnyString& string)
	{
		assign(string);
		return *this;
	}


	inline bool UUID::null() const
	{
		return  (0 == m_value.u64[0]) and (0 == m_value.u64[1]);
	}


	inline bool UUID::operator == (const UUID& rhs) const
	{
		return  (m_value.u64[0] == rhs.m_value.u64[0])
			and (m_value.u64[1] == rhs.m_value.u64[1]);
	}


	inline bool UUID::operator != (const UUID& rhs) const
	{
		return (not operator == (rhs));
	}


	inline bool UUID::operator < (const UUID& rhs) const
	{
		return (m_value.u64[0] == rhs.m_value.u64[0])
			? (m_value.u64[1] < rhs.m_value.u64[1])
			: (m_value.u64[0] < rhs.m_value.u64[0]);
	}


	inline bool UUID::operator > (const UUID& rhs) const
	{
		return (m_value.u64[0] == rhs.m_value.u64[0])
			? (m_value.u64[1] > rhs.m_value.u64[1])
			: (m_value.u64[0] > rhs.m_value.u64[0]);
	}


	inline bool UUID::operator <= (const UUID& rhs) const
	{
		return ((*this < rhs) or (*this == rhs));
	}


	inline bool UUID::operator >= (const UUID& rhs) const
	{
		return ((*this > rhs) or (*this == rhs));
	}


	inline size_t UUID::hash() const
	{
		size_t seed = 0;
		if (sizeof(size_t) == sizeof(uint64_t))
		{
			Yuni::HashCombine(seed, m_value.u64[0]);
			Yuni::HashCombine(seed, m_value.u64[1]);
		}
		else
		{
			Yuni::HashCombine(seed, m_value.u32[0]);
			Yuni::HashCombine(seed, m_value.u32[1]);
			Yuni::HashCombine(seed, m_value.u32[2]);
			Yuni::HashCombine(seed, m_value.u32[3]);
		}
		return seed;
	}


} // namespace Yuni


namespace Yuni
{
namespace Private
{
namespace UUID
{

	struct Helper final
	{
		static inline void WriteToCString(char* cstr, const Yuni::UUID& uuid)
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
