#ifndef __YUNI_THREAD_CORE_H__
# define __YUNI_THREAD_CORE_H__

# include "../yuni.h"
# include "timer.h"
# include "../core/bind.h"


namespace Yuni
{

	/*!
	** \brief Convenient wrapper for executing some code into another thread
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
	**	auto thread1 = spawn([&] () {
	**		std::cout << "thread1: some complex computations here..." << std::endl;
	**		// suspend the execution of the thread for 3 seconds for the demo
	**		thread1->suspend(3000);
	**		std::cout << "thread1: done here !" << std::endl;
	**	});
	**
	**	auto thread2 = spawn([&] () {
	**		std::cout << "thread2: some complex computations here..." << std::endl;
	**		// suspend the execution of the thread for 3 seconds for the demo
	**		thread2->suspend(2000);
	**		std::cout << "thread2: done here !" << std::endl;
	**	});
	**
	**	thread1->wait(); // wait for the end of the first thread
	**	thread2->wait(); // wait for the end of the second thread
	**	return 0;
	** }
	** \endcode
	** \param callback The callback to execute
	** \param autostart True to automatically start the timer
	*/
	Thread::IThread::Ptr  spawn(const Bind<void ()>& callback, bool autostart = true);


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
	**	auto timer = every(1000, [&] () -> bool {
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
	Thread::Timer::Ptr  every(uint ms, const Bind<bool ()>& callback, bool autostart = true);


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
	**	auto timer = every(1000, false, [&] (uint64 elapsed) -> bool {
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
	Thread::Timer::Ptr  every(uint ms, bool precise, const Bind<bool (uint64 /*elapsed*/)>& callback, bool autostart = true);





} // namespace Yuni

#endif // __YUNI_THREAD_CORE_H__
