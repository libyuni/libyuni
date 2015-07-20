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
#include <yuni/yuni.h>
#include <yuni/core/event.h>
#include <yuni/core/string.h>
#include <iostream>


using namespace Yuni;


// A global event
//
// Here is an event with 2 arguments :
Event<void (const String&, const String&)> emailHasBeenReceived;




/*!
** \brief A subscriber class
**
** To be able to receive events, the class must inherit from the base class
** `Yuni::IEventObserver`
*/
class Subscriber : public IEventObserver<Subscriber>
{
public:
	//! Constructor
	Subscriber()
	{
		// This class wants to be aware when an event is fired
		emailHasBeenReceived.connect(this, &Subscriber::onMailReceived);
	}

	//! Destructor
	virtual ~Subscriber()
	{
		// It is really encouraged to disconnect all events as soon as possible.
		// The base class will do it, however the inherited class will already
		// be removed from the vtable and it might lead to a critical error
		destroyBoundEvents();
	}

	/*!
	** \brief Event callback
	**
	** This method will be called each time the event `emailHasBeenReceived`
	** is fired
	*/
	void onMailReceived(const String& subject, const String& from)
	{
		std::cout << "a mail has been received :" << std::endl
			<< "\tFrom    : " << from << std::endl
			<< "\tSubject : " << subject << std::endl;
	}
};



int main(void)
{
	// A subscriber, able to receive some events
	Subscriber sc;

	# ifdef YUNI_HAS_CPP_MOVE
	emailHasBeenReceived.connect([] (const String& subject, const String& from) {
		std::cout << "another notification from a lambda (c++11 minimum):" << std::endl
			<< "\tFrom    : " << from << std::endl
			<< "\tSubject : " << subject << std::endl;
	});
	# endif

	// A new event
	emailHasBeenReceived("My great subject !", "paul@utopia.org");


	return 0;
}

