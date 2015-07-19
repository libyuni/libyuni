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

