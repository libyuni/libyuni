#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_H__
# define __YUNI_CORE_FUNCTIONAL_VIEW_H__

# include "../../yuni.h"


namespace Yuni
{
namespace Functional
{


	//! A view is a virtual representation of a collection
	template<class T, bool UserTypeT = true>
	struct View;



} // namespace Functional
} // namespace Yuni



# include "view.hxx"



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


#endif // __YUNI_CORE_FUNCTIONAL_VIEW_H__
