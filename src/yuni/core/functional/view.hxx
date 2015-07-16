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
#ifndef __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
# define __YUNI_CORE_FUNCTIONAL_VIEW_HXX__

# include "../static/if.h"
# include "binaryfunctions.h"
# include "loop.h"

# ifdef YUNI_HAS_CPP_LAMBDA
#	include <utility>
# endif


namespace Yuni
{
namespace Functional
{

# ifdef YUNI_HAS_CPP_LAMBDA
	//! Forward declaration
	template<class T, class CallbackT>
	struct Mapping;
# endif


	//! Filter selects elements from the view that fulfill a certain predicate
	template<class T, class CallbackT>
	struct Filter final
	{
	public:
		typedef T  LoopType;
		typedef typename LoopType::ElementType  ElementType;
		typedef Filter<T, CallbackT>  Type;

		// If there is a mapping somewhere before, we must not pass by reference
		enum { hasMapping = LoopType::hasMapping };
		typedef typename Static::If<hasMapping, ElementType, const ElementType&>::Type  ResultT;

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

		// By copy or reference depending on whether there was a mapping before
		ResultT current() const { return pLoop.current(); }

	private:
		LoopType pLoop;
		const CallbackT& pCallback;
	};



# ifdef YUNI_HAS_CPP_LAMBDA


	template<class T, class CallbackT>
	struct Mapping final
	{
	public:
		typedef T  LoopType;
		typedef decltype(std::declval<CallbackT>()(std::declval<T>().current()))  ElementType;
		typedef Mapping<T, CallbackT>  Type;

		enum { hasMapping = true };

	public:
		Mapping(const LoopType& loop, const CallbackT& callback)
			: pLoop(loop)
			, pCallback(callback)
		{}

		void reset() { pLoop.reset(); }

		bool empty() const { return pLoop.empty(); }

		bool next() { return pLoop.next(); }

		// Always by copy
		ElementType current() const { return pCallback(pLoop.current()); }

	private:
		LoopType pLoop;
		const CallbackT& pCallback;
	};

# endif


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
		typedef typename Static::If<LoopType::hasMapping, ElementType, const ElementType&>::Type  ResultT;

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
		//ElementType& current() { return pLoop.current(); }
		ResultT current() const { return pLoop.current(); }

		//! View filtering
		template<class C>
		View<Filter<LoopType, C>, false> filter(const C& callback) const
		{
			return Filter<LoopType, C>(pLoop, callback);
		}

#ifdef YUNI_HAS_CPP_LAMBDA
		//! View mapping
		template<class C>
		View<Mapping<LoopType, C>, false> map(const C& callback) const
		{
			return Mapping<LoopType, C>(pLoop, callback);
		}
#endif

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
		template<class FoldedT, class AccumulatorT>
		FoldedT fold(const FoldedT& initval, const AccumulatorT& accumulate)
		{
			FoldedT result = initval;

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
		uint count()
		{
			return map([](const ElementType&) -> uint { return 1; }).sum();
		}


		//! Pre-defined folding : maximum
		ElementType max(const ElementType& lowBound = std::numeric_limits<ElementType>::min())
		{
			return fold(lowBound, Max<ElementType>());
		}

		//! Pre-defined folding : minimum
		ElementType min(const ElementType& highBound = std::numeric_limits<ElementType>::max())
		{
			return fold(highBound, Min<ElementType>());
		}

		//! Pre-defined folding : sum
		ElementType sum(const ElementType& initVal = 0)
		{
			return fold(initVal, Add<ElementType>());
		}

		//! Pre-defined folding : difference
		ElementType diff(const ElementType& initVal)
		{
			return fold(initVal, Sub<ElementType>());
		}

		//! Pre-defined folding : product
		ElementType product(const ElementType& initVal = 1)
		{
			return fold(initVal, Mul<ElementType>());
		}

		//! Pre-defined folding : quotient
		ElementType quotient(const ElementType& initVal)
		{
			return fold(initVal, Div<ElementType>());
		}

		//! Pre-defined folding : mean / mathematical average
		template<class FoldedT = ElementType>
		FoldedT average()
		{
			uint size = 0u;
			return fold((FoldedT)0, [&size](FoldedT& result, const ElementType& elt) -> bool
			{
				Add<FoldedT>()(result, elt);
				++size;
				return true;
			}) / (FoldedT)size;
		}

		//! Pre-defined folding : both the min and the max in a single pass
		std::pair<ElementType, ElementType> minMax(
			const ElementType& lowBound = std::numeric_limits<ElementType>::min(),
			const ElementType& highBound = std::numeric_limits<ElementType>::max())
		{
			return fold(std::pair<ElementType, ElementType>(highBound, lowBound),
				[](std::pair<ElementType, ElementType>& minMax, const ElementType& elt) -> bool
			{
				Min<ElementType>()(minMax.first, elt);
				Max<ElementType>()(minMax.second, elt);
				return true;
			});
		}

		//! Pre-defined folding : value range (maximum - minimum)
		ElementType valueRange()
		{
			std::pair<ElementType, ElementType> bounds = minMax();
			return bounds.second - bounds.first;
		}

	public:
		LoopType pLoop;
	};




} // namespace Functional
} // namespace Yuni



#endif // __YUNI_CORE_FUNCTIONAL_VIEW_HXX__
