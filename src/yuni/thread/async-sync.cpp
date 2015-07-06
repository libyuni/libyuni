
#include "utility.h"


namespace Yuni
{

	class AsyncJob final : public Job::IJob
	{
	public:
		AsyncJob(const Bind<void ()>& callback) :
			pCallback(callback)
		{}

		virtual ~AsyncJob()
		{
		}


	protected:
		virtual void onExecute() override
		{
			pCallback();
		}


	private:
		Bind<void ()> pCallback;

	}; // class AsyncJob




	Job::IJob::Ptr  async(Job::QueueService& queueservice, const Bind<void ()>& callback)
	{
		AsyncJob::Ptr job = new AsyncJob(callback);
		queueservice += job;
		return job;
	}




} // namespace Yuni

