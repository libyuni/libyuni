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
#include "taskgroup.h"
#include "queue/service.h"




namespace Yuni
{
namespace Job
{

	class YUNI_DECL Taskgroup::ITaskgroupJob : public Job::IJob
	{
	public:
		//! The threading policy
		typedef Job::IJob::ThreadingPolicy ThreadingPolicy;


	public:
		ITaskgroupJob(QueueService& queueservice);
		virtual ~ITaskgroupJob() {}


	public:
		//! The attached task (if running)
		Taskgroup* taskgroup;
		//! The attached queueservice
		QueueService& queueservice;


	protected:
		void onTerminate(bool success);

	}; // class TaskgroupJob




	class YUNI_DECL TaskgroupJobCallback final : public Taskgroup::ITaskgroupJob
	{
	public:
		//! The threading policy
		typedef Job::IJob::ThreadingPolicy ThreadingPolicy;


	public:
		TaskgroupJobCallback(QueueService& queueservice, const Bind<bool (IJob&)>& callback);
		virtual ~TaskgroupJobCallback() {}


	public:
		//! The callback to call
		Bind<bool (IJob&)> callback;

	protected:
		virtual void onExecute() override;

	}; // class TaskgroupJob





	inline Taskgroup::ITaskgroupJob::ITaskgroupJob(QueueService& queueservice)
		: taskgroup(nullptr)
		, queueservice(queueservice)
	{}


	inline void Taskgroup::ITaskgroupJob::onTerminate(bool success)
	{
		// notify that the job has finished
		ThreadingPolicy::MutexLocker locker(*this);
		if (taskgroup)
		{
			taskgroup->onJobTerminated(*this, success);
			taskgroup = nullptr;
		}
	}


	inline TaskgroupJobCallback::TaskgroupJobCallback(QueueService& queueservice, const Bind<bool (IJob&)>& callback)
		: Taskgroup::ITaskgroupJob(queueservice)
		, callback(callback)
	{}


	void TaskgroupJobCallback::onExecute()
	{
		// execute the real work
		bool success = false;
		// if the user likes c++exceptions
		try { success = callback(*this); } catch (...) {}
		// finish !
		onTerminate(success);
	}













	Taskgroup::Taskgroup(QueueService& queueservice, bool cancelOnError, bool autostart)
		: pTaskHasStarted(false)
		, pTaskStatus(stSucceeded) // nothing to do by default, so success
		, pJobsDoneCount(0)
		, pCancelOnError(cancelOnError)
		, pAutostart(autostart)
		, pDefaultQueueservice(queueservice)
	{}


	Taskgroup::~Taskgroup()
	{
		cancel();
	}


	inline void Taskgroup::deleteAllJobsWL()
	{
		for (uint i = 0; i != (uint) pJobs.size(); ++i)
		{
			ITaskgroupJob* job = pJobs[i];
			job->taskgroup = nullptr; // should be useless and already done
			if (job->release())
				delete job;
		}

		JobList clearAndMinimize; // force complete cleanup
		clearAndMinimize.swap(pJobs);
		pJobsDoneCount = 0;
	}


	inline void Taskgroup::onTaskStoppedWL()
	{
		// checking 'pTaskHasStarted' just in case it would have already been stopped
		if (pTaskHasStarted)
		{
			// trigger events
			on.terminated(pTaskStatus);
			// mark as stopped *after* calling the event for being consistent with onJobTerminated
			pTaskHasStarted = false;

			// notify waiters
			pSignalTaskStopped.notify();
		}
	}


	inline void Taskgroup::stopAllJobsWL()
	{
		// remove all waiting jobs
		if (not pJobs.empty())
		{
			bool atLeastOneJobWasAlive = false;

			for (uint i = 0; i != (uint) pJobs.size(); ++i)
			{
				ITaskgroupJob* job = pJobs[i];

				ITaskgroupJob::ThreadingPolicy::MutexLocker joblocker(*(job));
				if (job->taskgroup)
				{
					job->taskgroup = nullptr;
					job->cancel();
					atLeastOneJobWasAlive = true;
				}
			}

			if (atLeastOneJobWasAlive)
				onTaskStoppedWL();

			// resource cleanup
			deleteAllJobsWL();
		}
	}


