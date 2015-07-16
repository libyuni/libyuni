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



namespace Yuni
{

	template<class T, class Alloc>
	inline Nullable<T,Alloc>::~Nullable()
	{}


	template<class T, class Alloc>
	inline Nullable<T,Alloc>::Nullable()
	{}


	template<class T, class Alloc>
	template<class U>
	inline Nullable<T,Alloc>::Nullable(const U& rhs) :
		pHolder(rhs)
	{}


	template<class T, class Alloc>
	inline Nullable<T,Alloc>::Nullable(typename Nullable<T,Alloc>::const_pointer rhs)
	{
		if (rhs)
			pHolder.assign(*rhs);
	}


	template<class T, class Alloc>
	inline Nullable<T,Alloc>::Nullable(const Nullable<T,Alloc>& rhs)
		:pHolder(rhs.pHolder)
	{}


	template<class T, class Alloc>
	inline Nullable<T,Alloc>::Nullable(Static::MoveConstructor<Nullable<T,Alloc> > rhs)
		:pHolder(rhs.pHolder)
	{}


	template<class T, class Alloc>
	template<class Alloc1>
	inline Nullable<T,Alloc>::Nullable(const Nullable<T,Alloc1>& rhs)
		:pHolder(rhs.pHolder)
	{}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::iterator Nullable<T,Alloc>::begin()
	{
		return iterator(pHolder.empty() ? NULL : this);
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_iterator Nullable<T,Alloc>::begin() const
	{
		return const_iterator(pHolder.empty() ? NULL : this);
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::iterator Nullable<T,Alloc>::end()
	{
		return iterator();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_iterator Nullable<T,Alloc>::end() const
	{
		return const_iterator();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reverse_iterator Nullable<T,Alloc>::rbegin()
	{
		return reverse_iterator(this);
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_reverse_iterator Nullable<T,Alloc>::rbegin() const
	{
		return const_reverse_iterator(this);
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reverse_iterator Nullable<T,Alloc>::rend()
	{
		return reverse_iterator();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_reverse_iterator Nullable<T,Alloc>::rend() const
	{
		return const_reverse_iterator();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::size_type Nullable<T,Alloc>::size() const
	{
		return pHolder.empty() ? 0 : 1;
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::size_type Nullable<T,Alloc>::capacity()
	{
		return 1;
	}

	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::size_type Nullable<T,Alloc>::max_size()
	{
		return 1;
	}


	template<class T, class Alloc>
	inline void Nullable<T,Alloc>::reserve(typename Nullable<T,Alloc>::size_type)
	{
		/* Do nothing */
	}


	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::empty() const
	{
		return pHolder.empty();
	}


	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::null() const
	{
		return pHolder.empty();
	}



	template<class T, class Alloc>
	template<class Alloc1>
	inline void Nullable<T,Alloc>::swap(Nullable<T,Alloc1>& rhs)
	{
		pHolder.swap(rhs.pHolder);
	}


	template<class T, class Alloc>
	inline void Nullable<T,Alloc>::clear()
	{
		pHolder.clear();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reference Nullable<T,Alloc>::operator * ()
	{
		return pHolder.reference();
	}

	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reference Nullable<T,Alloc>::operator -> ()
	{
		return pHolder.reference();
	}


	template<class T, class Alloc>
	inline void Nullable<T,Alloc>::push_back(const_reference rhs)
	{
		pHolder.assign(rhs);
	}


	template<class T, class Alloc>
	inline void Nullable<T,Alloc>::pop_back()
	{
		pHolder.clear();
	}



	template<class T, class Alloc>
	void Nullable<T,Alloc>::resize(const size_type n, const T& t)
	{
		if (!n)
			pHolder.clear();
		else
		{
			if (pHolder.empty())
				pHolder.assign(t);
		}
	}


	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::operator ! () const
	{
		return pHolder.empty();
	}


	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::operator == (const Nullable& rhs) const
	{
		return pHolder.empty()
			? rhs.empty()
			: (rhs.pHolder.data() == pHolder.data());
	}

	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::operator != (const Nullable& rhs) const
	{
		return pHolder.empty()
			? !rhs.empty()
			: (rhs.pHolder.data() != pHolder.data());
	}



	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::operator != (const NullPtr&) const
	{
		return !pHolder.empty();
	}

	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::operator == (const NullPtr&) const
	{
		return pHolder.empty();
	}



	template<class T, class Alloc>
	inline Nullable<T,Alloc>& Nullable<T,Alloc>::operator = (const Nullable<T,Alloc>& rhs)
	{
		pHolder.assign(rhs);
		return *this;
	}


	template<class T, class Alloc>
	template<class Alloc1>
	inline Nullable<T,Alloc>& Nullable<T,Alloc>::operator = (const Nullable<T,Alloc1>& rhs)
	{
		pHolder.assign(rhs);
		return *this;
	}


	template<class T, class Alloc>
	inline Nullable<T,Alloc>& Nullable<T,Alloc>::operator = (const NullPtr&)
	{
		pHolder.clear();
		return *this;
	}

	template<class T, class Alloc>
	template<class U>
	inline Nullable<T,Alloc>& Nullable<T,Alloc>::operator = (const U& rhs)
	{
		pHolder.assign(rhs);
		return *this;
	}


	template<class T, class Alloc>
	inline Nullable<T,Alloc>& Nullable<T,Alloc>::operator = (typename Nullable<T,Alloc>::const_pointer rhs)
	{
		if (rhs)
			pHolder.assign(*rhs);
		else
			pHolder.clear();
		return *this;
	}




	template<class T, class Alloc>
	inline Nullable<T,Alloc>& Nullable<T,Alloc>::operator = (void* const rhs)
	{
		if (!rhs)
			pHolder.clear();
		else
			throw "Can not assign a nullable with an arbitrary pointer (void*)";
		return *this;
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::Type Nullable<T,Alloc>::value() const
	{
		return pHolder.empty() ? Type() : pHolder.data();
	}

	template<class T, class Alloc>
	template<class U>
	inline typename Nullable<T,Alloc>::Type Nullable<T,Alloc>::value(const U& nullValue) const
	{
		return pHolder.empty() ? nullValue : pHolder.data();
	}




	template<class T, class Alloc>
	template<class S, class U>
	inline void Nullable<T,Alloc>::print(S& out, const U& nullValue) const
	{
		if (pHolder.empty())
			out << nullValue;
		else
			out << pHolder.data();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reference Nullable<T,Alloc>::operator[] (size_type n)
	{
		return pHolder.reference();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_reference Nullable<T,Alloc>::operator[] (size_type n) const
	{
		return pHolder.reference();
	}


	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reference Nullable<T,Alloc>::front()
	{
		return pHolder.reference();
	}

	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::reference Nullable<T,Alloc>::back()
	{
		return pHolder.reference();
	}

	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_reference Nullable<T,Alloc>::front() const
	{
		return pHolder.reference();
	}

	template<class T, class Alloc>
	inline typename Nullable<T,Alloc>::const_reference Nullable<T,Alloc>::back() const
	{
		return pHolder.reference();
	}


	template<class T, class Alloc>
	inline bool Nullable<T,Alloc>::operator < (const Nullable& rhs) const
	{
		return false;
	}





} // namespace Yuni

