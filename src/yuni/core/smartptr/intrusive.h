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
#include "smartptr.h"



namespace Yuni
{

	/*!
	** \brief Give to inherited classes an intrusive counting through CRTP.
	**
	** \code
	** class IControl : public IIntrusiveSmartPtr<IControl>
	** {
	** public:
	**	typedef IIntrusiveSmartPtr<IControl> Ancestor;
	**	typedef Ancestor::SmartPtrType<IControl>::Ptr  Ptr;
	**
	**  virtual ~IControl() {}
	** };
	** \endcode
	** \tparam ChildT Child class type
	** \tparam TP  Threading policy. Set by default for a single thread
	*/
	template<class ChildT, bool VirtualT = true, template<class> class TP = Yuni::Policy::ObjectLevelLockable>
	class YUNI_DECL IIntrusiveSmartPtr : public TP<IIntrusiveSmartPtr<ChildT, VirtualT, TP> >
	{
	public:
		//! Simplified type name of the instance for the current child type & threading policy.
		typedef IIntrusiveSmartPtr<ChildT, VirtualT, TP> IntrusiveSmartPtrType;
		//! Threading policy type
		typedef TP<IntrusiveSmartPtrType> ThreadingPolicy;

		/*!
		** \brief Class Helper to determine the most suitable smart pointer for a class
		**   according the current threading policy
		*/
		template<class T>
		class SmartPtrType final
		{
		public:
			//! A thread-safe type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>  PtrThreadSafe;
			//! A default type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>    PtrSingleThreaded;
			//! The most suitable smart pointer for T
			typedef typename Yuni::Static::If<ThreadingPolicy::threadSafe, PtrThreadSafe, PtrSingleThreaded>::ResultType  Ptr;

		}; // class SmartPtrType

		/*!
		** \brief Most convenient smartptr for this class
		**
		** Derived classes must use the class Helper \p SmartPtrType for retrieving
		** a more appropriate smart ptr
		*/
		typedef typename SmartPtrType<IntrusiveSmartPtrType>::Ptr  Ptr;


	public:
		//! \name Pointer management
		//@{
		//! Increment the internal reference counter (should not be called directly)
		void addRef() const;
		//! Decrement the internal reference counter and returns true if it should
		// be deleted (should not be called directly)
		bool release() const;
		//! Dummy method for checking smartptr compatibility
		bool hasIntrusiveSmartPtr() const {return true;}
		//@}


		//! \name Operators
		//@{
		/*!
		** \brief Assignment operator
		** \param rhs Assignment source
		** \return Reference to "this"
		*/
		IIntrusiveSmartPtr& operator = (const IIntrusiveSmartPtr& rhs);
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
		virtual void onRelease() const {}
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
		mutable Atomic::Int<8 * sizeof(void*), TP> pRefCount;

	}; // class IIntrusiveSmartPtr






	template<class ChildT, template<class> class TP>
	class  IIntrusiveSmartPtr<ChildT, false, TP> : public TP<IIntrusiveSmartPtr<ChildT, false, TP> >
	{
	public:
		//! Simplified type name of the instance for the current child type & threading policy.
		typedef IIntrusiveSmartPtr<ChildT, false, TP> IntrusiveRef;
		//! Typedef used for detecting misuse of smartptr. The real type does not really matter
		typedef IntrusiveRef IntrusiveSmartPtrType;
		//! Threading policy type
		typedef TP<IntrusiveRef> ThreadingPolicy;

		/*!
		** \brief Class Helper to determine the most suitable smart pointer for a class
		**   according the current threading policy
		*/
		template<class T>
		class SmartPtrType final
		{
		public:
			//! A thread-safe type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>  PtrThreadSafe;
			//! A default type
			typedef Yuni::SmartPtr<T, Yuni::Policy::Ownership::COMReferenceCounted>    PtrSingleThreaded;
			//! The most suitable smart pointer for T
			typedef typename Yuni::Static::If<ThreadingPolicy::threadSafe, PtrThreadSafe, PtrSingleThreaded>::ResultType Ptr;

		}; // class SmartPtrType

		/*!
		** \brief Most convenient smartptr for this class
		**
		** Derived classes must use the class Helper \p SmartPtrType for retrieving
		** a more appropriate smart ptr
		*/
		typedef typename SmartPtrType<IntrusiveRef>::Ptr  Ptr;


	public:
		//! \name Pointer management
		//@{
		//! Increment the internal reference counter (should not be called directly)
		void addRef() const;
		//! Decrement the internal reference counter and returns true if it should
		// be deleted (should not be called directly)
		bool release() const;
		//! Dummy method for checking smartptr compatibility
		bool hasIntrusiveSmartPtr() const {return true;}
		//@}


		//! \name Operators
		//@{
		/*!
		** \brief Assignment operator
		** \param rhs Assignment source
		** \return Reference to "this"
		*/
		IIntrusiveSmartPtr& operator = (const IIntrusiveSmartPtr& rhs);
		//@}


	protected:
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
		~IIntrusiveSmartPtr();

		/*!
		** \brief Copy constructor
		** \param rhs Copy source
		*/
		explicit IIntrusiveSmartPtr(const IIntrusiveSmartPtr& rhs);
		//@}


	private:
		//! Intrusive reference count
		mutable Atomic::Int<8 * sizeof(void*), TP> pRefCount;

	}; // class IIntrusiveSmartPtr






} // namespace Yuni

#include "intrusive.hxx"

