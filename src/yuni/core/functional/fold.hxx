#ifndef __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
# define __YUNI_CORE_FUNCTIONAL_FOLD_HXX__


namespace Yuni
{
namespace Functional
{

	template<class T, class ContainerT, class FunctorT>
	T Fold(const ContainerT& container, const T& defval, const FunctorT& callback)
	{
		const typename ContainerT::const_iterator end = container.end();
		typename ContainerT::const_iterator i   = container.begin();

		T result = defval;
		for (; i != end; ++i)
		{
			if (not callback(result, *i))
				return result;
		}

		return result;
	}




} // namespace Functional
} // namespace Yuni

#endif // __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
