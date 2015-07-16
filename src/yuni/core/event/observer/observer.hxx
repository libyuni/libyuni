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
#include "observer.h"
#include <cassert>




namespace Yuni
{
namespace Event
{

	template<class D, template<class> class TP>
	inline Observer<D,TP>::Observer() :
		pCanObserve(true)
	{}


	template<class D, template<class> class TP>
	inline Observer<D,TP>::~Observer()
	{
		// The derived parent class must call the method `destroyingObserver()`
		// from its destructor
		assert((pCanObserve == false)
			and "All ancestor of the class `Yuni::Observer` must call destroyingObserver() in their destructor");
	}


	template<class D, template<class> class TP>
	void
	Observer<D,TP>::destroyingObserver()
	{
		// Lock
		typename ThreadingPolicy::MutexLocker locker(*this);
		// Prevent against further connection attempts
		pCanObserve = false;
		// Disconnecting from all events
		if (not pEvents.empty())
		{
			IEvent::List::iterator end = pEvents.end();
			for (IEvent::List::iterator i = pEvents.begin(); i != end; ++i)
				(*i)->internalDetachObserver(this);
			pEvents.clear();
		}
	}


	template<class D, template<class> class TP>
	void
	Observer<D,TP>::disconnectAllEventEmitters()
	{
		// Disconnecting from all events
		typename ThreadingPolicy::MutexLocker locker(*this);
		if (not pEvents.empty())
		{
			IEvent::List::iterator end = pEvents.end();
			for (IEvent::List::iterator i = pEvents.begin(); i != end; ++i)
				(*i)->internalDetachObserver(this);
			pEvents.clear();
		}
	}

	template<class D, template<class> class TP>
	void
	Observer<D,TP>::disconnectEvent(const IEvent* event)
	{
		if (event)
		{
			// Lock
			typename ThreadingPolicy::MutexLocker locker(*this);
			// Disconnecting from the event
			if (not pEvents.empty() and IEvent::RemoveFromList(pEvents, event))
				event->internalDetachObserver(this);
		}
	}


	template<class D, template<class> class TP>
	void
	Observer<D,TP>::internalAttachEvent(IEvent* evt)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		if (pCanObserve and !IEvent::Exists(pEvents, evt))
			pEvents.push_back(evt);
	}


	template<class D, template<class> class TP>
	inline void
	Observer<D,TP>::internalDetachEvent(const IEvent* evt)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		IEvent::RemoveFromList(pEvents, evt);
	}





} // namespace Event
} // namespace Yuni
