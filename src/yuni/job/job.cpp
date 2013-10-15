
#include "job.h"
#include "../core/math.h"



namespace Yuni
{
namespace Job
{

	IJob::IJob() :
		pState(stateIdle),
		pProgression(),
		pCanceling(),
		pThread(nullptr)
	{}


	IJob::~IJob()
	{
		assert(this != NULL and "IJob: Destructor: Oo `this' is null !?");
		assert(pThread == NULL and "A job can not be attached to a thread when destroyed");
	}


	bool IJob::suspend(uint delay) const
	{
		// This method must only be called from a thread
		assert(pThread and "Job: The pointer to the attached thread must not be NULL");

		// We can suspend the job only if it is running
		if (pState == stateRunning)
		{
			// It is important (for thread-safety reasons) that this method
			// does not modify the state.
			// This may lead to unwanted behaviors.
			// Sleeping for a while...
			bool r = pThread->suspend(delay);
			// The state may have changed while we were sleeping
			return (pCanceling or r);
		}
		return true;
	}


	void IJob::execute(Thread::IThread* t)
	{
		// note: \p t can be null

		// Reseting data
		// We will keep the state in `waiting` until we have properly set
		// all other values
		pThread = t;
		pCanceling = 0;
		pProgression = 0;

		// Here we go !
		pState = stateRunning;
		// Execute the specific implementation of the job
		onExecute();
		// The state must be reset to idle as soon as possible while the
		// other values are still valid.
		pState = stateIdle;

		// Other values
		pThread = nullptr;
		pProgression = 100;
	}


	void IJob::name(const AnyString& newname)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		pName = newname;
	}





} // namespace Job
} // namespace Yuni
