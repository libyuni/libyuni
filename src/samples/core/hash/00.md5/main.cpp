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
#include <yuni/yuni.h>   // The Yuni Global header
#include <yuni/core/hash/checksum/md5.h>
#include <iostream>      // Only for std::cout

// Our string to compute into a MD5
#define STR_TO_MD5  "Hello world"



int main(void)
{
	// Our MD5 class
	Yuni::Hash::Checksum::MD5 md5;
	// Compute the MD5
	md5.fromString(STR_TO_MD5);
	// Print the value on the std::cout
	std::cout << STR_TO_MD5 << ": md5 -> " << md5.value() << std::endl;

	// A faster method would be to directly use the `fromString()` method
	// with the std::cout, like this :
	//
	// Yuni::Hash::MD5 md5;
	// std::cout << STR_TO_MD5 << ": md5 -> " << md5.fromString(STR_TO_MD5) << std::endl;

	// Those statements are equivalent :
	//
	// md5.value() and md5()
	//
	// md5.fromString(X) and md5[X]

	// Keep in mind the MD5 hash is stored and the method .value() only gets
	// the stored value

	return 0;
}
