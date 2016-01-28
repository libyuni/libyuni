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
#include <yuni/yuni.h>
#include <yuni/thread/utility.h>
#include <yuni/core/system/suspend.h>
#include <yuni/core/logs.h>

using namespace Yuni;


# ifdef YUNI_HAS_CPP_LAMBDA // C++11 features

int main(void)
{
	// A simple logger, which only display on std::cout/cerr
	Logs::Logger<> logs;

	auto thread1 = spawn([&] () {
		logs.info() << "thread1: some complex computations here...";
		// suspend the execution of the thread for 3 seconds for the demo
		Suspend(3);
		logs.info() << "thread1: done here !";
	});

	auto thread2 = spawn([&] () {
		logs.info() << "thread2: some complex computations here...";
		// suspend the execution of the thread for 2 seconds for the demo
		Suspend(2);
		logs.info() << "thread2: done here !";
	});

    // will wait for thread1 and thread2
	return 0;
}








# else

// here is an example for compilers which do not support c++11 features


// A simple logger, which only display on std::cout/cerr
static Logs::Logger<> logs;


static void Thread1Callback()
{
	logs.info() << "thread1: some complex computations here...";
	// block the execution of the thread for 3 seconds for the demo
	Suspend(3);
	logs.info() << "thread1: done here !";
}

static void Thread2Callback()
{
	logs.info() << "thread2: some complex computations here...";
	// block the execution of the thread for 2 seconds for the demo
	Suspend(2);
	logs.info() << "thread2: done here !";
}


int main(void)
{
	Thread::IThread::Ptr thread1 = spawn(& Thread1Callback);
	Thread::IThread::Ptr thread2 = spawn(& Thread2Callback);

	thread1->wait(); // wait for the first thread
	thread2->wait(); // wait for the second thread
	return 0;
}



# endif
