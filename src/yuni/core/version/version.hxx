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
#include "version.h"



namespace Yuni
{

	inline Version::Version()
		: hi(), lo(), patch()
	{}


	inline Version::Version(uint32_t major)
		: hi(major), lo(), patch()
	{}


	inline Version::Version(uint32_t major, uint32_t minor)
		: hi(major), lo(minor), patch()
	{}


	inline Version::Version(uint32_t major, uint32_t minor, uint32_t patch)
		: hi(major), lo(minor), patch(patch)
	{}


	inline Version::Version(const Version& c)
		: hi(c.hi), lo(c.lo), patch(c.patch)
	{}


	inline void Version::assign(uint32_t major, uint32_t minor, uint32_t p)
	{
		hi = major;
		lo = minor;
		patch = p;
	}


	inline void Version::clear()
	{
		hi = lo = patch = 0;
		metadata.clear();
	}


	inline bool Version::isEqualTo(const Version& rhs) const
	{
		return (rhs.hi == hi) and (rhs.lo == lo) and (rhs.patch == patch);
	}


	inline bool Version::operator <  (const Version& rhs) const
	{
		return isLessThan(rhs);
	}


	inline bool Version::operator <= (const Version& rhs) const
	{
		return isEqualTo(rhs) or isLessThan(rhs);
	}


	inline bool Version::operator >  (const Version& rhs) const
	{
		return isGreaterThan(rhs);
	}


	inline bool Version::operator >= (const Version& rhs) const
	{
		return isEqualTo(rhs) or isGreaterThan(rhs);
	}


	inline bool Version::operator == (const Version& rhs) const
	{
		return isEqualTo(rhs);
	}


	inline bool Version::operator != (const Version& rhs) const
	{
		return !isEqualTo(rhs);
	}


	inline bool Version::null() const
	{
		return (hi == 0 and lo == 0 and patch == 0);
	}


	inline String Version::toString() const
	{
		return String() << hi << '.' << lo << '.' << patch;
	}


	template<class S>
	inline void Version::print(S& out) const
	{
		out << hi << '.' << lo << '.' << patch;
		if (not metadata.empty())
			out << '-' << metadata;
	}


	inline Version& Version::operator = (const Version& rhs)
	{
		hi = rhs.hi;
		lo = rhs.lo;
		patch = rhs.patch;
		metadata = rhs.metadata;
		return *this;
	}



} // namespace Yuni




inline std::ostream& operator << (std::ostream& out, const Yuni::Version& rhs)
{
	rhs.print(out);
	return out;
}
