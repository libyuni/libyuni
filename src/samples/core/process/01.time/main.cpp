#include <yuni/yuni.h>
#include <yuni/core/process.h>
#include <yuni/datetime/timestamp.h>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage : " << argv[0]
				  << " <program> [ <args> ]" << std::endl;
		return 1;
	}
	auto startTime = Yuni::DateTime::NowMilliSeconds();
	Yuni::Process process;
	process.program(argv[1]);
	for (int i = 2; i < argc; ++i)
	{
		process.argumentAdd(argv[i]);
	}
	process.execute();
	int exitStatus = process.wait();
	auto endTime = Yuni::DateTime::NowMilliSeconds();
	std::cout << "Completed in " << (endTime - startTime) << "ms" << std::endl;
	return exitStatus;
}
