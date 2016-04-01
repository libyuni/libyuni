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
#include <iostream>

using namespace Yuni;




# ifdef YUNI_HAS_CPP_LAMBDA

int main(void)
{
	uint counter = 0;
	auto timer = every(1000, [&] () -> bool {
		std::cout << "counter: " << ++counter << std::endl;
		return true; // continue looping
	});

	// wait for 5 seconds
	Suspend(5);
	return 0;
}






# else

// here is an example for compilers which do not support c++11 features

static bool TimerCallback()
{
	std::cout << "tick ! " << std::endl;
}

int main(void)
{
	Thead::Timer::Ptr timer = every(1000, & TimerCallback);

	// wait for 10 seconds
	Suspend(5);
	return 0;
}



# endif
