#ifndef YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__
# define YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__

# include <cassert>


namespace Yuni
{

	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP>::IIntrusiveSmartPtr() :
		pRefCount(0)
	{
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP>::~IIntrusiveSmartPtr()
	{
		assert(pRefCount == 0 and "Destroying smart ptr object with a non-zero reference count");
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP>::IIntrusiveSmartPtr(const IIntrusiveSmartPtr<ChildT,TP>& rhs) :
		pRefCount(0)
	{
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP> &
	IIntrusiveSmartPtr<ChildT,TP>::operator = (const IIntrusiveSmartPtr& rhs) const
	{
		// Does nothing
		return *this;
	}


	template<class ChildT, template<class> class TP>
	inline bool IIntrusiveSmartPtr<ChildT,TP>::unique() const
	{
		return (pRefCount == 1);
	}


	template<class ChildT, template<class> class TP>
	inline void	IIntrusiveSmartPtr<ChildT,TP>::addRef() const
	{
		++pRefCount;
	}


	template<class ChildT, template<class> class TP>
	bool IIntrusiveSmartPtr<ChildT,TP>::release() const
	{
		assert(pRefCount > 0 and "IIntrusiveSmartPtr: Invalid call to the method release");
		if (--pRefCount > 0)
			return false;
		// double check to avoid race conditions
		// TODO check if there is not another way
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pRefCount > 0)
				return false;
		}

		// we will be released soon
		onRelease();
		return true;
	}




} // namespace Yuni

#endif //  YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__
#ifndef YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__
# define YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__

# include <cassert>


namespace Yuni
{

	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP>::IIntrusiveSmartPtr() :
		pRefCount()
	{
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP>::~IIntrusiveSmartPtr()
	{
		assert(pRefCount == 0 and "Destroying smart ptr object with a non-zero reference count");
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP>::IIntrusiveSmartPtr(const IIntrusiveSmartPtr<ChildT,TP>& rhs) :
		pRefCount()
	{
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,TP> &
	IIntrusiveSmartPtr<ChildT,TP>::operator = (const IIntrusiveSmartPtr& rhs) const
	{
		// Does nothing
		return *this;
	}


	template<class ChildT, template<class> class TP>
	inline bool IIntrusiveSmartPtr<ChildT,TP>::unique() const
	{
		return (pRefCount == 1);
	}


	template<class ChildT, template<class> class TP>
	inline void	IIntrusiveSmartPtr<ChildT,TP>::addRef() const
	{
		++pRefCount;
	}


	template<class ChildT, template<class> class TP>
	bool IIntrusiveSmartPtr<ChildT,TP>::release() const
	{
		// TODO check if there is not another way
		{
			if (--pRefCount > 0)
				return false;
		}

		// we will be released soon
		static_cast<ChildT*>(const_cast<IIntrusiveSmartPtr<ChildT,TP>*>(this))->onRelease();
		return true;
	}




} // namespace Yuni

#endif //  YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__
