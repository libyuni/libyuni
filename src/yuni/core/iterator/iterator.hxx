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
#include "iterator.h"



namespace Yuni
{

	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>::IIterator() :
		AncestorType()
	{}


	template<class ModelT, bool ConstT>
	template<class M, bool C>
	inline IIterator<ModelT,ConstT>::IIterator(const IIterator<M,C>& rhs) :
		AncestorType(static_cast<const typename IIterator<M,C>::AncestorType&>(rhs))
	{}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>::IIterator(const IIterator<ModelT,ConstT>& rhs) :
		AncestorType(static_cast<const AncestorType&>(rhs))
	{}


	template<class ModelT, bool ConstT>
	template<class U1>
	inline IIterator<ModelT,ConstT>::IIterator(const U1& u1) :
		AncestorType(u1)
	{}


	template<class ModelT, bool ConstT>
	template<class U1, class U2>
	inline IIterator<ModelT,ConstT>::IIterator(const U1& u1, const U2& u2) :
		AncestorType(u1, u2)
	{}


	template<class ModelT, bool ConstT>
	template<class U1, class U2, class U3>
	inline IIterator<ModelT,ConstT>::IIterator(const U1& u1, const U2& u2, const U3& u3) :
		AncestorType(u1, u2, u3)
	{}


	template<class ModelT, bool ConstT>
	inline void IIterator<ModelT,ConstT>::forward()
	{
		AncestorType::forward();
	}


	template<class ModelT, bool ConstT>
	inline void IIterator<ModelT,ConstT>::forward(typename IIterator<ModelT,ConstT>::difference_type n)
	{
		AncestorType::forward(n);
	}


	template<class ModelT, bool ConstT>
	inline void IIterator<ModelT,ConstT>::backward()
	{
		AncestorType::backward();
	}


	template<class ModelT, bool ConstT>
	inline void IIterator<ModelT,ConstT>::backward(typename IIterator<ModelT,ConstT>::difference_type n)
	{
		AncestorType::backward(n);
	}


	template<class ModelT, bool ConstT>
	inline void IIterator<ModelT,ConstT>::advance(typename IIterator<ModelT,ConstT>::difference_type n)
	{
		if (n > 0)
		{
			AncestorType::forward(n);
		}
		else
		{
			if (n != 0)
				AncestorType::backward(-n);
		}
	}


	template<class ModelT, bool ConstT>
	template<class M, bool C>
	inline typename IIterator<ModelT,ConstT>::difference_type
	IIterator<ModelT,ConstT>::distance(const IIterator<M,C>& rhs) const
	{
		return AncestorType::distance(static_cast<const typename IIterator<M,C>::AncestorType&>(rhs));
	}


	template<class ModelT, bool ConstT>
	inline typename IIterator<ModelT,ConstT>::reference
	IIterator<ModelT,ConstT>::operator * ()
	{
		return AncestorType::operator * ();
	}


	template<class ModelT, bool ConstT>
	inline typename IIterator<ModelT,ConstT>::const_reference
	IIterator<ModelT,ConstT>::operator * () const
	{
		return AncestorType::operator * ();
	}


	template<class ModelT, bool ConstT>
	inline typename IIterator<ModelT,ConstT>::pointer
	IIterator<ModelT,ConstT>::operator -> ()
	{
		return AncestorType::operator -> ();
	}


	template<class ModelT, bool ConstT>
	inline typename IIterator<ModelT,ConstT>::const_pointer
	IIterator<ModelT,ConstT>::operator -> () const
	{
		return AncestorType::operator -> ();
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>&  IIterator<ModelT,ConstT>::operator ++ ()
	{
		AncestorType::forward();
		return *this;
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>&  IIterator<ModelT,ConstT>::operator -- ()
	{
		AncestorType::backward();
		return *this;
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>  IIterator<ModelT,ConstT>::operator ++ (int)
	{
		IteratorType copy(*this);
		AncestorType::forward();
		return copy;
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>  IIterator<ModelT,ConstT>::operator -- (int)
	{
		IteratorType copy(*this);
		AncestorType::backward();
		return copy;
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>&  IIterator<ModelT,ConstT>::operator += (typename IIterator<ModelT,ConstT>::difference_type n)
	{
		AncestorType::forward(n);
		return *this;
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>&  IIterator<ModelT,ConstT>::operator -= (typename IIterator<ModelT,ConstT>::difference_type n)
	{
		AncestorType::backward(n);
		return *this;
	}


	template<class ModelT, bool ConstT>
	template<class M, bool C>
	inline bool  IIterator<ModelT,ConstT>::operator == (const IIterator<M,C>& rhs) const
	{
		return AncestorType::equals(static_cast<const typename IIterator<M,C>::AncestorType&>(rhs));
	}


	template<class ModelT, bool ConstT>
	template<class M, bool C>
	inline bool  IIterator<ModelT,ConstT>::operator != (const IIterator<M,C>& rhs) const
	{
		return not AncestorType::equals(static_cast<const typename IIterator<M,C>::AncestorType&>(rhs));
	}


	template<class ModelT, bool ConstT>
	template<class M, bool C>
	inline IIterator<ModelT,ConstT>& IIterator<ModelT,ConstT>::operator = (const IIterator<M,C>& rhs)
	{
		AncestorType::reset(static_cast<const typename IIterator<M,C>::AncestorType&>(rhs));
		return *this;
	}


	template<class ModelT, bool ConstT>
	inline IIterator<ModelT,ConstT>& IIterator<ModelT,ConstT>::operator = (const IIterator<ModelT,ConstT>& rhs)
	{
		AncestorType::reset(static_cast<const AncestorType&>(rhs));
		return *this;
	}




} // namespace Yuni
