#ifndef __YUNI_THREAD_EVERY_H__
# define __YUNI_THREAD_EVERY_H__

# include "timer.h"
# include "../core/bind.h"


namespace Yuni
{

	/*!
	** \brief Convenient wrapper for executing some code every X milliseconds
	**
	** The code will be executed into another thread.
	** From sample:
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/thread/every.h>
	** #include <yuni/core/system/suspend.h>
	** #include <iostream>
	** using namespace Yuni;
	**
	** int main()
	** {
	**	uint counter = 0;
	**	auto timer = Every(1000, [&] () -> bool {
	**		std::cout << ++counter << std::endl;
	**		return true; // continue looping
	**	});
	**
	**	Suspend(5); // wait for 5 seconds
	**	return 0;
	** }
	** \endcode
	** \param ms An amount of time, in milliseconds
	** \param callback The callback to execute
	** \param autostart True to automatically start the timer
	*/
	Thread::Timer::Ptr  Every(uint ms, const Bind<bool ()>& callback, bool autostart = true);



	/*!
	** \brief Convenient wrapper for executing some code every X milliseconds and
	** getting the time elapsed between each tick
	**
	** The code will be executed into another thread.
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/thread/every.h>
	** #include <yuni/core/system/suspend.h>
	** #include <iostream>
	** using namespace Yuni;
	**
	** int main()
	** {
	**	auto timer = Every(1000, false, [&] (uint64 elapsed) -> bool {
	**		std::cout << elapsed << "ms since the last time" << std::endl;
	**		return true; // continue looping
	**	});
	**
	**	Suspend(5); // wait for 5 seconds
	**	return 0;
	** }
	** \endcode
	** \param ms An amount of time, in milliseconds
	** \param precise True to not take into consideration the time spent in the callback (with performance penalty)
	** \param callback The callback to execute
	** \param autostart True to automatically start the timer
	*/
	Thread::Timer::Ptr  Every(uint ms, bool precise, const Bind<bool (uint64 /*elapsed*/)>& callback, bool autostart = true);




} // namespace Yuni

#endif // __YUNI_THREAD_EVERY_H__
