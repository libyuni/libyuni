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
#ifndef __YUNI_CORE_STATIC_INHERIT_H__
# define __YUNI_CORE_STATIC_INHERIT_H__

# include "../../yuni.h"


namespace Yuni
{
namespace Static
{

	/*!
	** \brief Statically check if an arbitrary class C is a descendant of another one
	**
	** \code
	** #include <yuni/static/inherit.h>
	** #include <iostream>
	**
	** class Base {};
	**
	** class A : public Base {};
	**
	** class B {};
	**
	** int main()
	** {
	**	std::cout << "A inherits from Base :" << Yuni::Static::InheritsFrom<A,Base>::Yes << "\n";
	**	std::cout << "B inherits from Base :" << Yuni::Static::InheritsFrom<B,Base>::Yes << "\n";
	**	return 0;
	** }
	** \endcode
	**
	** \tparam C Any class (may be virtual)
	** \tparam BaseT The base class to compare with (may be virtual)
	*/
	template<class C, class BaseT>
	class InheritsFrom final
	{
	private:
		typedef int   TrueType;
		typedef char  FalseType;

		// C obviously inherits from BaseT
		static TrueType  deduce(const BaseT*);
		// C does not inherits from BaseT
		static FalseType deduce(...);
		// A dummy routine, for returning a type C
		static C* Helper();

	public:
		enum
		{
			//! A non-zero value if C inherits from BaseT
			Yes = sizeof(deduce(Helper())) == sizeof(TrueType),
			//! A non-zero value if C does not inherit from BaseT
			No = sizeof(deduce(Helper())) == sizeof(FalseType),
		};
	}; // class InheritsFrom





} // namespace Static
} // namespace Yuni

#endif // __YUNI_CORE_STATIC_INHERIT_H__
