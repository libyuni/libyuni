#ifndef __YUNI_CORE_FUNCTIONAL_LOOP_H__
# define __YUNI_CORE_FUNCTIONAL_LOOP_H__

# include "../static/remove.h"


namespace Yuni
{
namespace Functional
{


	template<class CollectionT>
	class Loop final
	{
	}; // class Loop



	//! Generic loop for basic STL collections.
	template<class T, class Other, template <class, class> class CollectionT>
	class Loop<CollectionT<T, Other> > final
	{
	private:
		typedef CollectionT<T, Other>  CollType;

	public:
		Loop(const CollType& collection):
			it(collection.begin()),
			end(collection.end())
		{}

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



	//! Generic loop for T*
	template<class T>
	class Loop<T*> final
	{
	public:
		Loop(const T* const& collection):
			ptr(collection)
		{}

		bool next()
		{
			++ptr;
			return 0 != *ptr;
		}

		const T& current() const { return *ptr; }

	private:
		const T* ptr;

	}; // class Loop



	//! Generic loop for const T*
	template<class T>
	class Loop<const T*> final
	{
	public:
		Loop(const T* const& collection):
			ptr(collection)
		{}

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
		Loop(const T collection[N]):
			i(0u),
			data(collection)
		{}

		bool next()
		{
			++i;
			return i < N;
		}

		const T& current() const { return data[i]; }

	private:
		uint i;
		const T* data;

	}; // class Loop




} // namespace Functional
} // namespace Yuni


#endif // __YUNI_CORE_FUNCTIONAL_LOOP_H__
