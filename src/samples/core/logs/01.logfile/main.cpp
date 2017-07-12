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
#include <yuni/core/logs.h>


int main(void)
{
	// To enable the writing into a log file, we have to set the handlers to use
	// We want to write to the stdcout _and_ a log file
	typedef Yuni::Logs::StdCout< Yuni::Logs::File<> >  MyLogHandlers;

	// Our logger
	Yuni::Logs::Logger<MyLogHandlers>  logs;

	// Creating the log file (in the current directory, read-write access required)
	logs.logfile("./sample.log");
	if (not logs.logfileIsOpened())
	{
		// An error has occured. That means the log file could not be opened for
		// writing for some reason
		// In this sample, we will continue anyway
		logs.error() << "Impossible to open the log file !";
		return EXIT_FAILURE;
	}

	logs.info() << "Log file : " << logs.logfile();

	// Hello, world !, all standard verbosity levels
	logs.checkpoint() << "Hello, " << "world !";
	logs.notice() << "Hello, " << "world !";
	logs.warning() << "Hello, " << "world !";
	logs.error() << "Hello, " << "world !";
	logs.debug() << "Hello, " << "world !";
	logs.fatal() << "Hello, " << "world !";

	return 0;
}
