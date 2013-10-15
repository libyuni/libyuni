#ifndef __YUNI_THREAD_THREAD_HXX__
# define __YUNI_THREAD_THREAD_HXX__


namespace Yuni
{
namespace Thread
{


	inline bool IThread::started() const
	{
		return pStarted;
	}


	inline bool IThread::shouldAbort()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		Yuni::MutexLocker locker(pInnerFlagMutex);
		return (pShouldStop || not pStarted);
		# else
		return false;
		# endif
	}


	inline bool IThread::operator ! () const
	{
		return not started();
	}




} // namespace Thread
} // namespace Yuni

#endif // __YUNI_THREAD_THREAD_HXX__
