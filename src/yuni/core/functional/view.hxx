#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
# define __YUNI_CORE_FUNCTIONAL_VIEW_HXX__

# include "binaryfunctions.h"
# include "loop.h"


namespace Yuni
{
namespace Functional
{

	template<class T, class CallbackT>
	struct Filter final
	{
	public:
		typedef T  LoopType;
		typedef typename LoopType::ElementType  ElementType;
		typedef Filter<T, CallbackT>  Type;

	public:
		Filter(const LoopType& loop, const CallbackT& callback)
			: pLoop(loop)
			, pCallback(callback)
		{}

		void reset() { pLoop.reset(); }

		bool empty() const { return pLoop.empty(); }

		bool next()
		{
			bool notFinished;
			do
			{
				notFinished = pLoop.next();
			}
			while (notFinished && not pCallback(pLoop.current()));
			return notFinished;
		}

		//ElementType& current() { return pLoop.current(); }
		const ElementType& current() const { return pLoop.current(); }

		template<class C>
		Filter<Type, C> filter(const C& callback)
		{
			return Filter<Type, C>(*this, callback);
		}

	private:
		LoopType pLoop;
		const CallbackT& pCallback;
	};


	/*
	template<class T>
	class Loop final
	{
	public:
		typedef uint ElementType;
		Loop(const T& loop)
			: pLoop(loop)
		{}
		void reset() {}
		bool empty() const {return true;}
		bool next() { return false; }
		ElementType& current() {}
		const ElementType& current() const {}

	private:
		const T& pLoop;
	};
	*/

	template<class T, bool UserTypeT>
	struct View final
	{
	public:
		typedef typename Static::If<UserTypeT, Loop<T>, T>::Type  LoopType;
		typedef typename LoopType::ElementType ElementType;

	public:
		View(const T& loop)
			: pLoop(loop)
		{}

		template<class BeginT, class EndT>
		View(BeginT begin, EndT end)
			: pLoop(begin, end)
		{}

		void reset() { pLoop.reset(); }
		bool empty() const {return pLoop.empty();}
		bool next() { return pLoop.next(); }
		const ElementType& current() const { return pLoop.current(); }

		//! View filtering
		template<class C>
		View<Filter<LoopType, C>, false> filter(const C& callback) const
		{
			return Filter<LoopType, C>(pLoop, callback);
		}

		//! Loop on all elements
		template<class CallbackT>
		void each(const CallbackT& callback)
		{
			if (empty())
				return;
			do
			{
				if (not callback(current()))
					break;
			} while (next());
		}

		//! User-defined folding
		template<class ResultT, class AccumulatorT>
		ResultT fold(const ResultT& initval, const AccumulatorT& accumulate)
		{
			ResultT result = initval;

			if (empty())
				return result;
			do
			{
				if (not accumulate(result, current()))
					break;
			} while (next());
			return result;
		}



		//! Pre-defined folding : maximum
		template<class ResultT>
		ResultT max(const ResultT& initVal = 0)
		{
			return fold(initVal, Max<ResultT>());
		}

		//! Pre-defined folding : minimum
		template<class ResultT>
		ResultT min(const ResultT& initVal) // TODO : std::limits ?
		{
			return fold(initVal, Min<ResultT>());
		}

		//! Pre-defined folding : sum
		template<class ResultT>
		ResultT sum(const ResultT& initVal = 0)
		{
			return fold(initVal, Add<ResultT>());
		}

		//! Pre-defined folding : difference
		template<class ResultT>
		ResultT diff(const ResultT& initVal)
		{
			return fold(initVal, Sub<ResultT>());
		}

		//! Pre-defined folding : product
		template<class ResultT>
		ResultT product(const ResultT& initVal = 1)
		{
			return fold(initVal, Mul<ResultT>());
		}

		//! Pre-defined folding : quotient
		template<class ResultT>
		ResultT quotient(const ResultT& initVal)
		{
			return fold(initVal, Div<ResultT>());
		}

		//! Pre-defined folding : mean / mathematical average
		// template<class ResultT>
		// ResultT average(const ResultT& initVal = 0) const
		// {
		// 	return fold(initVal, Add<ResultT>()) / static_cast<ResultT>(pData.size());
		// }

		//! Pre-defined folding : value range (maximum - minimum)
		template<class ResultT>
		ResultT range(const ResultT& lowerBound)
		{
			ResultT max = max(lowerBound);
			ResultT min = min(max);
			return max - min;
		}

	public:
		LoopType pLoop;
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
	template<class B, class E>
	Functional::View<Functional::LoopIterator<B, E> > makeView(B begin, E end)
	{
		return Functional::View<Functional::LoopIterator<B, E>, true>(begin, end);
	}



} // namespace Yuni



#endif // __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
