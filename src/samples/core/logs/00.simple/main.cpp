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
	// A simple logger, which only display on std::cout/cerr
	// Please note that std::cerr can not be used on Windows (the messages would not be displayed)
	Yuni::Logs::Logger<>  logs;


	// Hello, world !, all standard verbosity levels
	logs.checkpoint() << "Hello, " << "world !";
	logs.notice() << "Hello, " << "world !";
	logs.warning() << "Hello, " << "world !";
	logs.error() << "Hello, " << "world !";
	logs.debug() << "Hello, " << "world !";
	logs.fatal() << "Hello, " << "world !";

	return 0;
}
