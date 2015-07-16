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
#include "interfaces.h"
#include "../bind.h"
#include "../smartptr.h"
#include "traits.h"



namespace Yuni
{

	/*!
	** \brief Event
	** \ingroup Events
	*/
	template<class P = void ()>
	class YUNI_DECL Event final :
		public IEvent, // base class IEvent
		public Yuni::Private::EventImpl::WithNArguments<Bind<P>::argumentCount, Bind<P> >
	{
	public:
		//! Event
		typedef Event<P> EventType;
		//! The Bind Type
		typedef Bind<P> BindType;
		//! Type of the Ancestor
		typedef typename Yuni::Private::EventImpl::template WithNArguments<BindType::argumentCount, BindType> AncestorType;

		//! The Threading Policy
		typedef typename AncestorType::ThreadingPolicy ThreadingPolicy;
		//! The most suitable smartptr for the class
		typedef SmartPtr<Event> Ptr;
		//! Pointer-to-function
		typedef typename BindType::FunctionType FunctionType;

		/*!
		** \brief Pointer-to-member of any Class
		** \tparam C Any class
		*/
		template<class C>
		class PointerToMember final
		{
		public:
			//! Mere type for C
			typedef typename BindType::template PointerToMember<C>::Class Class;
			//! Type for a pointer-to-member
			typedef typename BindType::template PointerToMember<C>::Type Type;
			//! Type for a const pointer-to-member
			typedef typename BindType::template PointerToMember<C>::ConstType ConstType;
		};

		enum
		{
			//! The number of arguments
			argumentCount = BindType::argumentCount,
			//! A non-zero value if the prototype has a return value (!= void)
			hasReturnValue = BindType::hasReturnValue,
		};

		//! The type of the return value
		typedef typename BindType::ReturnType ReturnType;

		/*!
		** \brief Type of each argument by their index
		**
		** The typedef Type is always valid, even if the argument does not exist.
		** If this case, the type will be the empty class Yuni::None. You may
		** use `argumentCount` to know exactly the number of arguments.
		** \tparam I Index of the argument (zero-based)
		*/
		template<int I>
		struct Argument final
		{
			//! Type of the argument at position I (zero-based)
			typedef typename BindType::template Argument<I>::Type Type;
		};


	public:
		//! name Constructor & Destructor
		//@{
		//! Default Constructor
		Event();
		//! Copy constructor
		Event(const Event& rhs);
		//! Destructor
		~Event();
		//@}


		//! \name Connection
		//@{
		/*!
		** \brief Connect to a function
		**
		** \param pointer Pointer-to-function
		*/
		void connect(typename BindType::FunctionType pointer);

		/*!
		** \brief Connect to a function with a custom user data
		**
		** \param pointer Pointer-to-function
		** \param userdata The userdata to copy and store
		*/
		template<class U>
		void connect(typename BindType::template WithUserData<U>::FunctionType pointer,
			typename BindType::template WithUserData<U>::ParameterType userdata);

		/*!
		** \brief Connect to a object member
		**
		** \param o An object, with the class IEventObserver as ancestor (can be null)
		** \param method Pointer-to-member
		*/
		template<class C> void connect(C* o, typename PointerToMember<C>::Type method);
		/*!
		** \brief Connect to a const object member
		**
		** \param o An object, with the class IEventObserver as ancestor (can be null)
		** \param method const Pointer-to-member
		*/
		template<class C> void connect(const C* o, typename PointerToMember<C>::ConstType method);
		//@}


		//! \name Disconnection
		//@{
		/*!
		** \brief Disconnect all methods of an arbitrary object connected to the event
		**
		** \param object The object to find and to disconnect
		*/
		template<class U> void remove(const U* object);

		/*!
		** \brief Disconnect all
		*/
		void clear();
		//@}


		//! \name Misc
		//@{
		//! Get the number of current connections
		uint size() const;

		//! Get if the event does not have any single callback
		bool empty() const;
		//@}

		//[!] \name Invoke (documentation imported from the class WithNArguments)
		//@{
		// ReturnType invoke(<parameters>) const;
		// ReturnType operator () (<parameters>) const;
		//@}

		//! \name Operators
		//@{
		//! Assignment with a nullptr (equivalent to clear())
		Event& operator = (const NullPtr&);
		//! Copy operator
		Event& operator = (const Event& rhs);
		//! Get if the event is empty
		bool operator ! () const;
		//@}


	protected:
		/*!
		** \brief Unregister an observer
		**
		** This method will remove all delegates linked with the pointer to object.
		** This method should be called by any observer being destroyed.
		**
		** \param pointer Pointer-to-object (can be null)
		*/
		void unregisterObserver(const IEventObserverBase* pointer) override;

	private:
		//! Disconnect all callbacks (without lock)
		void clearWL();

	}; // class Event<>






} // namespace Yuni

#include "event.hxx"

