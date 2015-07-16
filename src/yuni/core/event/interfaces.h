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
#include <map>
#include <cassert>
#include "../../thread/policy.h"


namespace Yuni
{
namespace Private
{
namespace EventImpl
{

	// Forward declaration
	template<class BindT> class PredicateRemoveObject;
	template<class BindT> class PredicateRemoveObserverBase;
	template<class BindT> class PredicateRemoveWithoutChecks;


} // namespace EventImpl
} // namespace Private
} // namespace Yuni




namespace Yuni
{

	// Forward declarations
	template<class P> class Event;
	class IEvent;
	class IEventObserverBase;
	template<class Derived, template<class> class TP> class IEventObserver;



	class YUNI_DECL IEvent
	{
	public:
		//! IEvent list
		typedef std::map<IEvent*, uint> Map;

	public:
		IEvent() {}
		virtual ~IEvent() {}

	protected:
		/*!
		** \brief Unregister an observer
		**
		** This method will remove all delegates linked with the pointer to object.
		** This method should be called by any observer being destroyed.
		**
		** \param pointer Pointer-to-object (can be null)
		*/
		virtual void unregisterObserver(const IEventObserverBase* pointer) = 0;

		// Friend
		template<class Derived, template<class> class TP> friend class IEventObserver;

	}; // class IEvent





	class YUNI_DECL IEventObserverBase
	{
	public:
		IEventObserverBase() {}
		virtual ~IEventObserverBase() {}

	protected:
		/*!
		** \brief Increment a reference count for a given bound event
		**
		** /param evt An event
		*/
		virtual void boundEventIncrementReference(IEvent* evt) const = 0;

		/*!
		** \brief Decrementing a reference ocunt for a given bound event
		**
		** If the reference count reaches 0, it will be removed from the table.
		** /param evt An event
		*/
		virtual void boundEventDecrementReference(IEvent* evt) const = 0;

		/*!
		** \brief Remove an boundEvent from the table
		**
		** This method is called when the boundEvent is being destroyed.
		** /param evt An event
		*/
		virtual void boundEventRemoveFromTable(IEvent* evt) const = 0;

		// Friends
		template<class P> friend class Event;
		template<class BindT> friend class Yuni::Private::EventImpl::PredicateRemoveObject;
		template<class BindT> friend class Yuni::Private::EventImpl::PredicateRemoveObserverBase;

	}; // class IEventObserverBase




	/*!
	** \brief Base class for implementating an Observer class, able to connect to an event
	*/
	template<class Derived, template<class> class TP = Policy::ObjectLevelLockable>
	class YUNI_DECL IEventObserver :
		public IEventObserverBase, public TP<IEventObserver<Derived,TP> >
	{
	public:
		//! Type of the Event observer
		typedef IEventObserver<Derived,TP> IEventObserverType;
		//! The Threading policy
		typedef TP<IEventObserverType> ThreadingPolicy;

	public:
		//! \name Constructor & Destructor
		//@{
		IEventObserver();
		/*!
		** \brief Destructor
		*/
		virtual ~IEventObserver();
		//@}

	protected:
		/*!
		** \brief Destroy all existing and future bound events
		*/
		void destroyBoundEvents();

	private:
		// \see IEventObserverBase::boundEventIncrementReference()
		virtual void boundEventIncrementReference(IEvent* evt) const override;

		// \see IEventObserverBase::boundEventDecrementReference()
		virtual void boundEventDecrementReference(IEvent* evt) const override;

		// \see IEventObserverBase::boundEventRemoveFromTable()
		virtual void boundEventRemoveFromTable(IEvent* evt) const override;

	private:
		/*!
		** \brief List of attached boundEvents to the observer
		**
		** This class will accept any action from any bound event as long as
		** this pointer remains valid (not null). It is important to set this
		** variable to NULL when the class is being destroyed to avoid race
		** conditions with SMP processors.
		**
		** \internal The keyword 'mutable' is needed because we may want to bind
		**    an event to a const object (with a const method)
		*/
		mutable IEvent::Map* pBoundEventTable;

	}; // class IEventObserver





} // namespace Yuni

#include "interfaces.hxx"

