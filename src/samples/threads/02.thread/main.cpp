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
#include <iostream>
#include <yuni/core/system/suspend.h>
#include <yuni/core/system/cpu.h>



//! This mutex will serve to synchronize the standard output.
static Yuni::Mutex mutex;




/*!
** \brief This class represents a task that should be executed
** in a separate thread.
**
** This task is implemented in the onExecute() method, and consists
** here of a sample: counting beer bottles.
*/
class BottleTask : public Yuni::Thread::IThread
{
public:
	/*!
	** \brief Our sample constructor.
	** \param[in] identifier A thread identifier, used only for display
	**            purposes
	*/
	BottleTask(const int identifier) :Yuni::Thread::IThread(), x(identifier) {}

	virtual ~BottleTask() {stop();}

protected:
	//! The beer-bottle counting implementation itself
	virtual bool onExecute()
	{
		// Count from 99
		int i = 99;

		while (true)
		{
			mutex.lock();
			std::cout << " ["<< x <<"] " << i-- << " bottles of beer on a wall. Calculating a very complicated thing ..." << std::endl;
			mutex.unlock();

			// Simulate some work. The work of the thread is here simulated by
			// a call to sleep(). This is intended to simulate a period of
			// work during which the task cannot be interrupted by stop() methods
			// - for example a long calculation, for the purpose of this example.
			//
			// The bottom line is: DO NOT use sleep() to wait in threads.
			//Yuni::Suspend(1 /* second */);
			suspend(1000); // 1 second

			// After our work, if the task is repetitive, we will want to check
			// if we have to stop doing it and will eventually pause for some
			// seconds (while listening to any stop signal).
			// This is the purpose of the suspend function. We give 100ms here as
			// the time to wait before returning if no stop() signal is received.
			//
			// suspend() will either return false - the specified time elapsed
			// without any events - or return true - a stop signal was received.
			//
			// In the latter case, we may have a timeout before being killed,
			// so if possible defer any time-consuming task to the onStopped()
			// methods.
			if (suspend(100))
				return false;
		}
		return false;
	}

	/*!
	** \brief Things to do when the thread is stopped
	**
	** In this, we just signal to the user that the thread
	** execution has stopped, but we can also free thread-specific
	** resources and do general cleanup.
	*/
	virtual void onStopped()
	{
		std::cout << " [b] I have been interrupted." << std::endl;
	}

private:
	//! The class can hold the variables of your choice.
	int x;

}; // class BottleTask





int main(void)
{
	// For thread-planning purposes, Yuni provides an indication of
	// how many CPUs the system has, so you can plan how you will
	// manage your resources.
	std::cout << "[M] Your system has " << Yuni::System::CPU::Count()
		<< " logical processor(s)." << std::endl;

	// Here we create a new BottleTask with identifier 0 - keep in
	// mind that this identifier is specific to our sample class.
	// We can use it as an abstract or specific class, depending
	// on the way we want to manage it.
	Yuni::Thread::IThread* t = new BottleTask(0);
	std::cout << "[M] Starting bottle counting..." << std::endl;

	// Start counting bottles.
	t->start();

	// Then we can do all the processing we want, knowing that the
	// bottle counting task is running in background. But, beware,
	// because of the mutual access to the standard output, we
	// should lock a mutex before printing anything on it.
	mutex.lock();
	std::cout << "[M] Doing some processing here too." << std::endl;
	mutex.unlock();

	// Simulate a long processing
	Yuni::Suspend(5); // 5 seconds

	mutex.lock();
	std::cout << "[M] Stopping bottle counting..." << std::endl;
	mutex.unlock();

	// We then stop the bottle-counting thread. It will exit as
	// soon as possible (on its next call to suspend() for example),
	// but will be killed anyway after a timeout, in this case 4 seconds.
	t->stop(4);

	// We do not have to use mutexes anymore.
	std::cout << "[M] Thread stopped." << std::endl;

	// Delete the thread object.
	delete t;

	return 0;
}

