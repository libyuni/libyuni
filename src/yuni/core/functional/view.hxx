#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
# define __YUNI_CORE_FUNCTIONAL_VIEW_HXX__

# include "loop.h"
# include "fold.h"
# include "binaryfunctions.h"


namespace Yuni
{
namespace Functional
{


	template<class CollectionT>
	class View final
	{
	public:
		//! Main Constructor
		View(const CollectionT& collection):
			pData(collection)
		{}


		//! Loop on all elements
		template<class T, class CallbackT>
		void each(const CallbackT& callback) const
		{
			Loop<CollectionT> loop(pData);

			do
			{
				if (not callback(loop.current()))
					break;
			} while (loop.next());
		}


		//! User-defined folding
		template<class T, class AccumulatorT>
		T fold(const T& initval, const AccumulatorT& callback) const
		{
			return Yuni::fold(pData, initval, callback);
		}


		//! Pre-defined folding : maximum
		template<class T>
		T max(const T& initVal = 0) const
		{
			return fold(initVal, Max<T>());
		}

		//! Pre-defined folding : minimum
		template<class T>
		T min(const T& initVal) const // TODO : std::limits ?
		{
			return fold(initVal, Min<T>());
		}

		//! Pre-defined folding : sum
		template<class T>
		T sum(const T& initVal = 0) const
		{
			return fold(initVal, Add<T>());
		}

		//! Pre-defined folding : difference
		template<class T>
		T diff(const T& initVal) const
		{
			return fold(initVal, Sub<T>());
		}

		//! Pre-defined folding : product
		template<class T>
		T product(const T& initVal = 1) const
		{
			return fold(initVal, Mul<T>());
		}

		//! Pre-defined folding : quotient
		template<class T>
		T quotient(const T& initVal) const
		{
			return fold(initVal, Div<T>());
		}

		//! Pre-defined folding : mean / mathematical average
		template<class T>
		T average(const T& initVal = 0) const
		{
			return fold(initVal, Add<T>()) / static_cast<T>(pData.size());
		}

		//! Pre-defined folding : value range (maximum - minimum)
		template<class T>
		T range(const T& lowerBound) const
		{
			T max = max(lowerBound);
			T min = fold(pData, max, Min<T>());
			return max - min;
		}

	private:
		const CollectionT& pData;
	};


} // namespace Functional



	//! Create a view on a collection
	template<class CollectionT>
	Functional::View<CollectionT> makeView(const CollectionT& collection)
	{
		return Functional::View<CollectionT>(collection);
	}



} // namespace Yuni



#endif // __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
