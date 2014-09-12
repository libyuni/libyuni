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
	template<class ResultT, class ContainerT, class AccumulatorT>
	YUNI_DECL ResultT fold(const ContainerT& container, const ResultT& initval, const AccumulatorT& callback);



} // namespace Yuni

# include "fold.hxx"

#endif // __YUNI_CORE_FUNCTIONAL_FOLD_H__