	inline void Taskgroup::onJobTerminated(Taskgroup::ITaskgroupJob&, bool success)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (not pTaskHasStarted) // should never happen
			return;

		// we may need to stop at the first encoutered error
		bool forceCancel = ((not success and pTaskStatus == stSucceeded) and pCancelOnError);
		// update the status of the task
		if (not success and (pTaskStatus == stSucceeded)) // no update if == stCanceled for example
			pTaskStatus = stFailed;

		if (++pJobsDoneCount == (uint) pJobs.size() or forceCancel)
		{
			onTaskStoppedWL();
			deleteAllJobsWL();
		}
	}


	inline void Taskgroup::startWL()
	{
		pTaskHasStarted = true;
		pTaskStatus = stSucceeded; // success by default
		on.started();

		if (not pJobs.empty())
		{
			pSignalTaskStopped.reset();
			pJobsDoneCount = 0;

			for (uint i = 0; i != (uint) pJobs.size(); ++i)
			{
				ITaskgroupJob* job = pJobs[i];
				job->taskgroup = this;
				job->queueservice.add(job);
			}
		}
		else
		{
			pTaskHasStarted = false;
			on.terminated(stSucceeded);
		}
	}


	void Taskgroup::start()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (not pTaskHasStarted)
			startWL();
	}


	void Taskgroup::cancel()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		pTaskStatus = stCanceled;
		if (pTaskHasStarted)
			stopAllJobsWL();
	}


	void Taskgroup::markAsSucceeded()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		pTaskStatus = stSucceeded;
		if (pTaskHasStarted)
			stopAllJobsWL();
	}


	void Taskgroup::markAsFailed()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		pTaskStatus = stFailed;
		if (pTaskHasStarted)
			stopAllJobsWL();
	}



	void Taskgroup::add(QueueService& queueservice, const Bind<bool (IJob&)>& callback)
	{
		ITaskgroupJob* job = new TaskgroupJobCallback(queueservice, callback);
		job->addRef();

		// register the new job
		{
			ThreadingPolicy::MutexLocker locker(*this);
			// resize the job list
			pJobs.push_back(job);

			// get if the job should be automatically added to the queue
			if (not pTaskHasStarted)
			{
				// if the task is not running, starting it as if the user did it itself
				if (pAutostart)
					startWL();
				return;
			}

			job->taskgroup = this;
		}

		queueservice.add(job);
	}


	Taskgroup::Status Taskgroup::wait()
	{
		// checking if not already stopped
		{
			ThreadingPolicy::MutexLocker locker(*this);
			if (not pTaskHasStarted)
				return pTaskStatus;
		}

		// wait
		pSignalTaskStopped.wait();

		// getting the return value
		ThreadingPolicy::MutexLocker locker(*this);
		assert(pTaskHasStarted == false and "the task should have already been stopped");
		return pTaskStatus;
	}


	Taskgroup::Status Taskgroup::wait(uint timeout)
	{
		// checking if not already stopped
		{
			ThreadingPolicy::MutexLocker locker(*this);
			if (not pTaskHasStarted)
				return pTaskStatus;
		}

		if (pSignalTaskStopped.wait(timeout)) // received notification
		{
			// getting the return value
			ThreadingPolicy::MutexLocker locker(*this);
			assert(pTaskHasStarted == false and "the task should have already been stopped");
			return pTaskStatus;
		}

		// the timeout has been reached
		return stRunning;
	}


	Taskgroup::Status Taskgroup::status(uint* jobCount, uint* doneCount) const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (jobCount != nullptr)
			*jobCount = (uint) pJobs.size();
		if (doneCount != nullptr)
			*doneCount = pJobsDoneCount;

		return (not pTaskHasStarted) ? pTaskStatus : stRunning;
	}





} // namespace Job
} // namespace Yuni
