#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_H__
# define __YUNI_CORE_FUNCTIONAL_VIEW_H__

# include "../../yuni.h"
# include "viewpolicies.h"


namespace Yuni
{
namespace Functional
{


	//! A view is a virtual representation of a collection
	template<class CollectionT>
	class View;


	template<class CollectionT, class FilteringPolicy>
	class FilterView;


	template<class CollectionT, class MappingT>
	class MappingView;


} // namespace Functional



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



} // namespace Yuni


# include "view.hxx"


#endif // __YUNI_CORE_FUNCTIONAL_VIEW_H__
