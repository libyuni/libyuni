#ifndef YUNI_CORE_SMARTPTR_INTRUSIVE_H__
# define YUNI_CORE_SMARTPTR_INTRUSIVE_H__

# include "../../yuni.h"
# include "smartptr.h"


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
	template<class ChildT, bool VirtualT = true, template<class> class TP = Yuni::Policy::ObjectLevelLockableNotRecursive>
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

		//! Most convenient smartptr for this class
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

		//! Most convenient smartptr for this class
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
		IIntrusiveSmartPtr& operator = (const IIntrusiveSmartPtr& rhs) const;
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

# include "intrusive.hxx"

#endif //  YUNI_CORE_SMARTPTR_INTRUSIVE_H__
