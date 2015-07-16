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
#include "../../yuni.h"
#include "../../thread/policy.h"
#include "../system/windows.hdr.h"
#include "traits.h"




namespace Yuni
{
/*!
** \brief Atomic types
** \ingroup Atomic
*/
namespace Atomic
{

	/*!
	** \brief An atomic scalar type
	** \ingroup Atomic
	**
	** An atomic scalar value is a value that may be updated atomically (means
	** without the use of a mutex).
	**
	** \code
	** Atomic::Int<32> i;
	** ++i;
	** \endcode
	**
	** \tparam Size Size (in bits) of the scalar type (16 or 32 or 64)
	** \tparam TP A threading policy to guarantee thread-safety or not
	*/
	template<
		int Size = 8 * sizeof(void*), /* 32 or 64Bits */        // Size in Bits of the scalar type
		template<class> class TP = Policy::ObjectLevelLockable  // The threading policy
		>
	class YUNI_DECL Int final YUNI_ATOMIC_INHERITS
	{
	public:
		enum
		{
			//! Get if the class must be thread-safe
			threadSafe = TP<Int<Size,TP> >::threadSafe,
		};
		enum
		{
			//! Get if we have to guarantee ourselves the thread-safety
			useMutex  = YUNI_ATOMIC_MUST_USE_MUTEX,
			//! Get if we have a boolean type
			isBoolean = (Size == 1),
		};

		/*!
		** \brief The Threading Policy
		**
		** The threading policy will be Policy::SingleThreaded in all cases, except
		** when the compiler or the operating system can not provide methods
		** on atomic scalar types. It is the case when the version of gcc is < 4.1
		** for example.
		*/
		typedef typename Private::AtomicImpl::ThreadingPolicy<threadSafe,Int<Size,TP> >::Type  ThreadingPolicy;
		//! The scalar type
		typedef typename Private::AtomicImpl::TypeFromSize<Size>::Type  Type;
		//! The scalar type
		typedef Type  ScalarType;

		enum
		{
			//! Size (in bits) of the scalar type
			size = Private::AtomicImpl::TypeFromSize<Size>::size
		};

		/*!
		** \brief Type of the inner variable
		**
		** Most of the time the keyword `volatile` is required (to avoid dangerous
		** optimizations by the compiler), except when there is no need for
		** thread-safety or when a mutex is used.
		*/
		typedef typename Private::AtomicImpl::Volatile<threadSafe,Type>::Type InnerType;


	public:
		//! \name Constructors
		//@{
		/*!
		** \brief Default Constructor
		*/
		Int();
		/*!
		** \brief Constructor with an initial value (int16)
		*/
		Int(sint16 v);
		/*!
		** \brief Constructor with an initial value (int32)
		*/
		Int(sint32 v);
		/*!
		** \brief Constructor with an initial value (int64)
		*/
		Int(sint64 v);

		/*!
		** \brief Copy constructor
		*/
		Int(const Int& v);
		/*!
		** \brief Copy constructor from another type and another threading policy
		*/
		template<int Size2, template<class> class TP2>
		Int(const Int<Size2,TP2>& v);
		//@}


		//! \name Convenient routines
		//@{
		//! Reset to zero
		void zero();
		//@}


		//! \name Operators
		//@{
		//! Pre increment operator
		ScalarType operator ++ ();
		//! Pre decrement operator
		ScalarType operator -- ();
		//! Post increment operator
		ScalarType operator ++ (int);
		//! Post decrement operator
		ScalarType operator -- (int);

		Int& operator = (const ScalarType& v);

		//! Increment
		Int& operator += (const ScalarType& v);
		//! Decrement
		Int& operator -= (const ScalarType& v);

		//! Cast operator
		operator ScalarType () const;

		//! not
		bool operator ! () const;
		//@}


	private:
		//! The real variable
		mutable InnerType pValue;
		// A friend !
		template<int, template<class> class> friend struct Yuni::Private::AtomicImpl::Operator;

	}; // class Int




	//! Convenient typedef for atomic int32
	typedef Atomic::Int<32>  Int32;

	//! Convenient typedef for atomic int64
	typedef Atomic::Int<64>  Int64;




} // namespace Atomic
} // namespace Yuni

# include "int.hxx"
# include "string.hxx"

