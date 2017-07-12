/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#include <yuni/yuni.h>
#include <yuni/core/process/program.h>
#include <yuni/datetime/timestamp.h>
#include <yuni/core/getopt.h>
#include <iostream>
#include <signal.h>
#ifdef YUNI_OS_WINDOWS
# include <windows.h>
#endif

using namespace Yuni;


static Process::Program process;


#ifdef YUNI_OS_WINDOWS
BOOL ctrlHandler(DWORD ctrlType)
{
	switch (ctrlType)
	{
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
		process.signal(SIGINT);
		return FALSE;
	default:
		return FALSE;
	}
}
#else
void signalHandler(int sig)
{
	// sending the same signal to the sub-process. If the sub-process
	// handles the signal, it may have a chance to stop properly and should behave
	// the same as if not executed by this program.
	process.signal(sig);

	if (sig == SIGINT) // ctrl-c
	{
		// a new line to keep the output clean (^C)
		std::cout << std::endl;
	}
}
#endif





int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <program> [<args>]\n";
		return EXIT_FAILURE;
	}


	// providing arguments
	process.program(argv[1]);
	for (int i = 2; i < argc; ++i)
		process.argumentAdd(argv[i]);

	// use milliseconds instead of seconds
	process.durationPrecision(Process::Program::dpMilliseconds);
	// redirect the output to the console
	process.redirectToConsole(true);

	#ifdef YUNI_OS_WINDOWS
	::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlHandler, TRUE);
	#else
	// install signal handler for SIGINT (ctrl-c) and SIGTERM (quit)
	// see note in the 'signalHandler' routine
	::signal(SIGINT,  signalHandler); // interrupt program (^C ctrl-c)
	::signal(SIGTERM, signalHandler); // software termination signal
	#endif


	// execute the command
	sint64 duration = 0;
	int exitStatus = process.execute() ? process.wait(&duration) : EXIT_FAILURE;


	// display the result nearly like `time`
	std::cout << argv[1];
	for (int i = 2; i < argc; ++i)
		std::cout << ' ' << argv[i];

	std::cout << "  " << duration << "ms";
	if (exitStatus != 0)
		std::cout << " (exit: " << exitStatus << ')';
	std::cout << '\n';
	return exitStatus;
}
