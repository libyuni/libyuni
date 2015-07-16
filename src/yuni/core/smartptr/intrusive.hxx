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
#include <cassert>


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

