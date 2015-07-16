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
#ifndef __YUNI_CORE_ITERATOR_ITERATOR_H__
# define __YUNI_CORE_ITERATOR_ITERATOR_H__

# include "../../yuni.h"
# include "../static/if.h"
# include <iterator> // iterator traits from STL


namespace Yuni
{

	/*!
	** \brief Generic Iterator
	*/
	template<class ModelT, bool ConstT>
	class IIterator : public ModelT
	{
	public:
		//! The model implementation
		typedef ModelT ModelType;
		//! The iterator type
		typedef IIterator<ModelT,ConstT> IteratorType;
		//! Category
		typedef IteratorType iterator_category;

		//! Ancestor
		typedef ModelT AncestorType;

		//! Value type
		typedef typename ModelT::value_type  value_type;
		//! Difference type
		typedef typename ModelT::difference_type difference_type;
		//! Pointer
		typedef typename Static::If<ConstT, const value_type*, value_type*>::Type pointer;
		//! Const pointer
		typedef const value_type* const_pointer;
		//! Reference
		typedef typename Static::If<ConstT, const value_type&, value_type&>::Type reference;
		//! Const reference
		typedef const value_type& const_reference;
		//! Const this if required
		typedef typename Static::If<ConstT, const AncestorType*, AncestorType*>::Type ConstSelfPtr;
		enum
		{
			//! A non-zero value for const qualifier
			isConst = ConstT,
			//! A non-zero value if the iterator can go forward
			canGoForward = ModelT::canGoForward,
			//! A non-zero value if the iterator can go backward
			canGoBackward = ModelT::canGoBackward,
		};

	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Default Constructor
		*/
		IIterator();

		/*!
		** \brief Copy constructor
		*/
		IIterator(const IteratorType& rhs);

		/*!
		** \brief Copy constructor from any iterator
		*/
		template<class M, bool C> IIterator(const IIterator<M,C>& rhs);

		/*!
		** \brief Assignation from a custom type, defined by the real implementation
		*/
		template<class U1> explicit IIterator(const U1& u1);

		/*!
		** \brief Assignation from two custom types, defined by the real implementation
		*/
		template<class U1, class U2> IIterator(const U1& u1, const U2& u2);

		/*!
		** \brief Assignation from three custom types, defined by the real implementation
		*/
		template<class U1, class U2, class U3> IIterator(const U1& u1, const U2& u2, const U3& u3);
		//@}


		//! \name Common operations
		//@{
		/*!
		** \brief Increment the iterator by the distance 1
		*/
		void forward();

		/*!
		** \brief Increment the iterator by the distance n
		*/
		void forward(difference_type n);

		/*!
		** \brief Decrement the iterator by the distance n
		*/
		void backward();

		/*!
		** \brief Decrement the iterator by the distance n
		*/
		void backward(difference_type n);

		/*!
		** \brief Increment/Decrement the iterator by the distance n
		**
		** If n > 0 it is equivalent to executing ++i n times, and if n < 0 it is
		** equivalent to executing --i n times. If n == 0, the call has no effect.
		*/
		void advance(difference_type n);

		/*!
		** \brief Get the distance between this iterator and another one
		*/
		template<class M, bool C>
		difference_type distance(const IIterator<M,C>& rhs) const;
		//@}


		//! \name Operators
		//@{
		//! Dereferencing
		reference operator * ();
		//! Dereferencing (const)
		const_reference operator * () const;

		//! Dereferencing
		pointer operator -> ();
		//! Dereferencing (const)
		const_pointer operator -> () const;

		//! Increment the operator by the distance 1
		IteratorType& operator ++ ();
		//! Decrement the iterator by the distance 1
		IteratorType& operator -- ();

		//! (Post) Increment the iterator by the distance 1
		IteratorType operator ++ (int);
		//! (Post) Decrement the iterator by the distance 1
		IteratorType operator -- (int);

		//! Increment the operator by the distance n
		IteratorType& operator += (difference_type n);
		//! Decrement the operator by the distance n
		IteratorType& operator -= (difference_type n);

		//! Equality comparison
		template<class M, bool C> bool operator == (const IIterator<M,C>& rhs) const;
		//! Non-equality comparison
		template<class M, bool C> bool operator != (const IIterator<M,C>& rhs) const;

		//! Assignment
		IteratorType& operator = (const IteratorType& rhs);
		//! Assignment
		template<class M, bool C> IteratorType& operator = (const IIterator<M,C>& rhs);
		//@}


	protected:
		// Friend
		template<class M, bool C> friend class IIterator;

	}; // class IIterator





} // namespace Yuni



# include "iterator.hxx"




template<class ModelT, bool ConstT>
inline Yuni::IIterator<ModelT,ConstT>
operator + (const Yuni::IIterator<ModelT,ConstT>& it, typename Yuni::IIterator<ModelT,ConstT>::difference_type n)
{
	return (Yuni::IIterator<ModelT,ConstT>(it) += n);
}

template<class ModelT, bool ConstT>
inline Yuni::IIterator<ModelT,ConstT>
operator + (typename Yuni::IIterator<ModelT,ConstT>::difference_type n, const Yuni::IIterator<ModelT,ConstT>& it)
{
	return (Yuni::IIterator<ModelT,ConstT>(it) += n);
}


template<class ModelT, bool ConstT>
inline Yuni::IIterator<ModelT,ConstT>
operator - (const Yuni::IIterator<ModelT,ConstT>& it, typename Yuni::IIterator<ModelT,ConstT>::difference_type n)
{
	return (Yuni::IIterator<ModelT,ConstT>(it) -= n);
}


template<class ModelT, bool ConstT>
inline Yuni::IIterator<ModelT,ConstT>
operator - (typename Yuni::IIterator<ModelT,ConstT>::difference_type n, const Yuni::IIterator<ModelT,ConstT>& it)
{
	return (Yuni::IIterator<ModelT,ConstT>(it) -= n);
}



#endif // __YUNI_CORE_ITERATOR_ITERATOR_H__
