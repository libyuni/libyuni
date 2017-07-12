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
#include <yuni/io/file.h>
#include <iostream>

using namespace Yuni;



int main(int, char** argv)
{
	String root;
	IO::ExtractAbsoluteFilePath(root, argv[0]);
	std::cout << "executable root folder : " << root << std::endl;
	return 0;
}

