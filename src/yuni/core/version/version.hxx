#ifndef __YUNI_CORE_VERSION_HXX__
# define __YUNI_CORE_VERSION_HXX__


namespace Yuni
{

	inline Version::Version() :
		hi(0u), lo(0u), revision(0u)
	{}


	inline Version::Version(uint h) :
		hi(h), lo(0u), revision(0u)
	{}


	inline Version::Version(uint h, uint l) :
		hi(h), lo(l), revision(0u)
	{}


	inline Version::Version(uint h, uint l, uint r) :
		hi(h), lo(l), revision(r)
	{}


	inline void Version::assign(uint h, uint l, uint r)
	{
		hi = h;
		lo = l;
		revision = r;
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





} // namespace Yuni



inline std::ostream& operator << (std::ostream& out, const Yuni::Version& rhs)
{
	rhs.print(out);
	return out;
}


#endif // __YUNI_CORE_VERSION_HXX__
