#ifndef __YUNI_JOB_WAITING_ROOM_H__
# define __YUNI_JOB_WAITING_ROOM_H__

# include "../job.h"
# include "../../core/slist/slist.h"
# include "../../core/atomic/bool.h"



namespace Yuni
{
namespace Private
{
namespace QueueService
{

	/*!
	** \brief Container for all jobs waiting to be executed
	*/
	class YUNI_DECL WaitingRoom final
	{
	public:
		enum
		{
			//! Alias for The default priority
			priorityDefault = Yuni::Job::priorityDefault,
			//! Alias for The number of priorities
			priorityCount = Yuni::Job::priorityCount,
		};

	public:
		//! \name Container manipulation
		//@{
		/*!
		** \brief Get if the waiting room is empty
		*/
		bool empty() const;

		/*!
		** \brief Add a job into the waiting room, with a default priority
		**
		** The job will see its state changed to `stateWaiting`.
		** \param job The job to add
		*/
		void add(const Yuni::Job::IJob::Ptr& job);

		/*!
		** \brief Add a job into the waiting room
		**
		** The job will see its state changed to `stateWaiting`.
		** \param job The job to add
		** \param priority Its priority
		*/
		void add(const Yuni::Job::IJob::Ptr& job, Yuni::Job::Priority priority);

		/*!
		** \brief Get the next job to execute for a given priority
		**
		** \param[out] out Job to execute, if any. It will remain untouched if
		**   no job can be found.
		** \param priority The priority queue where to look for
		** \return True if a job is actually available
		*/
		bool pop(Yuni::Job::IJob::Ptr& out, const Yuni::Job::Priority priority);

		//! Get the number of jobs waiting to be executed
		uint size() const;
		//@}


	public:
		//! Flags to indicate if there are some remaining jobs by priority
		Atomic::Bool hasJob[priorityCount];

	private:
		//! Number of job waiting to be executed
		Atomic::Int<32>  pJobCount;
		//! List of waiting jobs by priority
		LinkedList<Yuni::Job::IJob::Ptr>  pJobs[priorityCount];
		//! Mutexes, by priority to reduce congestion
		Mutex pMutexes[priorityCount];

	}; // class WaitingRoom






} // namespace QueueService
} // namespace Private
} // namespace Yuni

# include "waitingroom.hxx"

#endif // __YUNI_JOB_WAITING_ROOM_H__
