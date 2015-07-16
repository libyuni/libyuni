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
#include "../../../thread/thread.h"
#include "../../../job/queue/service.h"
#include "../../../thread/signal.h"



namespace Yuni
{
namespace Private
{
namespace QueueService
{


	/*!
	** \brief A single thread for a queue service
	*/
	class YUNI_DECL QueueThread final : public Yuni::Thread::IThread
	{
	public:
		//! The most suitable smart pointer for the class
		typedef Yuni::Thread::IThread::Ptr::Promote<QueueThread>::Ptr Ptr;

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		*/
		explicit QueueThread(Yuni::Job::QueueService& queueservice);
		//! Destructor
		virtual ~QueueThread();
		//@}

		/*!
		** \brief Get the Job currently running
		*/
		Yuni::Job::IJob::Ptr currentJob() const;


	protected:
		//! Implementation of the `onExecute` method to run the jobs from the waiting room
		virtual bool onExecute() override;
		//! Implementation of the `onKill` method when the thread is killed without mercy
		virtual void onKill() override;
		//! Implementation of the `onStop` method when the thread is killed without mercy
		virtual void onStop() override;
		//! Implementation of the `onPause` method when the thread is killed without mercy
		virtual void onPause() override;

	private:
		//! Notify the queueservice that we have stopped to work
		void notifyEndOfWork();

	private:
		//! The scheduler
		Yuni::Job::QueueService& pQueueService;
		//! The current job
		Yuni::Job::IJob::Ptr pJob;

	}; // class QueueThread






} // namespace QueueService
} // namespace Private
} // namespace Yuni

#include "thread.hxx"
