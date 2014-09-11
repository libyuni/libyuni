#ifndef __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
# define __YUNI_CORE_FUNCTIONAL_FOLD_HXX__


namespace Yuni
{


	template<class T, class ContainerT, class FunctorT>
	T fold(const ContainerT& container, const T& initval, const FunctorT& callback)
	{
		const typename ContainerT::const_iterator end = container.end();
		typename ContainerT::const_iterator i   = container.begin();

		T result = initval;
		for (; i != end; ++i)
		{
			if (not callback(result, *i))
				return result;
		}

		return result;
	}



} // namespace Yuni

#endif // __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
