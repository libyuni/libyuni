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
#pragma once
#include "../yuni.h"
#include "timer.h"
#include "../job/job.h"
#include "../core/bind.h"

namespace Yuni { namespace Job { class QueueService; }}
namespace Yuni { namespace Job { class Taskgroup; }}


namespace Yuni
{


	/*!
	** \brief Convenient wrapper for executing some code into another thread
	**
	** The callback will be executed into another thread.
	**
	** From sample:
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/thread/utility.h>
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
	**	// Suspend the execution of the main thread until all threads are terminated
	**	thread1->wait();
	**	thread2->wait();
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
	** #include <yuni/thread/thread.h>
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
	**
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
	** #include <yuni/thread/thread.h>
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
	**
	** \param ms An amount of time, in milliseconds
	** \param precise True to not take into consideration the time spent in the callback (with performance penalty)
	** \param callback The callback to execute
	** \param autostart True to automatically start the timer
	*/
	Thread::Timer::Ptr  every(uint ms, bool precise, const Bind<bool (uint64 /*elapsed*/)>& callback, bool autostart = true);


	/*!
	** \brief Convenient wrapper for executing a new job
	**
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/thread/utility.h>
	** #include <yuni/core/system/suspend.h>
	** #include <iostream>
	** using namespace Yuni;
	**
	** int main()
	** {
	**	async([&] () {
	**		std::cout << "thread1: some complex computations here..." << std::endl;
	**		std::cout << "thread1: done here !" << std::endl;
	**	});
	**	Suspend(2);
	**	return 0;
	** }
	** \endcode
	**
	** \param queueservice The queueservice to dispatch the new job
	** \param callback The callback to execute
	*/
	Job::IJob::Ptr  async(Job::QueueService& queueservice, const Bind<void ()>& callback);


	/*!
	** \brief Convenient wrapper for creating a job from a lambda
	**
	** Since no queueservice is provided, the job will only be created and not added in
	** any queue (and thus not executed).
	**
	** \param callback The callback to execute
	*/
	Job::IJob::Ptr  async(const Bind<bool (Job::IJob&)>& callback);


	/*!
	** \brief Convenient wrapper for creating a job from a lambda into a taskgroup
	*/
	void async(Job::Taskgroup& task, const Bind<bool (Job::IJob&)>& callback);




} // namespace Yuni
