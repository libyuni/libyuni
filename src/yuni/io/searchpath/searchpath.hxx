#pragma once
#include "searchpath.h"


namespace Yuni
{
namespace IO
{


	inline bool SearchPath::cacheLookup() const
	{
		return pCacheLookup;
	}


	inline void SearchPath::cacheLookup(bool enabled)
	{
		pCacheLookup = enabled;
	}


	inline void SearchPath::cacheClear()
	{
		pCache.clear();
	}






} // namespace IO
} // namespace Yuni
