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
#include "../../yuni.h"
#include "remove.h"


/*!
** \brief Macro for implementing a check for Determining whether a class has a specific typedef or not
*/
#define YUNI_IMPL_STATIC_HAS_TYPEDEF(TYPEDEF)  \
		template<class T> \
		class TYPEDEF final \
		{ \
			typedef typename ::Yuni::Static::Remove::All<T>::Type  Type; \
			typedef struct {char a[2];}   TrueType; \
			typedef char  FalseType; \
			template<class C> static TrueType  deduce(typename C::TYPEDEF const *); \
			template<class C> static FalseType deduce(...); \
		\
		public: \
			enum \
			{ \
				yes = sizeof(deduce<Type>(0)) == sizeof(TrueType), \
				no = !yes, \
			}; \
		} /* class */






namespace Yuni
{
namespace Static
{
namespace HasTypedef
{

	/*!
	** \brief Determine whether a class has the typedef `Ptr` or not
	**
	** \code
	** std::cout << Static::HasTypedef::Ptr<MyClass>::yes << std::endl;
	** \endcode
	*/
	YUNI_IMPL_STATIC_HAS_TYPEDEF(Ptr);


	/*!
	** \brief Determine whether a class has the typedef `IntrusiveSmartPtrType` or not
	**
	** This test is mainly used to prevent misuse of some incompatible smartptr.
	*/
	YUNI_IMPL_STATIC_HAS_TYPEDEF(IntrusiveSmartPtrType);


	/*!
	** \brief Determine whether a class has the typedef `CStringType` or not
	**
	** This test is mainly used to detect Yuni string where templates can not be used.
	*/
	YUNI_IMPL_STATIC_HAS_TYPEDEF(CStringType);




} // namespace HasMethod
} // namespace Static
} // namespace Yuni
