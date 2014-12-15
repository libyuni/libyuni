#include <yuni/yuni.h>
#include <yuni/core/process/program.h>
#include <yuni/datetime/timestamp.h>
#include <yuni/core/getopt.h>
#include <iostream>

using namespace Yuni;





int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <program> [<args>]\n";
		return EXIT_FAILURE;
	}

	Process::Program process;

	// providing arguments
	process.program(argv[1]);
	for (int i = 2; i < argc; ++i)
		process.argumentAdd(argv[i]);

	// use milliseconds instead of seconds
	process.durationPrecision(Process::Program::dpMilliseconds);

	// execute the command
	sint64 duration;
	int exitStatus = process.execute() ? process.wait(&duration) : EXIT_FAILURE;


	// display the result nearly like `time`
	std::cout << argv[1];
	for (int i = 2; i < argc; ++i)
		std::cout << ' ' << argv[i];

	std::cout << "  " << duration << "ms\n";
	return exitStatus;
}

