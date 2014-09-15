#ifndef __YUNI_CORE_FUNCTIONAL_LOOP_H__
# define __YUNI_CORE_FUNCTIONAL_LOOP_H__

# include "../math.h"
# include "view.h"

namespace Yuni
{
namespace Functional
{


	template<class CollectionT>
	class Loop final
	{
	};



	template<class CollectionT>
	class Loop<View<CollectionT> > final
	{
	public:
		typedef typename View<CollectionT>::EltType EltType;
		typedef CollectionT CollType;

	public:
		Loop(const View<CollectionT>& collection):
			pCollection(collection)
		{}
		/*
		Loop(const View<CollectionT>& collection, const PredicateT& predicate):
			pCollection(collection),
			pFilter(predicate)
		{}
		*/

		bool empty() const { return pCollection.empty(); }

		bool next()
		{
			// bool ok;
			// do
			// {
			// 	ok = pCollection.next();
			// } while (ok && !pFilter(current()));
			return pCollection.next();
		}

		const typename View<CollectionT>::EltType& current() const
		{
			return pCollection.current();
		}

	private:
		const View<CollectionT>& pCollection;
		//const PredicateT& pFilter;

	}; // class Loop



	//! Generic loop for basic STL collections.
	template<class T, class Other, template <class, class> class CollectionT>
	class Loop<CollectionT<T, Other> > final
	{
	public:
		typedef CollectionT<T, Other>  CollType;
		typedef T  EltType;

	public:
		Loop(const CollType& collection):
			it(collection.begin()),
			end(collection.end())
		{}

		Loop(const typename CollType::const_iterator& itBegin,
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

	}; // class Loop



	//! Generic loop for T* null-terminated !
	template<class T>
	class Loop<T*> final
	{
	public:
		typedef T*  CollType;
		typedef T  EltType;

	public:
		Loop(const T* const& collection):
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

	}; // class Loop



	//! Generic loop for const T* null-terminated !
	template<class T>
	class Loop<const T*> final
	{
	public:
		typedef const T*  CollType;
		typedef T  EltType;

	public:
		Loop(const T* const& collection):
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

	}; // class Loop



	//! Generic loop for T[N]
	template<class T, int N>
	class Loop<T[N]> final
	{
	public:
		typedef T  CollType[N];
		typedef T  EltType;

	public:
		Loop(const T collection[N]):
			i(0u),
			end((uint)-1),
			data(collection)
		{}

		Loop(uint startIdx, uint endIdx):
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

	}; // class Loop




} // namespace Functional
} // namespace Yuni


#endif // __YUNI_CORE_FUNCTIONAL_LOOP_H__
