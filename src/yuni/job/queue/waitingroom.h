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
#pragma once
#include "../job.h"
#include "../../core/slist/slist.h"
#include "../../core/atomic/bool.h"
#include <deque>



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
		//! \name Constructor & Destructor
		//@{
		//! Destructor
		~WaitingRoom();
		//@}

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
		** \return True if a job is actually available, false otherwise
		*/
		bool pop(Yuni::Job::IJob::Ptr& out, const Yuni::Job::Priority priority);

		/*!
		** \brief Get the next job to execute
		**
		** \param[out] out Job to execute, if any. It will remain untouched if
		**   no job can be found.
		** \return True if a job is actually available, false otherwise
		*/
		bool pop(Yuni::Job::IJob::Ptr& out);

		//! Get the number of jobs waiting to be executed
		uint size() const;

		//! Remove all job waiting
		void clear();
		//@}


	private:
		//! Number of job waiting to be executed
		Atomic::Int<32>  pJobCount;
		//! List of waiting jobs by priority
		std::deque<Yuni::Job::IJob::Ptr>  pJobs[priorityCount];
		//! Mutexes, by priority to reduce congestion
		Mutex pMutexes[priorityCount];

	}; // class WaitingRoom






} // namespace QueueService
} // namespace Private
} // namespace Yuni

#include "waitingroom.hxx"
