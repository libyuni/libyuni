
#include "version.h"


namespace Yuni
{


	void Version::InternalLib(Version& v)
	{
		v.hi = YUNI_VERSION_HI;
		v.lo = YUNI_VERSION_LO;
		v.revision = YUNI_VERSION_REV;
	}


	bool Version::isLessThan(const Version& rhs) const
	{
		if (hi < rhs.hi)
			return true;
		if (hi == rhs.hi)
		{
			if (lo < rhs.lo)
				return true;
			if (lo == rhs.lo)
				return revision < rhs.revision;
		}
		return false;
	}


	bool Version::isGreaterThan(const Version& rhs) const
	{
		if (hi > rhs.hi)
			return true;
		if (hi == rhs.hi)
		{
			if (lo > rhs.lo)
				return true;
			if (lo == rhs.lo)
				return revision > rhs.revision;
		}
		return false;
	}


	String Version::toString() const
	{
		return String() << hi << '.' << lo << '.' << revision;
	}





} // namespace Yuni

