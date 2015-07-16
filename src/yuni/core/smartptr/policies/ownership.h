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


/*!
** \file
** \brief Ownership policies
**
** All available ownership policies :
**
** Policy::Ownership::ReferenceCounted<T>
** Policy::Ownership::ReferenceCountedMT<T>
** Policy::Ownership::COMReferenceCounted<T>
** Policy::Ownership::DestructiveCopy<T>
** Policy::Ownership::NoCopy<T>
*/

#include <cassert>
#include "../../../thread/policy.h"
#include "../../static/assert.h"
#include "../../static/method.h"
#include "../../atomic/int.h"
#include "../../static/method.h"



namespace Yuni
{
namespace Policy
{

/*!
** \brief Ownership policies
** \ingroup Policies
*/
namespace Ownership
{

	namespace // anonymous
	{
		template<class T>
		struct HasIntrusiveSmartPtr final
		{
			enum
			{
				yes =   Static::HasMethod::addRef<T>::yes
					and Static::HasMethod::release<T>::yes
					and Static::HasMethod::hasIntrusiveSmartPtr<T>::yes,
				no  = !yes,
			};
		};

	} // anonymous namespace




	/*!
	** \brief Implementation of the thread-safe reference counting ownership policy
	** \ingroup Policies
	*/
	template<class T>
	class ReferenceCountedMT
	{
	public:
		enum
		{
			//! Get if the ownership policy is destructive
			destructiveCopy = false
		};
		typedef Atomic::Int<>  AtomicType;


	public:
		//! \name Constructors
		//@{
		//! Default constructor
		ReferenceCountedMT() :
			pCount(new AtomicType(1))
		{
			// Check if T is a compatible class for this kind of ownership
			// If it does not compile, `COMReferenceCounted` is probably more suitable
			YUNI_STATIC_ASSERT(HasIntrusiveSmartPtr<T>::no, IncompatibleSmartPtrType);
		}
		//! Copy constructor
		ReferenceCountedMT(const ReferenceCountedMT& c)
		{
			pCount = c.pCount;
		}
		//! Copy constructor for any king of template parameter
		template<typename U> ReferenceCountedMT(const ReferenceCountedMT<U>& c)
		{
			pCount = reinterpret_cast<const ReferenceCountedMT<T>&>(c).pCount;
		}
		//@}

		/*!
		** \brief Initialization from a raw pointer
		*/
		void initFromRawPointer(const T&)
		{
		}

		/*!
		** \brief Clone this object
		** \param rhs The original object
		*/
		T clone(const T& rhs)
		{
			++(*pCount);
			return rhs;
		}

		/*!
		** \brief Release the reference
		*/
		bool release(const T&)
		{
			if (!(--(*pCount)))
			{
				AtomicType* ptr = pCount;
				pCount = nullptr;
				delete ptr;
				return true;
			}
			return false;
		}

		void swapPointer(ReferenceCountedMT& rhs)
		{
			std::swap(pCount, rhs.pCount);
		}

	private:
		//! The reference count
		AtomicType* pCount;

	}; // class ReferenceCountedMT



	/*!
	** \brief Implementation of the reference counting ownership policy
	** \ingroup Policies
	*/
	template<class T>
	class ReferenceCounted
	{
	public:
		enum
		{
			//! Get if the ownership policy is destructive
			destructiveCopy = false,
		};


	public:
		//! \name Constructors
		//@{
		//! Default constructor
		ReferenceCounted() :
			pCount(new uint(1))
		{
			// Check if T is a compatible class for this kind of ownership
			// If it does not compile, `COMReferenceCounted` is probably more suitable
			YUNI_STATIC_ASSERT(HasIntrusiveSmartPtr<T>::no, IncompatibleSmartPtrType);
		}

		//! Copy constructor
		ReferenceCounted(const ReferenceCounted& c) :
			pCount(c.pCount) // copying shared pointer
		{}

		//! Copy constructor for any king of template parameter
		template<typename U> ReferenceCounted(const ReferenceCounted<U>& c) :
			pCount(reinterpret_cast<const ReferenceCounted&>(c).pCount)
		{}
		//@}

		/*!
		** \brief Initialization from a raw pointer
		*/
		void initFromRawPointer(const T&) {}

		/*!
		** \brief Clone this object
		** \param rhs The original object
		*/
		T clone(const T& rhs)
		{
			++(*pCount);
			return rhs;
		}

