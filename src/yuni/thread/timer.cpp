
#include "timer.h"
#include <cassert>


namespace Yuni
{
namespace Thread
{

	Timer::Timer() :
		IThread(),
		pTimeInterval(defaultInterval),
		pCycleCount(infinite)
	{}


	Timer::Timer(uint interval) :
		IThread(),
		pTimeInterval(interval),
		pCycleCount(infinite)
	{}


	Timer::Timer(uint interval, uint cycles) :
		IThread(),
		pTimeInterval(interval),
		pCycleCount(cycles)
	{}


	Timer::~Timer()
	{
		assert(started() == false);
	}



	Timer::Timer(const Timer& rhs) :
		IThread()
	{
		rhs.pTimerMutex.lock();
		pTimeInterval = rhs.pTimeInterval;
		pCycleCount = rhs.pCycleCount;
		rhs.pTimerMutex.unlock();
	}


	Timer& Timer::operator = (const Timer& rhs)
	{
		pTimerMutex.lock();
		rhs.pTimerMutex.lock();
		pTimeInterval = rhs.pTimeInterval;
		pCycleCount = rhs.pCycleCount;
		pTimerMutex.unlock();
		rhs.pTimerMutex.unlock();
		return *this;
	}


	bool Timer::internalRunInfiniteLoop()
	{
		uint nnTimeInterval(pTimeInterval);
		pTimerMutex.unlock();

		while (true)
		{
			if (IThread::suspend(nnTimeInterval))
				break;
			if (!onInterval(infinite /* no cycle */))
				break;
			if (pShouldReload)
				return false;
		}
		return true;
	}


	bool Timer::internalRunFixedNumberOfCycles()
	{
		uint cycleIndex = 0;
		uint nnTimeInterval = pTimeInterval;
		pTimerMutex.unlock();

		while (true)
		{
			// Wait then execute the timer
			if (suspend(nnTimeInterval) || !onInterval(cycleIndex))
				return true;
			if (++cycleIndex >= pCycleCount) // the maximum number of cycle is reached
				return true;
			if (pShouldReload)
				return false;
		}
		return true;
	}


	bool Timer::onExecute()
	{
		pShouldReload = 0;
		while (true)
		{
			// Lock
			// Note : pTimerMutex will be unlocked by internalRunInfiniteLoop
			// and internalRunFixedNumberOfCycles
			pTimerMutex.lock();

			// No cycle to do, aborting now
			if (!pCycleCount)
			{
				pTimerMutex.unlock();
				return false;
			}

			// infinite loop
			if (infinite == pCycleCount)
			{
				if (internalRunInfiniteLoop())
					// Stopping the thread
					return false;
			}
			else
			{
				if (internalRunFixedNumberOfCycles())
					// Stopping the thread
					return false;
			}
		}
		return false;
	}


	void Timer::interval(uint milliseconds)
	{
		pTimerMutex.lock();
		pTimeInterval = milliseconds;
		pTimerMutex.unlock();
	}


	void Timer::cycleCount(uint n)
	{
		pTimerMutex.lock();
		pCycleCount = n;
		pTimerMutex.unlock();
	}


	void Timer::reload(uint milliseconds)
	{
		pTimerMutex.lock();
		pTimeInterval = milliseconds;
		pTimerMutex.unlock();
		pShouldReload = 1;
	}


	void Timer::reload(uint milliseconds, uint cycles)
	{
		pTimerMutex.lock();
		pTimeInterval = milliseconds;
		pCycleCount   = cycles;
		pTimerMutex.unlock();
		pShouldReload = 1;
	}




} // namespace Thread
} // namespace Yuni

