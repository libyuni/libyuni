/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
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
	public:
		enum { hasMapping = false };
	};



	//! Generic loop for basic STL collections.
	template<class T, class Other, template <class, class> class CollectionT>
	class Loop<CollectionT<T, Other> > final
	{
	public:
		typedef CollectionT<T, Other>  CollType;
		typedef T  ElementType;
		enum { hasMapping = false };

	public:
		Loop(const CollType& collection):
			begin(collection.begin()),
			it(collection.begin()),
			end(collection.end())
		{}

		void reset() { it = begin; }

		bool empty() const { return it == end; }

		bool next()
		{
			++it;
			return it != end;
		}

		//T& current() { return *it; }
		const T& current() const { return *it; }

	private:
		const typename CollType::const_iterator begin;
		typename CollType::const_iterator it;
		const typename CollType::const_iterator end;

	}; // class Loop



	//! Generic loop for T* null-terminated !
	template<class T>
	class Loop<T*> final
	{
	public:
		typedef T  ElementType;
		enum { hasMapping = false };

	public:
		Loop(const T* const& collection):
			start(collection),
			ptr(collection)
		{}

		void reset() { ptr = start; }

		bool empty() const { return 0 == *ptr; }

		bool next()
		{
			++ptr;
			return 0 != *ptr;
		}

		//T& current() { return *ptr; }
		const T& current() const { return *ptr; }

	private:
		const T* const start;
		const T* ptr;

	}; // class Loop



	//! Generic loop for const T* null-terminated !
	template<class T>
	class Loop<const T*> final
	{
	public:
		typedef T  ElementType;
		enum { hasMapping = false };

	public:
		Loop(const T* const& collection):
			start(collection),
			ptr(collection)
		{}

		void reset() { ptr = start; }

		bool empty() const { return 0 == *ptr; }

		bool next()
		{
			++ptr;
			return 0 != *ptr;
		}

		//T& current() { return *ptr; }
		const T& current() const { return *ptr; }

	private:
		const T* const& start;
		const T* const& ptr;

	}; // class Loop



	//! Generic loop for T[N]
	template<class T, int N>
	class Loop<T[N]> final
	{
	public:
		typedef T  ElementType;
		enum { hasMapping = false };

	public:
		Loop(const T collection[N]):
			start(0u),
			i(0u),
			end((uint)-1),
			data(collection)
		{}

		Loop(uint startIdx, uint endIdx, const T collection[N]):
			start(startIdx),
			i(startIdx),
			end(endIdx),
			data(collection)
		{
		}

		void reset() { i = start; }

		bool empty() const { return i >= Math::Min((uint)N, end); }

		bool next()
		{
			++i;
			return i < Math::Min((uint)N, end);
		}

		//T& current() { return data[i]; }
		const T& current() const { return data[i]; }

	private:
		uint start;
		uint i;
		uint end;
		const T* const data;

	}; // class Loop



	template<class BeginT, class EndT>
	class LoopIterator final
	{
	public:
		typedef typename BeginT::value_type  ElementType;
		enum { hasMapping = false };

	public:
		LoopIterator(const BeginT& itBegin, const EndT& itEnd):
			begin(itBegin),
			it(itBegin),
			end(itEnd)
		{}

		void reset() { it = begin; }

		bool empty() const { return it == end; }

		bool next()
		{
			++it;
			return it != end;
		}

		//ElementType& current() { return *it; }
		const ElementType& current() const { return *it; }

	private:
		BeginT begin;
		BeginT it;
		EndT end;
	};



} // namespace Functional
} // namespace Yuni


#endif // __YUNI_CORE_FUNCTIONAL_LOOP_H__
