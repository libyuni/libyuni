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
#include <yuni/core/functional/fold.h>
#include <yuni/core/functional/binaryfunctions.h>
#include <iostream>


using namespace Yuni;


// `Yuni::fold()` loops on a container and aggregates each element to a result
int main(void)
{
	const uint list[] = { 10, 3, 12, 7 };
	// Use the `lambda` macro if you do not have C++11 features available
	lambda(uint& result, const uint& value, result += value; return true; )  sum;

	// Pass `fold()` the container, an initialization value, and the aggregator function
	std::cout << "Sum : " << fold(list, 0u, sum) << std::endl;

#ifdef YUNI_HAS_CPP_LAMBDA

	// If you have access to C++11 features, you may use the new lambda notation
	std::cout << "Product : "
			  << fold(list, 1u, [](uint& result, const uint& value) -> bool
				 {
					 // Multiply each element with the result
					 result *= value;
					 // Keep looping
					 return true;
				 })
			  << std::endl;

	// Yuni also defines a few frequently-used binary functions for use as aggregators
	std::cout << "Max : " << fold(list, 1u, Functional::Max<uint>()) << std::endl;

#endif

	return 0;
}

