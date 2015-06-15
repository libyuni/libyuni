#pragma once
#include "enum.h"




namespace Yuni
{
namespace Job
{

	// Forward declaration
	class IJob;

} // namespace Job
} // namespace Yuni



namespace Yuni
{
namespace Private
{
namespace QueueService
{

	// Forward declaration
	class WaitingRoom;
	class QueueThread;



	template<class JobT>
	class JobAccessor final
	{
	public:
		template<class ThreadT>
		static void Execute(JobT& job, ThreadT* thread)
		{
			job.execute(thread);
		}

		static void ThreadHasBeenKilled(JobT& job)
		{
			job.pThread = nullptr;
			job.pState  = Yuni::Job::stateWaiting;
		}

		static void AddedInTheWaitingRoom(JobT& job)
		{
			job.pState = Yuni::Job::stateWaiting;
			job.pProgression = 0;
		}

	}; // class JobAccessor





} // namespace QueueService
} // namespace Private
} // namespace Yuni
