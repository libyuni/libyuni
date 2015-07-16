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
#include <list>
#include <cassert>
#include "interfaces.h"



namespace Yuni
{

	template<class Derived, template<class> class TP>
	inline IEventObserver<Derived,TP>::IEventObserver() :
		pBoundEventTable(new IEvent::Map())
	{}


	template<class Derived, template<class> class TP>
	inline IEventObserver<Derived,TP>::~IEventObserver()
	{
		// The derived class must remove the boumd events itself
		// to prevent race data and a corrupt vtable
		assert((NULL == pBoundEventTable)
			and "IEventObserver: The derived class must call `destroyBoundEvents()` by itself to prevent a corrupt vtable when destroying the object");
	}


	template<class Derived, template<class> class TP>
	void IEventObserver<Derived,TP>::boundEventIncrementReference(IEvent* evt) const
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		if (pBoundEventTable)
		{
			++((*pBoundEventTable)[evt]);
		}
	}


	template<class Derived, template<class> class TP>
	void IEventObserver<Derived,TP>::boundEventDecrementReference(IEvent* evt) const
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		if (pBoundEventTable and not pBoundEventTable->empty())
		{
			IEvent::Map::iterator i = pBoundEventTable->find(evt);
			if (i != pBoundEventTable->end())
			{
				if (i->second <= 1)
				{
					// We won't keep an entry with no reference
					pBoundEventTable->erase(i);
				}
				else
				{
					// Decrementing the reference count
					--(i->second);
				}
			}
			else
			{
				assert(false and "Impossible to find IEvent");
			}
		}
		else
		{
			assert(false and "The table is empty");
		}
	}


	template<class Derived, template<class> class TP>
	void IEventObserver<Derived,TP>::boundEventRemoveFromTable(IEvent* evt) const
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		if (pBoundEventTable and not pBoundEventTable->empty())
		{
			IEvent::Map::iterator i = pBoundEventTable->find(evt);
			if (i != pBoundEventTable->end())
			{
				pBoundEventTable->erase(i);
			}
			else
			{
				# ifndef NDEBUG
				assert(0 and " -- Impossible to find the IEvent ");
				# endif
			}
		}
	}


	template<class Derived, template<class> class TP>
	void IEventObserver<Derived,TP>::destroyBoundEvents()
	{
		// The mutex must stay locked while we unregister all observers
		// to avoid race conditions in SMP processors (see previous versions)
		typename ThreadingPolicy::MutexLocker locker(*this);
		if (pBoundEventTable)
		{
			if (not pBoundEventTable->empty())
			{
				// Unlinking this observer to all events
				IEvent::Map::iterator end = pBoundEventTable->end();
				for (IEvent::Map::iterator i = pBoundEventTable->begin(); i != end; ++i)
					(i->first)->unregisterObserver(this);
			}

			// We can now delete the table
			delete pBoundEventTable;
			// And to set it to NULL to avoid futur operations
			pBoundEventTable = NULL;
		}
	}




} // namespace Yuni
