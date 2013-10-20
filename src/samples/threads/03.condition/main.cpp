
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

