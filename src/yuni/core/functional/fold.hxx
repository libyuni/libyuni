#ifndef __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
# define __YUNI_CORE_FUNCTIONAL_FOLD_HXX__

# include "view.h"


namespace Yuni
{



	template<class ResultT, class CollectionT, class AccumulatorT>
	ResultT fold(const CollectionT& collection, const ResultT& initval, const AccumulatorT& callback)
	{
		ResultT result = initval;
		Functional::View<CollectionT> loop(collection);
		if (loop.empty())
			return result;
		do
		{
			if (not callback(result, loop.current()))
				break;
		} while (loop.next());
		return result;
	}




} // namespace Yuni

#endif // __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
