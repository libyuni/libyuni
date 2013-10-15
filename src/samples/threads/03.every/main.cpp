
#include <yuni/yuni.h>
#include <yuni/thread/every.h>
#include <yuni/core/system/suspend.h>

using namespace Yuni;


# ifdef YUNI_HAS_CPP_LAMBDA

int main(void)
{
	uint counter = 0;
	auto timer = Every(1000, [&] () -> bool {
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
	Thead::Timer::Ptr timer = Every(1000, & TimerCallback);

	// wait for 10 seconds
	Suspend(5);
	return 0;
}



# endif
