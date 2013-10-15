#ifndef YUNI_CORE_SMARTPTR_INTRUSIVE_H__
# define YUNI_CORE_SMARTPTR_INTRUSIVE_H__

# include "../../yuni.h"
# include "smartptr.h"


namespace Yuni
{

	/*!
	** \brief Give to inherited classes an intrusive counting through CRTP.
	**
	** \tparam ChildT Child class type
	** \tparam TP  Threading policy. Set by default for a single thread
	*/
	template<class ChildT, template<class> class TP = Yuni::Policy::SingleThreaded>
	class  IIntrusiveSmartPtr : public TP<IIntrusiveSmartPtr<ChildT,TP> >
	{
	public:
		//! Simplified type name of the instance for the current child type & threading policy.
		typedef IIntrusiveSmartPtr<ChildT,TP> IntrusiveRef;
		//! Threading policy type
		typedef TP<IntrusiveRef> ThreadingPolicy;

		/*!
		** \brief Class Helper to determine the most suitable smart pointer for a class
		**   according the current threading policy
		*/
		template<class T>
		class SmartPtr
		{
		public:
			//! A thread-safe type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>  PtrThreadSafe;
			//! A default type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>    PtrSingleThreaded;
			//! The most suitable smart pointer for T
			typedef typename Yuni::Static::If<ThreadingPolicy::threadSafe, PtrThreadSafe, PtrSingleThreaded>::ResultType Ptr;

		}; // class SmartPtr


	public:
		//! \name Pointer management
		//@{
		//! Get if the object is an unique reference (COW idiom)
		bool unique() const;

		//! Increment the internal reference counter (should not be called directly)
		void addRef() const;
		//! Decrement the internal reference counter and returns true if it should
		// be deleted (should not be called directly)
		bool release() const;
		//@}


		//! \name Operators
		//@{
		/*!
		** \brief Assignment operator
		** \param rhs Assignment source
		** \return Reference to "this"
		*/
		IIntrusiveSmartPtr& operator = (const IIntrusiveSmartPtr& rhs) const;
		//@}


	protected:
		//! \name Event
		//@{
		/*!
		** \brief Method called before the smart pointer destroy ourselves
		**
		** This method can be useful to avoid issues with a corrupted vtable
		** (ex: in UI, detaching the object from its parent before calling the destructor)
		*/
		virtual void onRelease() {}
		//@}

		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		IIntrusiveSmartPtr();

		/*!
		** \brief Destructor
		**
		** \internal The keyword 'virtual' is mandatory to have a proper call to the
		**   destructor
		*/
		virtual ~IIntrusiveSmartPtr();

		/*!
		** \brief Copy constructor
		** \param rhs Copy source
		*/
		explicit IIntrusiveSmartPtr(const IIntrusiveSmartPtr& rhs);
		//@}


	private:
		//! Intrusive reference count
		mutable Atomic::Int<> pRefCount;
		typedef typename ThreadingPolicy::template Volatile<int>::Type ReferenceCounterType;
		//! Intrusive reference count
	//	mutable ReferenceCounterType pRefCount;

	}; // class IIntrusiveSmartPtr






} // namespace Yuni

# include "intrusive.hxx"

#endif //  YUNI_CORE_SMARTPTR_INTRUSIVE_H__
#ifndef YUNI_CORE_SMARTPTR_INTRUSIVE_H__
# define YUNI_CORE_SMARTPTR_INTRUSIVE_H__

# include "../yuni.h"
# include "../core/smartptr.h"


namespace Yuni
{

	/*!
	** \brief Give to inherited classes an intrusive counting through CRTP.
	**
	** \tparam ChildT Child class type
	** \tparam TP  Threading policy. Set by default for a single thread
	*/
	template<template<class> class TP = Yuni::Policy::SingleThreaded>
	class  IIntrusiveSmartPtr : public TP<IIntrusiveSmartPtr<ChildT,TP> >
	{
	public:
		//! Simplified type name of the instance for the current child type & threading policy.
		typedef IIntrusiveSmartPtr<ChildT,TP> IntrusiveRef;
		//! Threading policy type
		typedef TP<IntrusiveRef> ThreadingPolicy;

		/*!
		** \brief Class Helper to determine the most suitable smart pointer for a class
		**   according the current threading policy
		*/
		template<class T>
		class SmartPtr
		{
		public:
			//! A thread-safe type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>  PtrThreadSafe;
			//! A default type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>    PtrSingleThreaded;
			//! The most suitable smart pointer for T
			typedef typename Yuni::Static::If<ThreadingPolicy::threadSafe, PtrThreadSafe, PtrSingleThreaded>::ResultType Ptr;

		}; // class SmartPtr


	public:
		//! \name Pointer management
		//@{
		//! Get if the object is an unique reference (COW idiom)
		bool unique() const;

		//! Increment the internal reference counter (should not be called directly)
		void addRef() const;
		//! Decrement the internal reference counter and returns true if it should
		// be deleted (should not be called directly)
		bool release() const;
		//@}


		//! \name Operators
		//@{
		/*!
		** \brief Assignment operator
		** \param rhs Assignment source
		** \return Reference to "this"
		*/
		IIntrusiveSmartPtr& operator = (const IIntrusiveSmartPtr& rhs) const;
		//@}


	protected:
		//! \name Event
		//@{
		/*!
		** \brief Method called before the smart pointer destroy ourselves
		**
		** This method can be useful to avoid issues with a corrupted vtable
		** (ex: in UI, detaching the object from its parent before calling the destructor)
		*/
		virtual void onRelease() {}
		//@}

		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		IIntrusiveSmartPtr();

		/*!
		** \brief Destructor
		**
		** \internal The keyword 'virtual' is mandatory to have a proper call to the
		**   destructor
		*/
		virtual ~IIntrusiveSmartPtr();

		/*!
		** \brief Copy constructor
		** \param rhs Copy source
		*/
		explicit IIntrusiveSmartPtr(const IIntrusiveSmartPtr& rhs);
		//@}


	private:
		//! Intrusive reference count
		mutable Atomic::Int<> pRefCount;
		typedef typename ThreadingPolicy::template Volatile<int>::Type ReferenceCounterType;
		//! Intrusive reference count
	//	mutable ReferenceCounterType pRefCount;

	}; // class IIntrusiveSmartPtr






} // namespace Yuni

# include "intrusive.hxx"

#endif //  YUNI_CORE_SMARTPTR_INTRUSIVE_H__
