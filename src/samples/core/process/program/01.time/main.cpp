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
