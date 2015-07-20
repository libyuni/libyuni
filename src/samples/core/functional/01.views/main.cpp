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
#include <yuni/core/functional/view.h>
#include <yuni/core/string.h>
#include <vector>


using namespace Yuni;


// Functional::View wraps a container and can store filter / map operations that operate on it
// The View will not be iterated on until an `each()` or `fold()` call.
int main(void)
{
	// Create a fixed-size array
	const uint list[4] = { 10, 3, 12, 7 };
	// Create a view on a container using `Yuni::makeView`
	// Fold using the pre-defined `max()` aggregator
	std::cout << "Max at " << makeView(list).max() << std::endl;

#ifdef YUNI_HAS_CPP_LAMBDA

	// Make another view on the same container, and iterate on it using `each()`
	makeView(list).each([](const uint& val) -> bool
		{
			std::cout << "Value : " << val << std::endl;
			return true;
		});

	// It is possible to eliminate some elements from a view using `filter()`
	std::cout << "Number of values >= 10 : " << makeView(list)
		.filter([](const uint& val)-> bool { return val >= 10; })
		.count() << std::endl;

	// Views work also with STL containers
	std::vector<uint> vect = { 10, 3, 12, 7 };
	auto view = makeView(vect);

	// It is also possible to make a view using iterators
	std::cout << "Average : " << makeView(vect.begin(), vect.end()).average() << std::endl;

	// Filters can be chained as many times as necessary before folding
	std::cout << "Min even value : "
			  << view.filter([](const uint& value)-> bool { return !(value % 2); })
					.filter([](const uint&)-> bool { return true; })
					.min() << std::endl;

#endif

	// It is necessary to reset a view before iterating on it a second time
	view.reset();

	// Value range is (Max - Min) calculated in a single iteration
	std::cout << "Range : " << view.valueRange() << std::endl;

	// Any type of element is acceptable for the container
	std::vector<const char*> list2 = { "ab", "ra", "ca", "da", "bra" };

#ifdef YUNI_HAS_CPP_LAMBDA

	// It is possible to `map()` each element to another value, which may be of another type
	std::cout << "Concatenated size : "
			  << makeView(list2)
		// Mapping function, here we map an object to its size
		.map([](const char* const& str) -> uint
		{
			// Creating an AnyString on each element is not efficient
			// But this is prettier than `::strlen()`
			return AnyString(str).size();
		})
		.sum()
			  << std::endl;

	// WARNING : If you want to use variable-size pointer arrays, they need to be zero-terminated !
	uint* list3 = new uint[8] { 3, 189, 42, 69, 51, 310, 21, 0 };

	// Filters and maps can be composed at will
	std::cout << "Number of values with an even number of digits : "
			  << makeView(list3)
		// Map each value to its number of digits
		.map([](const uint& value) -> uint
		{
			// This is not efficient at all ! This is just for example purposes.
			return String(value).size();
		})
		// Filter out odd values
		.filter([](const uint& value) -> bool
		{
			return !(value % 2);
		})
		// Count the number of iterations
		.count()
			  << std::endl;

	delete[] list3;

#endif

	return 0;
}

