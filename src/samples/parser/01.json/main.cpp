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
#include <yuni/core/system/console.h>
#include "json.h"
#include <iostream>



int main()
{
	Demo::JSON::Parser parser;

	if (parser.loadFromFile("example.json"))
	{
		// Colors only when the attached console is a TTY
		bool withColors = Yuni::System::Console::IsStdoutTTY();

		Yuni::Clob output;
		Demo::JSON::Node::Export(output, *parser.root, withColors);
		std::cout << output << std::endl;
		return EXIT_SUCCESS;
	}

	std::cerr << "failed to parse\n";
	return EXIT_FAILURE;
}
