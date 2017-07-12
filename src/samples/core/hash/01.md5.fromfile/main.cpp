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
#include <yuni/yuni.h>                      // The Yuni Global header
#include <yuni/core/hash/checksum/md5.h> // The MD5 class
#include <iostream>	                        // Only for std::cout


int main(int /*argc*/, char* argv[])
{
	// Our MD5 class
	Yuni::Hash::Checksum::MD5 md5;
	// Print the value on the std::cout
	std::cout << "MD5 (" << argv[0] << ") = " << md5.fromFile(argv[0]) << std::endl;

	return 0;
}
