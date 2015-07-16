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
	** \brief Observer (Interface)
	** \ingroup Events
	*/
	class YUNI_DECL IObserver
	{
		YUNI_EVENT_ALLFRIEND_DECL_E;
	public:
		virtual ~IObserver() {}
	protected:
		virtual void internalAttachEvent(IEvent* evt) = 0;
		virtual void internalDetachEvent(const IEvent* evt) = 0;

	}; // class IObserver



	/*!
	** \brief Observer (Base class)
	** \ingroup Events
	**
	** Only classes derived from the class `Observer` can pretend to receive
	** signals from an event. This is due to the fact that there is a strong
	** relationship between the emitter and the receiver, to prevent the
	** corruption of pointers.
	**
	** The class uses the CRTP (Curiously recurring template pattern) Idiom to
	** broadcast the good type to the threading policy.
	**
	** The derived class *must* disconnect all event emitters as soon as possible
	** when the instance is being destroyed to avoid race exceptions (the VTable may
	** partially be deleted). The method `destroyingObserver()` prevents any further
	** connection with an event emitter and must always be called at least from the
	** destructor.
	**
	**
	** A complete example using the threading policies :
	** \code
	** #include <iostream>
	** #include <yuni/core/event.h>
	**
	**
	** template<template<class> class TP = Policy::ObjectLevelLockable>
	** class ThermalSensor : public TP<ThermalSensor>
	** {
	** public:
	** 		//! The threading policy
	** 		typedef TP<ThermalSensor>  ThreadingPolicy;
	**
	** public:
	** 		ThermalSensor() :pLastValue(0.) {}
	** 		~ThermalSensor() {}
	**
	** 		Event<void (float)> eventOnChanged;
	**
	** 		void update(const float newT)
	** 		{
	** 			{
	** 				typename ThreadingPolicy::MutexLocker locker(*this);
	** 				if (pLastValue - newT < 0.1f)
	** 					pLastValue = newT;
	** 				else
	** 					return;
	** 			}
	** 			onChanged(newT);
	** 		}
	**
	** private:
	** 		float pLastValue;
	** };
	**
	**
	** template<template<class> class TP = Policy::ObjectLevelLockable>
	** class Radiator : Event::Observer<Radiator<TP>, TP>
	** {
	** public:
	** 		//! The threading policy
	** 		typedef TP<ThermalSensor>  ThreadingPolicy;
	**
	** public:
	** 		Radiator(const String& name, const float limit)
	** 			:pName(name), pStarted(false), pLimit(limit)
	** 		{}
	**
	** 		~Radiator()
	** 		{
	** 			destroyingObserver();
	** 		}
	**
	** 		String name()
	** 		{
	** 			typename ThreadingPolicy::MutexLocker locker(*this);
	** 			return pName;
	** 		}
	**
	** 		void start()
	** 		{
	** 			typename ThreadingPolicy::MutexLocker locker(*this);
	** 			if (!pStarted)
	** 			{
	** 				pStarted = true;
	** 				std::cout << "The radiator `" << pName << "` has started." << std::endl;
	** 			}
	** 		}
	**
	** 		void stop()
	** 		{
	** 			typename ThreadingPolicy::MutexLocker locker(*this);
	** 			if (pStarted)
	** 			{
	** 				pStarted = false;
	** 				std::cout << "The radiator `" << pName << "`has stopped." << std::endl;
	** 			}
	** 		}
	**
	** 		void onTemperatureChanged(float t)
	** 		{
	** 			std::cout << "Temperature: " << t << std::endl;
	** 			checkTemperature(t);
	** 		}
	**
	** 		float limit() const {return pLimit;}
	** 		void limit(const float newL) {pLimit = newL;}
	**
	** private:
	** 		void checkTemperature(float t)
	** 		{
	** 			typename ThreadingPolicy::MutexLocker locker(*this);
	** 			if (t >= pLimit)
	** 			{
	** 				if (pStarted)
	** 					stop();
	** 			}
	** 			else
	** 			{
	** 				if (!pStarted)
	** 					start();
	** 			}
	** 		}
	**
	** private:
	** 		const String pName;
	** 		bool pStarted;
	** 		float pLimit;
	** };
	**
	**
	** int main(void)
	** {
	** 		// Our thermal sensor
	** 		ThermalSensor<> sensor;
	**
	** 		// Our observer
	** 		Radiator<> radiatorA("A", 30.0);
	** 		Radiator<> radiatorB("B", 16.0);
	** 		sensor.eventOnChanged.connect(&radiatorA, &Radiator::onTemperatureChanged);
	** 		sensor.eventOnChanged.connect(&radiatorB, &Radiator::onTemperatureChanged);
	**
	** 		sensor.update(-2.);
	** 		sensor.update(10.1);
	** 		sensor.update(15.9);
	** 		sensor.update(22.7);
	** 		sensor.update(42.);
	**
	** 		return 0;
	** }
	** \endcode
	**
	** \tparam D The Derived class (CRTP)
	** \tparam TP The threading policy
	*/
	template<class D, template<class> class TP = Policy::ObjectLevelLockable>
	class YUNI_DECL Observer : public TP<D>, public IObserver
	{
		YUNI_EVENT_ALLFRIEND_DECL_E;
	public:
		//! The threading policy
		typedef TP<D>  ThreadingPolicy;

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		Observer();
		//! Destructor
		virtual ~Observer();
		//@}

	protected:
		/*!
		** \brief Disconnect all event emitters and prevents any further connection
		*/
		void destroyingObserver();

		/*!
		** \brief Disconnect all event emitters, if any
		*/
		void disconnectAllEventEmitters();

		/*!
		** \brief Disconnect an event emitter, if connected to the observer
		**
		** \param event The event to disconnect
		*/
		void disconnectEvent(const IEvent* event);

	private:
		/*!
		** \internal Attach an event emitter without doing anything else
		*/
		virtual void internalAttachEvent(IEvent* evt);

		/*!
		** \internal Detach an event emitter without doing anything else
		*/
		virtual void internalDetachEvent(const IEvent* evt);

	private:
		//! List of events that are linked with the observer
		IEvent::List pEvents;
		/*!
		** \brief True if we can connect to the observer
		**
		** False would mean actually that the object is being destroying.
		*/
		bool pCanObserve;

	}; // class Observer





} // namespace Event
} // namespace Yuni

#include "observer.hxx"

