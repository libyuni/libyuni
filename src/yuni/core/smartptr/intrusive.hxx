#ifndef YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__
# define YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__

# include <cassert>


namespace Yuni
{

	template<class ChildT, bool VirtualT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,VirtualT,TP>::IIntrusiveSmartPtr() :
		pRefCount()
	{
	}


	template<class ChildT, bool VirtualT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,VirtualT,TP>::~IIntrusiveSmartPtr()
	{
		assert(pRefCount == 0 and "Destroying smart ptr object with a non-zero reference count");
	}


	template<class ChildT, bool VirtualT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,VirtualT,TP>::IIntrusiveSmartPtr(const IIntrusiveSmartPtr<ChildT,VirtualT,TP>&) :
		pRefCount()
	{
	}


	template<class ChildT, bool VirtualT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,VirtualT,TP> &
	IIntrusiveSmartPtr<ChildT,VirtualT,TP>::operator = (const IIntrusiveSmartPtr&) const
	{
		// Does nothing
		return *this;
	}


	template<class ChildT, bool VirtualT, template<class> class TP>
	inline void	IIntrusiveSmartPtr<ChildT,VirtualT,TP>::addRef() const
	{
		++pRefCount;
	}


	template<class ChildT, bool VirtualT, template<class> class TP>
	bool IIntrusiveSmartPtr<ChildT,VirtualT,TP>::release() const
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














	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,false,TP>::IIntrusiveSmartPtr() :
		pRefCount()
	{
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,false,TP>::~IIntrusiveSmartPtr()
	{
		assert(pRefCount == 0 and "Destroying smart ptr object with a non-zero reference count");
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,false,TP>::IIntrusiveSmartPtr(const IIntrusiveSmartPtr<ChildT,false,TP>&) :
		ThreadingPolicy(),
		pRefCount()
	{
	}


	template<class ChildT, template<class> class TP>
	inline IIntrusiveSmartPtr<ChildT,false,TP> &
	IIntrusiveSmartPtr<ChildT,false,TP>::operator = (const IIntrusiveSmartPtr&) const
	{
		// Does nothing
		return *this;
	}


	template<class ChildT, template<class> class TP>
	inline void	IIntrusiveSmartPtr<ChildT,false,TP>::addRef() const
	{
		++pRefCount;
	}


	template<class ChildT, template<class> class TP>
	bool IIntrusiveSmartPtr<ChildT,false,TP>::release() const
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
		return true;
	}




} // namespace Yuni

#endif //  YUNI_CORE_SMARTPTR_INTRUSIVE_HXX__
