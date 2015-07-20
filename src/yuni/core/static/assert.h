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
#pragma once
#include "../preprocessor/std.h"




/*!
** \def YUNI_STATIC_ASSERT(X,ID)
** \brief Assert at compile time
**
**
** YUNI_STATIC_ASSERT is like assert for C/C++ (man 3 assert), except that the
** test is done at compile time.
**
** Here is an example to produce an error to prevent compilation from a missing
** implementation :
** \code
** #include <iostream>
** #include <yuni/core/static/assert.h>
**
**
** template<typename T>
** struct Dummy
** {
**	void foo()
**	{
**		YUNI_STATIC_ASSERT(false, TheImplementationIsMissing);
**	}
** };
**
** template<>
** struct Dummy<int>
** {
**	void foo()
**	{
**		std::cout << "Called Dummy<int>::foo()" << std::endl;
**	}
** };
**
** int main()
** {
**	// As the static assert is within a method and not the class itself,
**	// it is possible to instanciate the class in all cases.
**	// But this is not true for the method `foo()`.
**
**	Dummy<int> dummy0;
**	// The following statement will compile
**	dummy0.foo();
**
**	Dummy<uint> dummy1;
**	// But this one will fail
**	dummy1.foo();
**
**	// Error with gcc 3.x :
**	// ./main.cpp: In member function `void Dummy<T>::foo() [with T = uint]':
**	//	./main.cpp:36:   instantiated from here
**	//	./main.cpp:11: error: creating array with size zero (` Assert_TheImplementationIsMissing')
**
**	// Error with gcc 4.x :
**	// ./main.cpp: In member function ‘void Dummy<T>::foo() [with T = uint]’:
**	//	./main.cpp:36:   instantiated from here
**	//	./main.cpp:11: error: creating array with negative size (‘(StaticAssert_TheImplementationIsMissing::._67)-0x000000001’)
**	return 0;
** }
** \endcode
**
** This macro might be used anywhere.
** <br />
** When using several static asserts within the same scope, each ID must be
** unique.
**
** \param X An expression
** \param ID An unique ID for the scope
*/
#ifdef YUNI_HAS_CPP_STATIC_ASSERT
#	define YUNI_STATIC_ASSERT(X, ID)   static_assert((X), #ID)
#else
#	define YUNI_STATIC_ASSERT(X, ID)  \
		struct StaticAssert_##ID { \
			enum { Assert_##ID = Yuni::Static::Assert<(0 != (X))>::result }; \
		}; \
		typedef char invokeStaticAssert_##ID[StaticAssert_##ID::Assert_##ID]
#endif



namespace Yuni
{
namespace Static
{

	template<int N> struct Assert   final { enum {result =  1}; }; // No error

	template<> struct Assert<false> final { enum {result = -1}; }; // Error


} // namespace Static
} // namespaec Yuni
