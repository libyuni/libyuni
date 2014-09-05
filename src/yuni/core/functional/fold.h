#ifndef __YUNI_CORE_FUNCTIONAL_FOLD_H__
# define __YUNI_CORE_FUNCTIONAL_FOLD_H__


namespace Yuni
{


	/*!
	** \brief Folding implementation
	** \ingroup Functional
	**
	** \code
	** bool gotSomething = fold(data, false, [] (bool& result, const A& current) -> bool
	** {
	**	if (current.isModified())
	**	{
	**		result = true;
	**		current.doSomething();
	**	}
	**	return true;
	** });
	** \endcode
	*/
	template<class T, class ContainerT, class FunctorT>
	YUNI_DECL T fold(const ContainerT& container, const T& defval, const FunctorT& callback);




} // namespace Yuni

# include "fold.hxx"

#endif // __YUNI_CORE_FUNCTIONAL_FOLD_H__
