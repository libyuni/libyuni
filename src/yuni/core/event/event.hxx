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
#include "event.h"



namespace Yuni
{

	template<class P>
	inline Event<P>::Event()
	{}


	template<class P>
	inline Event<P>::Event(const Event<P>& /*rhs*/)
	{}


	template<class P>
	inline Event<P>::~Event()
	{
		clearWL();
	}


	template<class P>
	inline uint Event<P>::size() const
	{
		return AncestorType::pBindList.size();
	}


	template<class P>
	void Event<P>::clearWL()
	{
		if (not AncestorType::pBindList.empty())
		{
			// We will inform all bound objects that we are no longer linked.
			{
				IEvent* baseThis = dynamic_cast<IEvent*>(this);

				const typename AncestorType::BindList::iterator end = AncestorType::pBindList.end();
				typename AncestorType::BindList::iterator i = AncestorType::pBindList.begin();
				for (; i != end; ++i)
				{
					if ((*i).isDescendantOfIEventObserverBase())
					{
						// Getting the object pointer, if any, then decrementing the ref counter
						const IEventObserverBase* base = ((*i).observerBaseObject());
						if (base)
							base->boundEventRemoveFromTable(baseThis);
					}
				}
			}
			// Clear our own list
			AncestorType::pBindList.clear();
		}
	}


	template<class P>
	inline void Event<P>::clear()
	{
		if (not AncestorType::pEmpty)
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			// In this case, the flag `empty` must be set first, to avoid concurrent
			// calls to `invoke()` for nothing.
			AncestorType::pEmpty = true;
			// Cleanup !
			clearWL();
		}
	}


	template<class P>
	inline void Event<P>::connect(typename BindType::FunctionType pointer)
	{
		Bind<P> b;
		b.bind(pointer);

		typename ThreadingPolicy::MutexLocker locker(*this);
		# ifdef YUNI_HAS_CPP_MOVE
		AncestorType::pBindList.push_back(std::move(b));
		# else
		AncestorType::pBindList.push_back(b);
		# endif
		AncestorType::pEmpty = false;
	}


	template<class P>
	template<typename U>
	void Event<P>::connect(typename BindType::template WithUserData<U>::FunctionType pointer,
		typename BindType::template WithUserData<U>::ParameterType userdata)
	{
		Bind<P> b;
		b.bind(pointer, userdata);

		// locking
		typename ThreadingPolicy::MutexLocker locker(*this);
		# ifdef YUNI_HAS_CPP_MOVE
		AncestorType::pBindList.push_back(std::move(b));
		# else
		AncestorType::pBindList.push_back(b);
		# endif

		AncestorType::pEmpty = false;
		// unlocking
	}


	template<class P>
	template<class C>
	void Event<P>::connect(C* o, typename Event<P>::template PointerToMember<C>::Type method)
	{
		if (o)
		{
			Bind<P> b;
			b.bind(o, method);
			assert(b.isDescendantOfIEventObserverBase() and "Invalid class C. The calling class must inherit from Event::Observer<CRTP, ThreadingPolicy>");

			// Locking
			typename ThreadingPolicy::MutexLocker locker(*this);
			// list + increment ref counter
			# ifdef YUNI_HAS_CPP_MOVE
			AncestorType::pBindList.push_back(std::move(b));
			# else
			AncestorType::pBindList.push_back(b);
			# endif

			(dynamic_cast<const IEventObserverBase*>(o))->boundEventIncrementReference(dynamic_cast<IEvent*>(this));
			AncestorType::pEmpty = false;
			// Unlocking
		}
	}


	template<class P>
	template<class C>
	void Event<P>::connect(const C* o, typename Event<P>::template PointerToMember<C>::ConstType method)
	{
		if (o)
		{
			Bind<P> b;
			b.bind(o, method);
			assert(b.isDescendantOfIEventObserverBase() and "Invalid class C");

			// locking
			typename ThreadingPolicy::MutexLocker locker(*this);
			// list + increment ref counter
			# ifdef YUNI_HAS_CPP_MOVE
			AncestorType::pBindList.push_back(std::move(b));
			# else
			AncestorType::pBindList.push_back(b);
			# endif

			(dynamic_cast<const IEventObserverBase*>(o))->boundEventIncrementReference(dynamic_cast<IEvent*>(this));
			AncestorType::pEmpty = false;
			// unlocking
		}
	}


	template<class P>
	template<class U>
	void Event<P>::remove(const U* object)
	{
		if (object)
		{
			const IEventObserverBase* base = dynamic_cast<const IEventObserverBase*>(object);
			if (base)
			{
				typename ThreadingPolicy::MutexLocker locker(*this);
				if (not AncestorType::pBindList.empty())
				{
					typedef Yuni::Private::EventImpl::template
						PredicateRemoveObserverBase<typename AncestorType::BindType> RemoveType;
					AncestorType::pBindList.remove(RemoveType(dynamic_cast<IEvent*>(this), base));
					AncestorType::pEmpty = AncestorType::pBindList.empty();
				}
				// unlocking
			}
			else
			{
				typename ThreadingPolicy::MutexLocker locker(*this);
				if (not AncestorType::pBindList.empty())
				{
					typedef Yuni::Private::EventImpl::template
						PredicateRemoveObject<typename AncestorType::BindType> RemoveType;
					AncestorType::pBindList.remove(RemoveType(object));
					AncestorType::pEmpty = AncestorType::pBindList.empty();
				}
				// unlocking
			}
		}
	}


	template<class P>
	void Event<P>::unregisterObserver(const IEventObserverBase* pointer)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		// When unregistering an observer, we have to remove it without any more checks
		if (not AncestorType::pBindList.empty())
		{
			typedef Yuni::Private::EventImpl::template
				PredicateRemoveWithoutChecks<typename AncestorType::BindType> RemoveType;
			AncestorType::pBindList.remove(RemoveType(pointer));
		}
		AncestorType::pEmpty = AncestorType::pBindList.empty();
	}


	template<class P>
	inline Event<P>& Event<P>::operator = (const NullPtr&)
	{
		clear();
		return *this;
	}


	template<class P>
	inline Event<P>& Event<P>::operator = (const Event<P>& rhs)
	{
		AncestorType::assign(rhs);
		return *this;
	}


	template<class P>
	inline bool Event<P>::operator ! () const
	{
		return (AncestorType::pEmpty);
	}


	template<class P>
	inline bool Event<P>::empty() const
	{
		return (AncestorType::pEmpty);
	}





} // namespace Yuni
