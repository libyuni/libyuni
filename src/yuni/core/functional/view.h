#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_H__
# define __YUNI_CORE_FUNCTIONAL_VIEW_H__


namespace Yuni
{
namespace Functional
{


	//! A view is a virtual representation of a collection
	template<class ContainerT>
	class View;


} // namespace Functional



	/*!
	** \brief Create a view on a collection
	** \ingroup Functional
	**
	** \code
	** uint maxValue = fold(data).max();
	** \endcode
	*/
	template<class ContainerT>
	YUNI_DECL Functional::View<ContainerT> makeView(const ContainerT& container);



} // namespace Yuni


# include "view.hxx"


#endif // __YUNI_CORE_FUNCTIONAL_VIEW_H__