		/*!
		** \brief Release the reference
		*/
		bool release(const T&)
		{
			if (!(--(*pCount)))
			{
				delete pCount;
				pCount = nullptr;
				return true;
			}
			return false;
		}

		void swapPointer(ReferenceCounted& rhs)
		{
			std::swap(pCount, rhs.pCount);
		}

	private:
		//! The reference count
		uint* pCount;

	}; // class ReferenceCounted





	/*!
	** \brief Implementation of the COM intrusive reference counting ownership policy
	** \ingroup Policies
	*/
	template<class T>
	class COMReferenceCounted
	{
	public:
		enum
		{
			//! Get if the ownership policy is destructive
			destructiveCopy = false
		};


	public:
		//! \name Constructors
		//@{
		//! Default constructor
		COMReferenceCounted()
		{
			// Check if T is a compatible class for this kind of ownership
			// If it does not compile, `ReferenceCountedMT` is probably more suitable
			YUNI_STATIC_ASSERT(HasIntrusiveSmartPtr<T>::yes, IncompatibleSmartPtrType);
		}
		//! Copy constructor for any king of template parameter
		template<typename U> COMReferenceCounted(const COMReferenceCounted<U>&) {}
		//@}


		/*!
		** \brief Initialization from a raw pointer
		*/
		static void initFromRawPointer(const T& rhs)
		{
			clone(rhs);
		}

		/*!
		** \brief Clone this object
		** \param rhs The original object
		*/
		static T clone(const T& rhs)
		{
			if (0 != rhs)
				rhs->addRef();
			return rhs;
		}

		/*!
		** \brief Release the reference
		*/
		static bool release(const T& rhs)
		{
			return (0 != rhs) ? rhs->release() : false;
		}

		static void swapPointer(COMReferenceCounted&) {}

	}; // class COMReferenceCounted







	/*!
	** \brief Implementation of the destructive copy ownership policy
	** \ingroup Policies
	*/
	template<class T>
	class DestructiveCopy
	{
	public:
		enum
		{
			//! Get if the ownership policy is destructive
			destructiveCopy = true
		};


	public:
		//! \name Constructors
		//@{
		//! Default constructor
		DestructiveCopy()
		{
			// Check if T is a compatible class for this kind of ownership
			// If it does not compile, `COMReferenceCounted` is probably more suitable
			YUNI_STATIC_ASSERT(HasIntrusiveSmartPtr<T>::no, IncompatibleSmartPtrType);
		}
		//! Copy constructor
		template<class U> DestructiveCopy(const DestructiveCopy<U>&) {}
		//@}

		/*!
		** \brief Initialization from a raw pointer
		*/
		static void initFromRawPointer(const T&) {}

		/*!
		** \brief Clone this object
		** \param rhs The original object
		*/
		template<class U> static T clone(U& rhs)
		{
			T result(rhs);
			rhs = U();
			return result;
		}

		static bool release(const T&) {return true;}

		static void swapPointer(DestructiveCopy&) {}

	}; // class DestructiveCopy





	/*!
	** \brief Implementation of the no-copy ownership policy
	** \ingroup Policies
	*/
	template<class T>
	class NoCopy
	{
	public:
		enum
		{
			//! Get if the ownership policy is destructive
			destructiveCopy = false
		};


	public:
		//! \name Constructors
		//@{
		//! Default constructor
		NoCopy()
		{
			// Check if T is a compatible class for this kind of ownership
			// If it does not compile, `COMReferenceCounted` is probably more suitable
			YUNI_STATIC_ASSERT(HasIntrusiveSmartPtr<T>::no, IncompatibleSmartPtrType);
		}
		//! Copy constructor
		template <class U> NoCopy(const NoCopy<U>&) {}
		//@}

		/*!
		** \brief Initialization from a raw pointer
		*/
		static void initFromRawPointer(const T&) {}

		/*!
		** \brief Clone this object
		**
		** This method can not be called at compile time
		*/
		static T clone(const T&)
		{
			YUNI_STATIC_ASSERT(sizeof(T) == 0, OwnershipPolicyNoCopy);
		}

		static bool release(const T&) {return true;}

		static void swapPointer(NoCopy&) {}

	}; // class NoCopy






} // namespace Ownership
} // namespace Policy
} // namespace Yuni

