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
		typedef typename Loop<CollectionT>::EltType EltType;
		typedef CollectionT CollType;

	public:
		//! Main Constructor
		View(const CollectionT& collection):
			pLoop(collection)
		{}

		template<class IteratorT>
		View(const IteratorT& begin,
			 const IteratorT& end):
			pLoop(begin, end)
		{}

		View(uint startIdx, uint endIdx, const CollectionT& collection):
			pLoop(startIdx, endIdx, collection)
		{}

		//! Copy constructor
		template<class PredicateT>
		View(const View<CollectionT>& other, const PredicateT& predicate):
			pLoop(other.pLoop)
		{}


		bool empty() const { return pLoop.empty(); }

		bool next() const { return pLoop.next(); }

		const EltType& current() const { return pLoop.current(); }


		//! Loop on all elements
		template<class T, class CallbackT>
		void each(const CallbackT& callback) const
		{
			if (pLoop.empty())
				return;
			do
			{
				if (not callback(pLoop.current()))
					break;
			} while (pLoop.next());
		}


		//! View filtering
		template<class PredicateT>
		View<View<CollectionT> > filter(const PredicateT& predicate) const
		{
			return View<View<CollectionT> >(*this, predicate);
		}


		//! User-defined folding
		template<class ResultT, class AccumulatorT>
		ResultT fold(const ResultT& initval, const AccumulatorT& accumulate) const
		{
			ResultT result = initval;

			if (pLoop.empty())
				return result;
			do
			{
				if (not accumulate(result, pLoop.current()))
					break;
			} while (pLoop.next());
			return result;
		}



		//! Pre-defined folding : maximum
		template<class ResultT>
		ResultT max(const ResultT& initVal = 0) const
		{
			return fold(initVal, Max<ResultT>());
		}

		//! Pre-defined folding : minimum
		template<class ResultT>
		ResultT min(const ResultT& initVal) const // TODO : std::limits ?
		{
			return fold(initVal, Min<ResultT>());
		}

		//! Pre-defined folding : sum
		template<class ResultT>
		ResultT sum(const ResultT& initVal = 0) const
		{
			return fold(initVal, Add<ResultT>());
		}

		//! Pre-defined folding : difference
		template<class ResultT>
		ResultT diff(const ResultT& initVal) const
		{
			return fold(initVal, Sub<ResultT>());
		}

		//! Pre-defined folding : product
		template<class ResultT>
		ResultT product(const ResultT& initVal = 1) const
		{
			return fold(initVal, Mul<ResultT>());
		}

		//! Pre-defined folding : quotient
		template<class ResultT>
		ResultT quotient(const ResultT& initVal) const
		{
			return fold(initVal, Div<ResultT>());
		}

		//! Pre-defined folding : mean / mathematical average
		/*
		template<class ResultT>
		ResultT average(const ResultT& initVal = 0) const
		{
			return fold(initVal, Add<ResultT>()) / static_cast<ResultT>(pData.size());
		}
		*/

		//! Pre-defined folding : value range (maximum - minimum)
		template<class ResultT>
		ResultT range(const ResultT& lowerBound) const
		{
			ResultT max = max(lowerBound);
			ResultT min = min(max);
			return max - min;
		}

	private:
		mutable Loop<CollectionT> pLoop;
	};


} // namespace Functional




	//! Create a view on a collection
	template<class CollectionT>
	Functional::View<CollectionT> makeView(const CollectionT& collection)
	{
		return Functional::View<CollectionT>(collection);
	}



	//! Create a view on an array slice
	template<class T, int N>
	Functional::View<T[N]> makeView(uint start, uint end, const T collection[N])
	{
		return Functional::View<T[N]>(collection);
	}


	//! Create a view from const iterators
	template<class CollectionT>
	Functional::View<CollectionT> makeView(const typename CollectionT::const_iterator& start,
		const typename CollectionT::const_iterator& end)
	{
		return Functional::View<CollectionT>(start, end);
	}


	//! Create a view from iterators
	template<class CollectionT>
	Functional::View<CollectionT> makeView(const typename CollectionT::iterator& start,
		const typename CollectionT::iterator& end)
	{
		return Functional::View<CollectionT>(start, end);
	}


} // namespace Yuni



#endif // __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
