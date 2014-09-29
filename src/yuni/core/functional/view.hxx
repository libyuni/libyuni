#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
# define __YUNI_CORE_FUNCTIONAL_VIEW_HXX__

# include "binaryfunctions.h"


namespace Yuni
{
namespace Functional
{



	template<
		class ParentT,
		class FilteringT = FilteringPolicy::AcceptAll<typename ParentT::EltType>,
		class MappingT = MappingPolicy::Identity<typename ParentT::EltType>
	>
	class BaseView final :
		public FilteringT,
		public MappingT
	{
	public:
		typedef typename ParentT::EltType  EltType;
		typedef ParentT  ParentType;
		typedef BaseView<ParentT, FilteringT, MappingT>  ThisType;

	public:
		//! Main Constructor
		BaseView():
			FilteringT(None()),
			MappingT(None())
		{}
		/*
		template<class IteratorT>
		BaseView(const IteratorT& begin,
			 const IteratorT& end):
			FilteringT(None()),
			MappingT(None())
		{}

		BaseView(uint startIdx, uint endIdx, const CollectionT& collection):
			pLoop(startIdx, endIdx, collection)
		{}
		*/

		//! Copy constructor for filtering
		template<class PredicateT>
		BaseView(const PredicateT& filter):
			FilteringT(filter),
			MappingT(None())
		{}

		bool empty() const { return static_cast<ParentT*>(this)->empty(); }

		bool next() const { return static_cast<ParentT*>(this)->next(); }

		const EltType& current() const { return static_cast<ParentT*>(this)->current(); }


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
		FilterView<ThisType, FilteringPolicy::Function<EltType, PredicateT> >
		filter(const PredicateT& predicate) const
		{
			return FilterView<ThisType, FilteringPolicy::Function<EltType, PredicateT> >(*this, predicate);
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
	};



	//! Base template
	template<class CollectionT>
	class View final: public BaseView<View<CollectionT> >
	{};



	//! Generic loop for nested view
	template<class CollectionT>
	class View<View<CollectionT> > final: public BaseView<View<CollectionT> >
	{
	public:
		typedef CollectionT CollType;
		typedef typename View<CollectionT>::EltType EltType;

	public:
		View(const View<CollectionT>& collection):
			pCollection(collection)
		{}

		bool empty() const { return pCollection.empty(); }

		bool next()
		{
			return pCollection.next();
		}

		const typename View<CollectionT>::EltType& current() const
		{
			return pCollection.current();
		}

	private:
		const View<CollectionT>& pCollection;

	}; // class View



	//! Generic loop for basic STL collections.
	template<class T, class Other, template <class, class> class CollectionT>
	class View<CollectionT<T, Other> > final: public BaseView<View<CollectionT<T, Other> >
	{
	public:
		typedef CollectionT<T, Other>  CollType;
		typedef T  EltType;

	public:
		View(const CollType& collection):
			it(collection.begin()),
			end(collection.end())
		{}

		View(const typename CollType::const_iterator& itBegin,
			 const typename CollType::const_iterator& itEnd):
			it(itBegin),
			end(itEnd)
		{}

		bool empty() const { return it == end; }

		bool next()
		{
			++it;
			return it != end;
		}

		const T& current() const { return *it; }

	private:
		typename CollType::const_iterator it;
		const typename CollType::const_iterator end;

	}; // class View



	//! Generic loop for T* null-terminated !
	template<class T>
	class View<T*> final: public BaseView<View<T*> >
	{
	public:
		typedef T*  CollType;
		typedef T  EltType;

	public:
		View(const T* const& collection):
			ptr(collection)
		{}

		bool empty() const { return 0 == *ptr; }

		bool next()
		{
			++ptr;
			return 0 != *ptr;
		}

		const T& current() const { return *ptr; }

	private:
		const T* ptr;

	}; // class View



	//! Generic loop for const T* null-terminated !
	template<class T>
	class View<const T*> final: public BaseView<View<const T*> >
	{
	public:
		typedef const T*  CollType;
		typedef T  EltType;

	public:
		View(const T* const& collection):
			ptr(collection)
		{}

		bool empty() const { return 0 == *ptr; }

		bool next()
		{
			++ptr;
			return 0 != *ptr;
		}

		const T& current() const { return *ptr; }

	private:
		const T* const& ptr;

	}; // class View



	//! Generic loop for T[N]
	template<class T, int N>
	class View<T[N]> final: public BaseView<View<T[N]> >
	{
	public:
		typedef T  CollType[N];
		typedef T  EltType;

	public:
		View(const T collection[N]):
			i(0u),
			end((uint)-1),
			data(collection)
		{}

		View(uint startIdx, uint endIdx):
			i(startIdx)
		{
		}

		bool empty() const { return i >= Math::Min(N, end); }

		bool next()
		{
			++i;
			return i < Math::Min(N, end);
		}

		const T& current() const { return data[i]; }

	private:
		uint i;
		uint end;
		const T* data;

	}; // class View



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
