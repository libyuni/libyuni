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
#include <yuni/job/job.h>
#include <yuni/job/queue/service.h>
#include <yuni/core/system/cpu.h>
#include <iostream>

using namespace Yuni;



//! This mutex will serve to synchronize the standard output.
static Yuni::Mutex mutex;



/*!
** \brief This class represents a task that should be executed
** in a separate thread.
**
** This task is implemented in the onExecute() method, and consists
** here of a sample: counting beer bottles.
*/
class MyJob final : public Job::IJob
{
public:
	MyJob(int identifier) :
		x(identifier)
	{
	}
	virtual ~MyJob() { }

private:
	virtual void onExecute() override
	{
		mutex.lock();
		std::cout << " ["<< x <<"] Starting..." << std::endl;
		mutex.unlock();

		// Count from 10
		int i = 5;

		while (i > 0)
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
			// SuspendMilliSeconds(1200); // seconds
			suspend(1200); // milliseconds
		}
		mutex.lock();
		std::cout << " ["<< x <<"] Finished." << std::endl;
		mutex.unlock();
	}

private:
	//! The class can hold the variables of your choice.
	int x;

}; // class MyJob






int main(void)
{
	// Here we create a new MyJob with identifier 0 - keep in
	// mind that this identifier is specific to our sample class.
	// We can use it as an abstract or specific class, depending
	// on the way we want to manage it.

	Job::QueueService qs;

	for (int job = 1; job <= 5; ++job)
		qs.add(new MyJob(job));

	std::cout << "[M] Starting bottle counting..." << std::endl;
	qs.start();

	// Then we can do all the processing we want, knowing that the
	// bottle counting task is running in background. But, beware,
	// because of the mutual access to the standard output, we
	// should lock a mutex before printing anything on it.

	//	mutex.lock();
	//	std::cout << "[M] Doing some processing here too." << std::endl;
	//	mutex.unlock();

	// Simulate a long processing
	//	Yuni::Suspend(5 /* seconds */);

	// Waiting for our tasks to complete.
	//	mutex.lock();
	//	std::cout << "[M] Main thread processing is over." << std::endl;
	//	std::cout << "[M] Waiting bottle counting tasks..." << std::endl;
	//	mutex.unlock();

	mutex.lock();
	std::cout << "[M] Wait #1" << std::endl;
	mutex.unlock();

	qs.wait();

	mutex.lock();
	std::cout << "[M] Wait #2" << std::endl;
	mutex.unlock();

	qs.wait();

	mutex.lock();
	std::cout << "[M] Stop #1" << std::endl;
	mutex.unlock();

	qs.stop();


	// We do not have to use mutexes anymore.
	std::cout << "[M] Thread stopped." << std::endl;

	return 0;
}

