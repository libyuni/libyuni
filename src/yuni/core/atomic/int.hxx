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
#include "int.h"


namespace Yuni
{
namespace Atomic
{

	template<int Size, template<class> class TP>
	inline Int<Size,TP>::Int() :
		# if YUNI_ATOMIC_MUST_USE_MUTEX != 1
		pValue()
		# else
		TP<Int<Size,TP> >(), pValue()
		# endif
	{}


	template<int Size, template<class> class TP>
	inline Int<Size,TP>::Int(sint16 v) :
		# if YUNI_ATOMIC_MUST_USE_MUTEX != 1
		pValue((ScalarType)v)
		# else
		TP<Int<Size,TP> >(), pValue((ScalarType)v)
		# endif
	{}

	template<int Size, template<class> class TP>
	inline Int<Size,TP>::Int(sint32 v) :
		# if YUNI_ATOMIC_MUST_USE_MUTEX != 1
		pValue((ScalarType)v)
		# else
		TP<Int<Size,TP> >(), pValue((ScalarType)v)
		# endif
	{}


	template<int Size, template<class> class TP>
	inline Int<Size,TP>::Int(sint64 v) :
		# if YUNI_ATOMIC_MUST_USE_MUTEX != 1
		pValue((ScalarType)v)
		# else
		TP<Int<Size,TP> >(), pValue((ScalarType)v)
		# endif
	{}




	template<int Size, template<class> class TP>
	inline Int<Size,TP>::Int(const Int<Size,TP>& v) :
		# if YUNI_ATOMIC_MUST_USE_MUTEX != 1
		pValue((ScalarType)v)
		# else
		TP<Int<Size,TP> >(), pValue((ScalarType)v.pValue)
		# endif
	{
	}

	template<int Size, template<class> class TP>
	template<int Size2, template<class> class TP2>
	inline Int<Size,TP>::Int(const Int<Size2,TP2>& v) :
		# if YUNI_ATOMIC_MUST_USE_MUTEX != 1
		pValue()
		# else
		TP<Int<Size,TP> >(), pValue()
		# endif
	{
		if (threadSafe)
			Private::AtomicImpl::Operator<size,TP>::Increment(*this, (ScalarType) v.pValue);
		else
			pValue = (ScalarType)v.pValue;
	}


	template<int Size, template<class> class TP>
	inline Int<Size,TP>::operator ScalarType () const
	{
		return (threadSafe)
			? Private::AtomicImpl::Operator<size,TP>::Increment(*this, 0)
			: (pValue);
	}


	template<int Size, template<class> class TP>
	inline typename Int<Size,TP>::ScalarType Int<Size,TP>::operator ++ ()
	{
		return (threadSafe)
			? Private::AtomicImpl::Operator<size,TP>::Increment(*this)
			: (++pValue);
	}


	template<int Size, template<class> class TP>
	inline typename Int<Size,TP>::ScalarType Int<Size,TP>::operator -- ()
	{
		return (threadSafe)
			? Private::AtomicImpl::Operator<size,TP>::Decrement(*this)
			: (--pValue);
	}


	template<int Size, template<class> class TP>
	inline typename Int<Size,TP>::ScalarType Int<Size,TP>::operator ++ (int)
	{
		return (threadSafe)
			? Private::AtomicImpl::Operator<size,TP>::Increment(*this) - 1
			: (pValue++);
	}


	template<int Size, template<class> class TP>
	inline typename Int<Size,TP>::ScalarType Int<Size,TP>::operator -- (int)
	{
		return (threadSafe)
			? Private::AtomicImpl::Operator<size,TP>::Decrement(*this) + 1
			: (pValue--);
	}


	template<int Size, template<class> class TP>
	inline bool Int<Size,TP>::operator ! () const
	{
		return (threadSafe)
			? (0 == Private::AtomicImpl::Operator<size,TP>::Increment(*this, 0))
			: (0 == pValue);

	}


	template<int Size, template<class> class TP>
	inline Int<Size,TP>& Int<Size,TP>::operator = (const ScalarType& v)
	{
		if (threadSafe)
			Private::AtomicImpl::Operator<size,TP>::Set(*this, v);
		else
			pValue = v;
		return *this;
	}


	template<int Size, template<class> class TP>
	inline Int<Size,TP>& Int<Size,TP>::operator += (const ScalarType& v)
	{
		if (threadSafe)
			Private::AtomicImpl::Operator<size,TP>::Increment(*this, v);
		else
			pValue += v;
		return *this;
	}


	template<int Size, template<class> class TP>
	inline Int<Size,TP>& Int<Size,TP>::operator -= (const ScalarType& v)
	{
		if (threadSafe)
			Private::AtomicImpl::Operator<size,TP>::Decrement(*this, v);
		else
			pValue -= v;
		return *this;
	}


	template<int Size, template<class> class TP>
	inline void Int<Size,TP>::zero()
	{
		if (threadSafe)
			Private::AtomicImpl::Operator<size,TP>::Zero(*this);
		else
			pValue = 0;
	}






} // namespace Atomic
} // namespace Yuni
