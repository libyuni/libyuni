#ifndef __YUNI_CORE_FUNCTIONAL_FOLD_H__
# define __YUNI_CORE_FUNCTIONAL_FOLD_H__


namespace Yuni
{
namespace Functional
{

	/*!
	** \brief Folding implementation
	**
	** \code
	** bool gotSomething = Functional::Fold(data, false, [] (bool& result, const A& current) -> bool
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
	YUNI_DECL T Fold(const ContainerT& container, const T& defval, const FunctorT& callback);




} // namespace Functional
} // namespace Yuni

# include "fold.hxx"

#endif // __YUNI_CORE_FUNCTIONAL_FOLD_H__
