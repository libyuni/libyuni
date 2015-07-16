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




namespace Yuni
{
namespace Event
{

	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	*/
	template<typename R, class Dummy = void>
	class IObserverItemA0
	{
	public:
		virtual ~IObserverItemA0() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent() const = 0;

	}; // class IObserverItemA0


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	*/
	template<typename R, typename A0, class Dummy = void>
	class IObserverItemA1
	{
	public:
		virtual ~IObserverItemA1() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0) const = 0;

	}; // class IObserverItemA1


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	*/
	template<typename R, typename A0, typename A1, class Dummy = void>
	class IObserverItemA2
	{
	public:
		virtual ~IObserverItemA2() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1) const = 0;

	}; // class IObserverItemA2


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, class Dummy = void>
	class IObserverItemA3
	{
	public:
		virtual ~IObserverItemA3() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2) const = 0;

	}; // class IObserverItemA3


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, class Dummy = void>
	class IObserverItemA4
	{
	public:
		virtual ~IObserverItemA4() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3) const = 0;

	}; // class IObserverItemA4


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, class Dummy = void>
	class IObserverItemA5
	{
	public:
		virtual ~IObserverItemA5() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const = 0;

	}; // class IObserverItemA5


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, class Dummy = void>
	class IObserverItemA6
	{
	public:
		virtual ~IObserverItemA6() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const = 0;

	}; // class IObserverItemA6


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, class Dummy = void>
	class IObserverItemA7
	{
	public:
		virtual ~IObserverItemA7() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const = 0;

	}; // class IObserverItemA7


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, class Dummy = void>
	class IObserverItemA8
	{
	public:
		virtual ~IObserverItemA8() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const = 0;

	}; // class IObserverItemA8


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, class Dummy = void>
	class IObserverItemA9
	{
	public:
		virtual ~IObserverItemA9() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const = 0;

	}; // class IObserverItemA9


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, class Dummy = void>
	class IObserverItemA10
	{
	public:
		virtual ~IObserverItemA10() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) const = 0;

	}; // class IObserverItemA10


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, class Dummy = void>
	class IObserverItemA11
	{
	public:
		virtual ~IObserverItemA11() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) const = 0;

	}; // class IObserverItemA11


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, class Dummy = void>
	class IObserverItemA12
	{
	public:
		virtual ~IObserverItemA12() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11) const = 0;

	}; // class IObserverItemA12


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, class Dummy = void>
	class IObserverItemA13
	{
	public:
		virtual ~IObserverItemA13() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12) const = 0;

	}; // class IObserverItemA13


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	** \tparam A13 Type of the 14th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, class Dummy = void>
	class IObserverItemA14
	{
	public:
		virtual ~IObserverItemA14() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13) const = 0;

	}; // class IObserverItemA14


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	** \tparam A13 Type of the 14th argument
	** \tparam A14 Type of the 15th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, class Dummy = void>
	class IObserverItemA15
	{
	public:
		virtual ~IObserverItemA15() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14) const = 0;

	}; // class IObserverItemA15


	/*!
	** \brief Item for a list of observer(Interface)
	**
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	** \tparam A13 Type of the 14th argument
	** \tparam A14 Type of the 15th argument
	** \tparam A15 Type of the 16th argument
	*/
	template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, class Dummy = void>
	class IObserverItemA16
	{
	public:
		virtual ~IObserverItemA16() {}
		virtual IObserver* observer() const = 0;
		virtual bool equalsTo(const IObserver* o) const = 0;
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15) const = 0;

	}; // class IObserverItemA16








	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	*/
	template<class C, class Mode, class R, class Dummy = void>
	class ObserverItemA0 : public IObserverItemA0<R, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA0(C* o, R (C::*method)());
		//! Destructor
		virtual ~ObserverItemA0() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent() const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)();

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	*/
	template<class C, class Mode, class R, typename A0, class Dummy = void>
	class ObserverItemA1 : public IObserverItemA1<R, A0, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA1(C* o, R (C::*method)(A0));
		//! Destructor
		virtual ~ObserverItemA1() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, class Dummy = void>
	class ObserverItemA2 : public IObserverItemA2<R, A0, A1, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA2(C* o, R (C::*method)(A0, A1));
		//! Destructor
		virtual ~ObserverItemA2() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, class Dummy = void>
	class ObserverItemA3 : public IObserverItemA3<R, A0, A1, A2, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA3(C* o, R (C::*method)(A0, A1, A2));
		//! Destructor
		virtual ~ObserverItemA3() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, class Dummy = void>
	class ObserverItemA4 : public IObserverItemA4<R, A0, A1, A2, A3, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA4(C* o, R (C::*method)(A0, A1, A2, A3));
		//! Destructor
		virtual ~ObserverItemA4() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, class Dummy = void>
	class ObserverItemA5 : public IObserverItemA5<R, A0, A1, A2, A3, A4, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA5(C* o, R (C::*method)(A0, A1, A2, A3, A4));
		//! Destructor
		virtual ~ObserverItemA5() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, class Dummy = void>
	class ObserverItemA6 : public IObserverItemA6<R, A0, A1, A2, A3, A4, A5, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA6(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5));
		//! Destructor
		virtual ~ObserverItemA6() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, class Dummy = void>
	class ObserverItemA7 : public IObserverItemA7<R, A0, A1, A2, A3, A4, A5, A6, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA7(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6));
		//! Destructor
		virtual ~ObserverItemA7() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, class Dummy = void>
	class ObserverItemA8 : public IObserverItemA8<R, A0, A1, A2, A3, A4, A5, A6, A7, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA8(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7));
		//! Destructor
		virtual ~ObserverItemA8() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, class Dummy = void>
	class ObserverItemA9 : public IObserverItemA9<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA9(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8));
		//! Destructor
		virtual ~ObserverItemA9() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, class Dummy = void>
	class ObserverItemA10 : public IObserverItemA10<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA10(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9));
		//! Destructor
		virtual ~ObserverItemA10() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, class Dummy = void>
	class ObserverItemA11 : public IObserverItemA11<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA11(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10));
		//! Destructor
		virtual ~ObserverItemA11() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, class Dummy = void>
	class ObserverItemA12 : public IObserverItemA12<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA12(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11));
		//! Destructor
		virtual ~ObserverItemA12() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, class Dummy = void>
	class ObserverItemA13 : public IObserverItemA13<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA13(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12));
		//! Destructor
		virtual ~ObserverItemA13() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	** \tparam A13 Type of the 14th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, class Dummy = void>
	class ObserverItemA14 : public IObserverItemA14<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA14(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13));
		//! Destructor
		virtual ~ObserverItemA14() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	** \tparam A13 Type of the 14th argument
	** \tparam A14 Type of the 15th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, class Dummy = void>
	class ObserverItemA15 : public IObserverItemA15<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA15(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14));
		//! Destructor
		virtual ~ObserverItemA15() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14);

	}; // class ObserverItem




	/*!
	** \tparam C The real type of the observer
	** \tparam Mode The mode when firing the event
	** \tparam R The type of the returned value
	** \tparam A0 Type of the 1th argument
	** \tparam A1 Type of the 2th argument
	** \tparam A2 Type of the 3th argument
	** \tparam A3 Type of the 4th argument
	** \tparam A4 Type of the 5th argument
	** \tparam A5 Type of the 6th argument
	** \tparam A6 Type of the 7th argument
	** \tparam A7 Type of the 8th argument
	** \tparam A8 Type of the 9th argument
	** \tparam A9 Type of the 10th argument
	** \tparam A10 Type of the 11th argument
	** \tparam A11 Type of the 12th argument
	** \tparam A12 Type of the 13th argument
	** \tparam A13 Type of the 14th argument
	** \tparam A14 Type of the 15th argument
	** \tparam A15 Type of the 16th argument
	*/
	template<class C, class Mode, class R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, class Dummy = void>
	class ObserverItemA16 : public IObserverItemA16<R, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, Dummy>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		**
		** \param o Pointer to the object
		** \param method Pointer to member
		*/
		ObserverItemA16(C* o, R (C::*method)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15));
		//! Destructor
		virtual ~ObserverItemA16() {}
		//@}

		/*!
		** \brief Fire the event
		*/
		virtual R fireEvent(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15) const;

		//! Get a pointer to the observer object
		virtual IObserver* observer() const;
		//! Test if the class contains a given observer
		virtual bool equalsTo(const IObserver* o) const;

	private:
		//! The real observer
		C* pObserver;
		//! Pointer to method
		R (C::*pMethod)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15);

	}; // class ObserverItem





} // namespace Event
} // namespace Yuni

#include "item.hxx"

