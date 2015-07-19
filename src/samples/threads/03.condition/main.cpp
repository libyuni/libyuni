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
#include <yuni/thread/thread.h>
#include <yuni/thread/signal.h>
#include <yuni/core/system/suspend.h>
#include <iostream>





class MyOwnThread : public Yuni::Thread::IThread
{
public:
	//! \name Constructor & Destructor
	//@{
	//! Default constructor
	MyOwnThread() {}
	//! Destructor
	virtual ~MyOwnThread() {stop();} // Required for Code robustness
	//@}

public:
	//! The condition to wait
	Yuni::Thread::Signal::Ptr condition;

protected:
	virtual bool onExecute() override
	{
		assert(!(!condition) && "the condition must not be nil");
		std::cout << "[thread] Started. Waiting for notification." << std::endl;

		// Atomically unlock and block until a notification arrive.
		condition->wait();
		std::cout << "[thread] notified to continue !" << std::endl;
		// Here we re-acquired the lock, since we received a notification.
		{
			// Some stuff here
			std::cout << "[thread] Doing some stuff" << std::endl;
		}

		std::cout << "[thread] Stopped." << std::endl;
		return false;
	}

}; // class MyOwnThread





int main(void)
{
	// Our condition, which is shared between the two threads
	Yuni::Thread::Signal::Ptr condition = new Yuni::Thread::Signal();

	// A thread, which will wait for our signal
	MyOwnThread thread;
	thread.condition = condition;

	// Starting the thread
	if (Yuni::Thread::errNone != thread.start())
	{
		std::cerr << "impossible to start a new thread";
		return EXIT_FAILURE;
	}

	// Performing a very long operation... (the thread still awaits our notification)
	std::cout << "[main] Performing a long operation (3 seconds)..." << std::endl;
	Yuni::Suspend(3); // seconds
	std::cout << "[main] long operation done" << std::endl;

	// Notifying our thread that it should do its work
	std::cout << "[main] notifying our thread to continue" << std::endl;
	condition->notify();

	// Waiting for the end of the thread (timeout = default: 5s)
	thread.wait();

	return 0;
}

