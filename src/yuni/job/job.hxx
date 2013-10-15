#ifndef __YUNI_JOB_AJOB_HXX__
# define __YUNI_JOB_AJOB_HXX__


namespace Yuni
{
namespace Job
{

	inline enum Job::State IJob::state() const
	{
		return (enum Job::State) ((sint32) pState);
	}


	inline bool IJob::idle() const
	{
		return (0 != (pState & stateIdle));
	}


	inline bool IJob::waiting() const
	{
		return (0 != (pState & stateWaiting));
	}


	inline bool IJob::running() const
	{
		return (0 != (pState & stateRunning));
	}


	inline void IJob::cancel()
	{
		pCanceling = 1;
	}


	inline bool IJob::canceling() const
	{
		return (0 != pCanceling);
	}


	inline String IJob::name() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pName;
	}


	inline void IJob::progression(const int p)
	{
		pProgression = ((p < 0) ? 0 : (p > 100 ? 100 : p));
	}


	inline bool IJob::finished() const
	{
		// The state must be at the very end
		return (pProgression >= 100 and pState == stateIdle);
	}


	inline bool IJob::shouldAbort() const
	{
		assert(pThread != NULL and "Job: The pointer to the attached thread must not be NULL");
		return (pCanceling or pThread->shouldAbort());
	}


	template<class T>
	void IJob::fillInformation(T& info)
	{
		// The first important value is the state
		info.state = (Yuni::Job::State) ((int) (pState));
		// Then, if the job is canceling its work
		info.canceling = pCanceling;

		info.progression = pProgression;

		ThreadingPolicy::MutexLocker locker(*this);
		info.name = pName;
	}


	inline void IJob::nameWL(const AnyString& newName)
	{
		pName = newName;
	}




} // namespace Job
} // namespace Yuni

#endif // __YUNI_JOB_AJOB_HXX__
