
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

#ifdef FEMAP11

	// Make another view on the same container, and iterate on it using `each()`
	makeView(list).each([](const uint& val) -> bool
		{
			std::cout << "Value : " << val << std::endl;
			return true;
		});

	// It is possible to eliminate some elements from a view using `filter()`
	std::cout << "Number of values >= 10 : " << makeView(vect)
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

	// It is necessary to reset a view before iterating on it a second time
	view.reset();

	// Value range is (Max - Min) calculated in a single iteration
	std::cout << "Range : " << view.valueRange() << std::endl;

#endif

	// Any type of element is acceptable for the container
	std::vector<const char*> list2 = { "ab", "ra", "ca", "da", "bra" };

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
		.sum();

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

	delete list3;

	return 0;
}

