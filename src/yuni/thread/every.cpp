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
#include "../thread/utility.h"
#include "../datetime/timestamp.h"
#ifdef YUNI_HAS_CPP_MOVE
# include <utility>
#endif



namespace Yuni
{

	class EveryTimer final : public Thread::Timer
	{
	public:
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



	Thread::Timer::Ptr  every(uint ms, const Bind<bool ()>& callback, bool autostart)
	{
		# ifdef YUNI_HAS_CPP_MOVE
		Thread::Timer* timer = new EveryTimer(ms, std::move(callback));
		# else
		Thread::Timer* timer = new EveryTimer(ms, callback);
		# endif
		if (autostart)
			timer->start();
		return timer;
	}





	template<bool PreciseT>
	class EveryTimerElapsed final : public Thread::Timer
	{
	public:
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



	Thread::Timer::Ptr  every(uint ms, bool precise, const Bind<bool (uint64)>& callback, bool autostart)
	{
		Thread::Timer* timer;

		if (precise)
		{
			# ifdef YUNI_HAS_CPP_MOVE
			timer = new EveryTimerElapsed<true>(ms, std::move(callback));
			# else
			timer = new EveryTimerElapsed<true>(ms, callback);
			# endif
		}
		else
		{
			# ifdef YUNI_HAS_CPP_MOVE
			timer = new EveryTimerElapsed<false>(ms, std::move(callback));
			# else
			timer = new EveryTimerElapsed<false>(ms, callback);
			# endif
		}

		if (autostart)
			timer->start();
		return timer;
	}




} // namespace Yuni

