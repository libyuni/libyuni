#ifndef __YUNI_CORE_FUNCTIONAL_FOLD_HXX__
# define __YUNI_CORE_FUNCTIONAL_FOLD_HXX__

# include "binaryfunctions.h"


namespace Yuni
{


	template<class T, class ContainerT>
	class FoldingData
	{
	public:
		FoldingData(const ContainerT& container):
			pData(container)
		{}

		T max(const T& initVal = 0)
		{
			return fold(pData, initVal, Functional::Max<T>());
		}

		T min(const T& initVal) // TODO : std::limits ?
		{
			return fold(pData, initVal, Functional::Min<T>());
		}

		T sum(const T& initVal = 0)
		{
			return fold(pData, initVal, Functional::Add<T>());
		}

		T diff(const T& initVal)
		{
			return fold(pData, initVal, Functional::Sub<T>());
		}

		T product(const T& initVal = 1)
		{
			return fold(pData, initVal, Functional::Mul<T>());
		}

		T quotient(const T& initVal)
		{
			return fold(pData, initVal, Functional::Div<T>());
		}

	private:
		const ContainerT& pData;
	};



	template<class T, class ContainerT>
	FoldingData<T, ContainerT> fold(const ContainerT& container)
	{
		return FoldingData<T, ContainerT>(container);
	}


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
