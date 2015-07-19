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
		uint nnTimeInterval = pTimeInterval;
		pTimerMutex.unlock();

		do
		{
			if (IThread::suspend(nnTimeInterval))
				break;
			if (not onInterval(infinite /* no cycle */))
				break;
			if (pShouldReload)
				return false;
		}
		while (true);

		return true;
	}


	bool Timer::internalRunFixedNumberOfCycles()
	{
		uint cycleIndex = 0;
		uint nnTimeInterval = pTimeInterval;
		pTimerMutex.unlock();

		do
		{
			// Wait then execute the timer
			if (suspend(nnTimeInterval) or not onInterval(cycleIndex))
				return true;
			if (++cycleIndex >= pCycleCount) // the maximum number of cycle is reached
				return true;
			if (pShouldReload)
				return false;
		}
		while (true);

		return true;
	}


	bool Timer::onExecute()
	{
		pShouldReload = false;
		do
		{
			// Note : pTimerMutex will be unlocked by internalRunInfiniteLoop
			// and internalRunFixedNumberOfCycles
			pTimerMutex.lock();

			// No cycle to do, aborting now
			if (0 == pCycleCount)
			{
				pTimerMutex.unlock();
				return false;
			}

			// infinite loop
			if (infinite == pCycleCount)
			{
				// pTimerMutex will be unlocked by internalRunInfiniteLoop
				if (internalRunInfiniteLoop())
				{
					// Stopping the thread
					return false;
				}
			}
			else
			{
				// pTimerMutex will be unlocked by internalRunFixesNumberOfCycles
				if (internalRunFixedNumberOfCycles())
				{
					// Stopping the thread
					return false;
				}
			}
		}
		while (true);

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
		pShouldReload = true;
	}


	void Timer::reload(uint milliseconds, uint cycles)
	{
		pTimerMutex.lock();
		pTimeInterval = milliseconds;
		pCycleCount   = cycles;
		pTimerMutex.unlock();
		pShouldReload = true;
	}




} // namespace Thread
} // namespace Yuni

