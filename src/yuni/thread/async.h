#ifndef __YUNI_THREAD_ASYNC_H__
# define __YUNI_THREAD_ASYNC_H__

# include "thread.h"
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
	**	auto thread1 = Async([&] () {
	**		std::cout << "thread1: some complex computations here..." << std::endl;
	**		// suspend the execution of the thread for 3 seconds for the demo
	**		thread1->suspend(3000);
	**		std::cout << "thread1: done here !" << std::endl;
	**	});
	**
	**	auto thread2 = Async([&] () {
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
	Thread::IThread::Ptr  Async(const Bind<void ()>& callback, bool autostart = true);





} // namespace Yuni

#endif // __YUNI_THREAD_ASYNC_H__
