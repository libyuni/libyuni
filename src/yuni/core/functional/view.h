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



namespace Yuni
{
namespace Functional
{


	//! A view is a virtual representation of a collection
	template<class T, bool UserTypeT = true>
	struct View;



} // namespace Functional
} // namespace Yuni



#include "view.hxx"



namespace Yuni
{

	/*!
	** \brief Create a view on a collection
	** \ingroup Functional
	**
	** \code
	** uint maxValue = makeView(list).max();
	** \endcode
	*/
	template<class CollectionT>
	YUNI_DECL Functional::View<CollectionT> makeView(const CollectionT& collection);


	//! Create a view on a collection
	template<class CollectionT>
	Functional::View<CollectionT> makeView(const CollectionT& collection)
	{
		return Functional::View<CollectionT>(collection);
	}


	//! Create a view on an array slice
	template<class T, int N>
	Functional::View<T[N]> makeView(uint start, uint end, const T collection[N])
	{
		return Functional::View<T[N]>(collection);
	}


	//! Create a view from iterators
	template<class B, class E>
	Functional::View<Functional::LoopIterator<B, E>, false> makeView(B begin, E end)
	{
		return Functional::View<Functional::LoopIterator<B, E>, false>(begin, end);
	}




} // namespace Yuni
