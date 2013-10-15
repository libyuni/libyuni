#ifndef __YUNI_CORE_VERSION_HXX__
# define __YUNI_CORE_VERSION_HXX__


namespace Yuni
{

	inline Version::Version() :
		hi(), lo(), revision()
	{}


	inline Version::Version(uint major) :
		hi(major), lo(), revision()
	{}


	inline Version::Version(uint major, uint minor) :
		hi(major), lo(minor), revision()
	{}


	inline Version::Version(uint major, uint minor, uint rev) :
		hi(major), lo(minor), revision(rev)
	{}


	inline Version::Version(const Version& c) :
		hi(c.hi), lo(c.lo), revision(c.revision)
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


#endif // __YUNI_CORE_VERSION_HXX__
