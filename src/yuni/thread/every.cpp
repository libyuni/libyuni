/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#include "../thread/utility.h"
#include "../datetime/timestamp.h"
#ifdef YUNI_HAS_CPP_MOVE
# include <utility>
#endif


namespace Yuni
{

	struct EveryTimer final : public Thread::Timer
	{
		EveryTimer(uint ms, const Bind<bool ()>& callback) :
			Thread::Timer(ms),
			pCallback(callback)
		{}

		virtual ~EveryTimer()
		{
			// mandatory, the user is unlikely to call it when using 'every'
			if (started())
			{
				gracefulStop();
				wait(); // wait indefinitively if needed
			}
		}

	protected:
		virtual bool onInterval(uint /*cycle*/) override
		{
			return pCallback();
		}

	private:
		Bind<bool ()> pCallback;
	};


	std::unique_ptr<Thread::Timer> every(uint ms, const Bind<bool ()>& callback, bool autostart)
	{
		auto timer = std::make_unique<EveryTimer>(ms, callback);
		if (autostart)
			timer->start();
		return std::unique_ptr<Thread::Timer>(std::move(timer));
	}


	template<bool PreciseT>
	struct EveryTimerElapsed final : public Thread::Timer
	{
		EveryTimerElapsed(uint ms, const Bind<bool (uint64)>& callback) :
			Thread::Timer(ms),
			pCallback(callback)
		{
			pLastTimestamp = DateTime::NowMilliSeconds();
		}

		virtual ~EveryTimerElapsed()
		{
			// mandatory, the user is unlikely to call it when using 'every'
			stop();
		}

	protected:
		virtual bool onInterval(uint /*cycle*/) override
		{
			if (PreciseT)
			{
				// current timestamp in ms
				auto now = DateTime::NowMilliSeconds();
				// callback
				bool shouldContinue = pCallback((uint64)(now - pLastTimestamp));
				// fetch again the current to avoid taking into consideration
				// the time spent in the callback
				pLastTimestamp = DateTime::NowMilliSeconds();
				return shouldContinue;
			}
			else
			{
				// current timestamp in ms
				auto now = DateTime::NowMilliSeconds();
				uint64 elapsed = (uint64) (now - pLastTimestamp);
				pLastTimestamp = now;
				return pCallback(elapsed);
			}
		}


	private:
		sint64 pLastTimestamp;
		Bind<bool (uint64)> pCallback;
	};


	std::unique_ptr<Thread::Timer> every(uint ms, bool precise, const Bind<bool (uint64)>& callback, bool autostart)
	{
		auto timer = [&]() -> std::unique_ptr<Thread::Timer> {
			if (precise)
				return std::make_unique<EveryTimerElapsed<true>>(ms, callback);
			return std::make_unique<EveryTimerElapsed<false>>(ms, callback);
		}();
		if (autostart)
			timer->start();
		return timer;
	}


} // namespace Yuni
