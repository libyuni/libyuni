#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
# define __YUNI_CORE_FUNCTIONAL_VIEW_HXX__

# include "binaryfunctions.h"


namespace Yuni
{
namespace Functional
{

	template<class T, class CallbackT>
	struct Filter final
	{
	public:
		Filter(const T& loop, const CallbackT& callback)
			: pLoop(loop)
			, pCallback(callback)
		{}
		void reset() {}
		bool empty() const {return pLoop.empty();}
		bool next() { while(pCallback(current())) {} return false; }
		ElementType& current() {}
		const ElementType& current() const {}

		template<class C>
		Filter<Filter<T>, C> filter(const C& callback)
		{
			return Filter<Filter<T>, C>(*this, callback);
		}

	private:
		T pLoop;
		const CallbackT& pCallback;
	};



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


	template<class T, bool UserTypeT = true>
	struct View final
	{
	public:
		typedef typename Static::If<UserTypeT, Loop<T>, T>::Type  LoopType;
		typedef typename LoopType::ElementType ElementType;

	public:
		View(const T& loop)
			: pLoop(loop)
		{}
		void reset() { pLoop.reset(); }
		bool empty() const {return pLoop.empty();}
		bool next() { return pLoop.next(); }
		const ElementType& current() const { return pLoop.current(); }

		template<class C>
		View<Filter<LoopType, C>, false> filter(const C& callback) const
		{
			return Filter<LoopType, C>(pLoop, callback);
		}

	public:
		LoopType pLoop;
	};


	template<class T> View<T> makeView(const T& container)
	{
		return View<T>(container);
	}

	template<class B, class E> View<LoopIterator<B, E> > makeView(B begin, E end)
	{
		return View<LoopIterator<B, E>, false>(begin, end);
	}





	// -----------------------------

	template<class T>
	struct Element final
	{
		typedef uint Type;
	};



	// comment on parcourt le container original
	template<class T>
	class Loop final
	{
	public:
		typedef typename Element<T>::Type ElementType;

	public:
		void reset() {}
		bool empty() const {return true;}
		bool next() { return false; }
		ElementType& current() {}
		const ElementType& current() const {}
	};


	template<class NextT>
	struct Identity final
	{
		typedef NextT Ancestor;

		void reset() { Ancestor::reset(); }
		bool empty() const {return Ancestor::empty();}
		bool next()
		{
			return Ancestor::next();
		}
		const ElementType& current() const {return Ancestor::current();}
	};




	template<
		class T, // container
		class FilteringT = FilteringPolicy::AcceptAll<typename ParentT::EltType>,
		class MappingT = MappingPolicy::Identity<typename ParentT::EltType>
	>
	class View final
		: public FilteringT
		, public MappingT
		, private NonCopyable<View<T, FilteringT, MappingT> >
	{
	public:
		typedef typename Element<T>::Type  ElementType;
		typedef typename Loop<T> LoopType;
		typedef typename MappingT<T>  MappingType;
		typedef typename MappingType::ReturnType  ReturnType;

	public:
		//! Main Constructor
		View(const T& container)
			: pLoop(container)
		{}

		bool empty() const { return pLoop.empty(); }

		bool next() { return pLoop.next(); }

		const EltType& current() const { return pMapping(pLoop.current()); }


		//! Loop on all elements
		template<class T, class CallbackT>
		void each(const CallbackT& callback) const
		{
			if (empty())
				return;
			do
			{
				// Filtering
				if (not accept(current()))
					continue;
				if (not callback(current()))
					break;
			} while (next());
		}


		//! View filtering
		template<class PredicateT>
		FilterView<Type, FilteringPolicy::Function<EltType, PredicateT> >
		filter(const PredicateT& predicate) const
		{
			return FilterView<Type, FilteringPolicy::Function<EltType, PredicateT> >(*this, predicate);
		}

		/*
		template<class DestT, class FuncT>
		View<
			ThisType,
			FilteringPolicy::AcceptAll<EltType, None>,
			MappingPolicy::Function<EltType, DestT, FuncT>
		>
		map(const FuncT& mapping) const
		{
			return View<ThisType, FilteringPolicy::AcceptAll<EltType, None>,
				MappingPolicy<EltType, DestT, FuncT> >(*this, mapping);
		}
		*/

		//! User-defined folding
		template<class ResultT, class AccumulatorT>
		ResultT fold(const ResultT& initval, const AccumulatorT& accumulate) const
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
		LoopType pLoop;
	};



	template<class CollectionT, class FilteringT>
	class FilterView: public BaseView<FilterView<CollectionT, FilteringT>, FilteringT>
	{};


	template<class CollectionT, class MappingT>
	class MappingView : public BaseView<
		MappingView<CollectionT, MappingT>,
		FilteringPolicy::AcceptAll<typename View<CollectionT>::EltType>,
		MappingT
	>
	{};



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
