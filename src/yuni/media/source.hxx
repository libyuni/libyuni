#ifndef __YUNI_MEDIA_SOURCE_HXX__
# define __YUNI_MEDIA_SOURCE_HXX__


namespace Yuni
{
namespace Media
{


	inline Source::Source() :
		pAStream(nullptr),
		pVStream(nullptr),
		pBufferCount(0),
		pSecondsElapsed(0),
		pSecondsCurrent(0)
	{
	}


	inline Source::~Source()
	{
	}


	inline uint Source::duration() const
	{
		ThreadingPolicy::MutexLocker lock(*this);
		if (pAStream)
			return pAStream->duration();

		if (pVStream)
			return pVStream->duration();

		return 0u;
	}



} // namespace Media
} // namespace Yuni

#endif // __YUNI_MEDIA_SOURCE_HXX__
