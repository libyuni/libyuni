#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
# define __YUNI_CORE_FUNCTIONAL_VIEW_HXX__

# include "fold.h"
# include "binaryfunctions.h"


namespace Yuni
{
namespace Functional
{


	template<class ContainerT>
	class View
	{
	public:
		//! Main Constructor
		View(const ContainerT& container):
			pData(container)
		{}

		template<class T>
		T max(const T& initVal = 0) const
		{
			return fold(pData, initVal, Max<T>());
		}

		template<class T>
		T min(const T& initVal) const // TODO : std::limits ?
		{
			return fold(pData, initVal, Min<T>());
		}

		template<class T>
		T sum(const T& initVal = 0) const
		{
			return fold(pData, initVal, Add<T>());
		}

		template<class T>
		T diff(const T& initVal) const
		{
			return fold(pData, initVal, Sub<T>());
		}

		template<class T>
		T product(const T& initVal = 1) const
		{
			return fold(pData, initVal, Mul<T>());
		}

		template<class T>
		T quotient(const T& initVal) const
		{
			return fold(pData, initVal, Div<T>());
		}

		template<class T>
		T average(const T& initVal = 0) const
		{
			return fold(pData, initVal, Add<T>()) / static_cast<T>(pData.size());
		}

		template<class T>
		T range(const T& lowerBound) const
		{
			T max = max(lowerBound);
			T min = fold(pData, max, Min<T>());
			return max - min;
		}

	private:
		const ContainerT& pData;
	};


} // namespace Functional



	template<class ContainerT>
	Functional::View<ContainerT> makeView(const ContainerT& container)
	{
		return Functional::View<ContainerT>(container);
	}



} // namespace Yuni



#endif // __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
