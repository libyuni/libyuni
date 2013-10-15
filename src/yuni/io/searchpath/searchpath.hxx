#ifndef __YUNI_IO_SEARCHPATH__SEARCH_PATH_HXX__
# define __YUNI_IO_SEARCHPATH__SEARCH_PATH_HXX__


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

#endif // __YUNI_IO_SEARCHPATH__SEARCH_PATH_HXX__
