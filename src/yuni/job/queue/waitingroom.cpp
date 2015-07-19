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
#include "../../yuni.h"
#include "waitingroom.h"


namespace Yuni
{
namespace Private
{
namespace QueueService
{


	WaitingRoom::~WaitingRoom()
	{
		// locking all mutex to prevent some race conditions
		// (with clear() for example)
		for (uint i = 0; i != (uint) priorityCount; ++i)
			pMutexes[i].lock();
		for (uint i = 0; i != (uint) priorityCount; ++i)
			pMutexes[i].unlock();
	}


	void WaitingRoom::clear()
	{
		// we should lock all lists before anything
		for (uint i = 0; i != (uint) priorityCount; ++i)
			pMutexes[i].lock();

		// reset the total number of job _before_ unlocking
		pJobCount = 0; // may notify listeners that there is nothing to do

		// clear
		for (uint i = 0; i != (uint) priorityCount; ++i)
			pJobs[i].clear();

		// unlock all
		for (uint i = 0; i != (uint) priorityCount; ++i)
				pMutexes[i].unlock();
	}


	void WaitingRoom::add(const Yuni::Job::IJob::Ptr& job, Yuni::Job::Priority priority)
	{
		// Locking the priority queue
		// We should avoid ThreadingPolicy::MutexLocker since it may not be
		// the good threading policy for these mutexes
		Yuni::MutexLocker locker(pMutexes[priority]);

		// Resetting some internal variables of the job
		Yuni::Private::QueueService::JobAccessor<Yuni::Job::IJob>::AddedInTheWaitingRoom(*job);
		// Adding it into the good priority queue
		pJobs[priority].push_back(job);

		// Resetting our internal state
		++pJobCount;
	}


	bool WaitingRoom::pop(Yuni::Job::IJob::Ptr& out, const Yuni::Job::Priority priority)
	{
		// We should avoid ThreadingPolicy::MutexLocker since it may not be
		// the good threading policy for these mutexes
		Yuni::MutexLocker locker(pMutexes[priority]);

		if (not pJobs[priority].empty())
		{
			// It remains at least one job to run !
			out = pJobs[priority].front();
			// Removing it from the list of waiting jobs
			pJobs[priority].pop_front();

			--pJobCount;
			return true;
		}
		// It does not remain any job for this priority. Aborting.
		return false;
	}


	bool WaitingRoom::pop(Yuni::Job::IJob::Ptr& out)
	{
		return (pop(out, Yuni::Job::priorityHigh))
			or (pop(out, Yuni::Job::priorityDefault))
			or (pop(out, Yuni::Job::priorityLow));
	}





} // namespace QueueService
} // namespace Private
} // namespace Yuni

