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
#include "version.h"



namespace Yuni
{

	inline Version::Version()
		: hi(), lo(), revision()
	{}


	inline Version::Version(uint major)
		: hi(major), lo(), revision()
	{}


	inline Version::Version(uint major, uint minor)
		: hi(major), lo(minor), revision()
	{}


	inline Version::Version(uint major, uint minor, uint rev)
		: hi(major), lo(minor), revision(rev)
	{}


	inline Version::Version(const Version& c)
		: hi(c.hi), lo(c.lo), revision(c.revision)
	{}




	inline void Version::assign(uint major, uint minor, uint rev)
	{
		hi = major;
		lo = minor;
		revision = rev;
	}


	inline void Version::clear()
	{
		hi = lo = revision = 0;
	}


	inline bool Version::isEqualTo(const Version& rhs) const
	{
		return (rhs.hi == hi) and (rhs.lo == lo) and (rhs.revision == revision);
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
		return (hi == 0 and lo == 0 and revision == 0);
	}


	template<class S>
	inline void Version::print(S& out) const
	{
		out << hi << '.' << lo << '.' << revision;
	}


	inline Version& Version::operator = (const Version& rhs)
	{
		hi = rhs.hi;
		lo = rhs.lo;
		revision = rhs.revision;
		return *this;
	}






} // namespace Yuni



inline std::ostream& operator << (std::ostream& out, const Yuni::Version& rhs)
{
	rhs.print(out);
	return out;
}
