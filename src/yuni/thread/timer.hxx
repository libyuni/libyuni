#ifndef __YUNI_THREAD_TIMER_HXX__
# define __YUNI_THREAD_TIMER_HXX__


namespace Yuni
{
namespace Thread
{

	inline uint Timer::interval() const
	{
		Yuni::MutexLocker locker(pTimerMutex);
		return pTimeInterval;
	}


	inline uint Timer::cycleCount() const
	{
		Yuni::MutexLocker locker(pTimerMutex);
		return pCycleCount;
	}


	inline void Timer::reload()
	{
		pShouldReload = 1;
	}




} // namespace Thread
} // namespace Yuni

#endif // __YUNI_THREAD_TIMER_HXX__
