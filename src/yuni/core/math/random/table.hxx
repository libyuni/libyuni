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
#pragma once
#include "table.h"


namespace Yuni
{
namespace Math
{
namespace Random
{


	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline const char* Table<D,TableSize,Cyclic,TP>::Name()
	{
		return D::Name();
	}



	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline Table<D,TableSize,Cyclic,TP>::Table()
		:D(), pOffset()
	{}


	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline Table<D,TableSize,Cyclic,TP>::~Table()
	{}


	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline void
	Table<D,TableSize,Cyclic,TP>::reset()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		static_cast<D*>(this)->reset();
		fillWL();
	}


	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline void
	Table<D,TableSize,Cyclic,TP>::fill()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		fillWL();
	}

	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	void
	Table<D,TableSize,Cyclic,TP>::fillWL()
	{
		// Regenerate all items in the cache
		for (int i = 0; i != TableSize; ++i)
			pCache[i] = static_cast<D*>(this)->next();
		// The index must be reset to `-1`, in order to use the first random number
		// to the next call to `next()`.
		// See `next()` for more informations.
		pOffset = -1;
	}


	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline int
	Table<D,TableSize,Cyclic,TP>::size()
	{
		return TableSize;
	}




	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline const typename Table<D,TableSize,Cyclic,TP>::Value
	Table<D,TableSize,Cyclic,TP>::next()
	{
		// Lock
		typename ThreadingPolicy::MutexLocker locker(*this);

		// In order to keep the best performances, we have to avoid post increments
		// and temporary variables.
		// A way to achieve this is to use a pre-increment and a simple test
		// for the index, then to directly return the result.
		if (++pOffset == TableSize)
		{
			if (!Cyclic) // The random numbers must be re-generated if not cyclic
				fillWL();
			// Restting the offset, which may be equal to `TableSize` or to `-1` (see `fillWL()`)
			pOffset = 0;
			return pCache[0];
		}
		return pCache[pOffset];
	}



	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	template<class U>
	inline Table<D,TableSize,Cyclic,TP>&
	Table<D,TableSize,Cyclic,TP>::operator >> (U& u)
	{
		u = next();
		return *this;
	}


	template<class D, int TableSize, bool Cyclic, template<class> class TP>
	inline const typename Table<D,TableSize,Cyclic,TP>::Value
	Table<D,TableSize,Cyclic,TP>::operator () ()
	{
		return next();
	}





} // namespace Random
} // namespace Math
} // namespace Yuni
