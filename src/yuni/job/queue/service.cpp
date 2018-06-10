/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#include "service.h"
#include "../../core/system/cpu.h"
#include "../../thread/array.h"
#include "../../private/jobs/queue/thread.h"
#ifndef YUNI_OS_WINDOWS
	#include <unistd.h>
#endif
#include <memory>


namespace Yuni
{
namespace Job
{

	namespace
	{

	constexpr static uint32_t maxNumberOfThreads = 512;

	using ThreadArray = Yuni::Thread::Array<Yuni::Private::QueueService::QueueThread>;

	inline uint32_t optimalCPUCount()
	{
		auto count = System::CPU::Count();
		return (count > 2) ? (count - 1) : (count < 1 ? 1 : count);
	}


	} // anonymous namespace




	QueueService::QueueService()
	{
		auto count = optimalCPUCount();
		pMinimumThreadCount = count;
		pMaximumThreadCount = count;
	}


	QueueService::QueueService(bool autostart)
	{
		auto count = optimalCPUCount();
		pMinimumThreadCount = count;
		pMaximumThreadCount = count;
		if (autostart)
			start();
	}


	QueueService::~QueueService()
	{
		// making sure that the queueservice is stopped before being destroyed
		stop();
	}


	bool QueueService::maximumThreadCount(uint count)
	{
		if (count > maxNumberOfThreads) // hard-coded value
			return false;
		if (0 == count) // default value
			count = optimalCPUCount();

		MutexLocker locker(*this);
		// checking for the lower bound
		if (pMinimumThreadCount > count)
			pMinimumThreadCount = count;
		// reseting the upper bound
		pMaximumThreadCount = count;
		return true;
	}


	uint QueueService::maximumThreadCount() const
	{
		MutexLocker locker(*this);
		return pMaximumThreadCount;
	}


	bool QueueService::minimumThreadCount(uint count)
	{
		if (count > maxNumberOfThreads) // hard-coded value
			return false;
		if (0 == count)
			count = optimalCPUCount();

		MutexLocker locker(*this);

		// checking for the upper bound
		if (pMaximumThreadCount < count)
			pMaximumThreadCount = count;
		// reseting the lower bound
		pMinimumThreadCount = count;
		return true;
	}


	uint QueueService::minimumThreadCount() const
	{
		MutexLocker locker(*this);
		return pMinimumThreadCount;
	}


	std::pair<uint,uint> QueueService::minmaxThreadCount() const
	{
		MutexLocker locker(*this);
		return std::pair<uint, uint>(pMinimumThreadCount, pMaximumThreadCount);
	}


	bool QueueService::minmaxThreadCount(const std::pair<uint, uint>& values)
	{
		uint maxv = values.second;
		if (maxv > maxNumberOfThreads)
			return false;
		uint minv = values.first;
		if (minv > maxv)
			minv = maxv;
		if (maxv == 0)
		{
			maxv = optimalCPUCount();
			minv = maxv;
		}

		MutexLocker locker(*this);
		pMinimumThreadCount = minv;
		pMaximumThreadCount = maxv;
		return true;
	}


	bool QueueService::start()
	{
		MutexLocker locker(*this);
		if (YUNI_LIKELY(pStatus == State::stopped))
		{
			pSignalAllThreadHaveStopped.reset();
			pSignalShouldStop.reset();

			delete (ThreadArray*) pThreads;
			pThreads = new ThreadArray();

			// alias to the thread pool
			ThreadArray& array = *((ThreadArray*) pThreads);
			// recreate the thread pool
			// adding the minimum number of threads
			array.clear();
			for (uint i = 0; i != pMinimumThreadCount; ++i)
				array += new Yuni::Private::QueueService::QueueThread(*this);

			// Start all threads at once
			array.start();
			// Ok now we have started
			pStatus = State::running;
		}
		return true;
	}


	void QueueService::stop(uint timeout)
	{
		std::unique_ptr<ThreadArray> threads; // the thread pool
		// getting the thread pool
		{
			MutexLocker locker(*this);
			if (pStatus != State::running)
				return;
			threads.reset((ThreadArray*) pThreads);
			pThreads = nullptr;
			pStatus  = State::stopping;
		}

		// Destroying the thread pool
		if (YUNI_LIKELY(threads))
		{
			// stopping all threads (**before** deleting them)
			threads->stop(timeout);
			threads.reset(nullptr);
		}
		MutexLocker locker(*this);
		// marking the queue service as stopped, just in case
		// (thread workers should already marked us as 'stopped')
		pStatus = State::stopped;
		// signalling that the queueservice is stopped. This signal
		// will come after pSignalAllThreadHaveStopped in this case
		pSignalShouldStop.notify();
	}


	void QueueService::registerWorker(void* threadself)
	{
		assert(threadself != nullptr);
		MutexLocker locker(*this);
		if (pWorkerSet.count(threadself) == 0)
			pWorkerSet.insert(threadself);
	}


	void QueueService::unregisterWorker(void* threadself)
	{
		assert(threadself != nullptr);
		MutexLocker locker(*this);

		Yuni::Set<void*>::Unordered::iterator it = pWorkerSet.find(threadself);
		if (pWorkerSet.end() != it)
		{
			pWorkerSet.erase(it);
			if (pWorkerSet.empty())
			{
				if (pStatus == State::stopping)
					pStatus = State::stopped;
				pSignalAllThreadHaveStopped.notify();
			}
		}
	}


	bool QueueService::restart(uint timeout)
	{
		stop(timeout);
		return start();
	}


	void QueueService::gracefulStop()
	{
		MutexLocker locker(*this);
		if (pThreads and pStatus == State::running)
		{
			// about to stop
			pStatus = State::stopping;
			// ask to stop to all threads
			((ThreadArray*) pThreads)->gracefulStop();
			// notifying that the queueservice is stopped (or will stop soon)
			pSignalShouldStop.notify();
		}
	}


	inline bool QueueService::waitForAllThreads(uint timeout)
	{
		// waiting for all threads to stop
		do
		{
			if (0 == timeout)
			{
				pSignalAllThreadHaveStopped.wait();
			}
			else
			{
				if (not pSignalAllThreadHaveStopped.wait(timeout))
					return false;
			}

			MutexLocker locker(*this);
			// if the queue is running, we may have to reset our internal state
			if (pStatus == State::running)
			{
				if (not pWorkerSet.empty() or not pWaitingRoom.empty())
				{
					pSignalAllThreadHaveStopped.reset();
					continue;
				}
			}
			break;
		}
		while (true);
		return true;
	}


	void QueueService::wait(QServiceEvent event)
	{
		assert((event == qseIdle or event == qseStop) and "invalid event");

		// checking if not started
		{
			MutexLocker locker(*this);
			if (pStatus == State::stopped)
				return;
		}

		switch (event)
		{
			case qseStop:
			{
				// waiting for being terminated
				pSignalShouldStop.wait();

				// break : do not break - waiting for all threads being stopped
				[[fallthrough]];
			}
			case qseIdle:
			{
				waitForAllThreads(0);
				break;
			}
			default:
			{
				assert(false and "invalid value for event");
			}
		}
	}


	bool QueueService::wait(QServiceEvent event, uint timeout)
	{
		assert((event == qseIdle or event == qseStop) and "invalid event");
		assert(timeout > 0 and "invalid timeout");

		// checking if not started
		{
			MutexLocker locker(*this);
			if (pStatus == State::stopped)
				return true;
		}

		switch (event)
		{
			case qseStop:
			{
				// waiting for being terminated
				if (not pSignalShouldStop.wait(timeout))
					return false;

				waitForAllThreads(0);
				break;
			}
			case qseIdle:
			{
				if (not waitForAllThreads(timeout))
					return false;
				break;
			}
			default:
			{
				assert(false and "invalid value for event");
			}
		}
		return true;
	}


	inline void QueueService::wakeupWorkers()
	{
		MutexLocker locker(*this);
		if (pWorkerSet.size() < pMaximumThreadCount and pThreads)
			((ThreadArray*) pThreads)->wakeUp();
	}


	void QueueService::add(const IJob::Ptr& job)
	{
		assert(!(!job) and "invalid job");
		pWaitingRoom.add(job);
		wakeupWorkers();
	}


	void QueueService::add(const IJob::Ptr& job, Priority priority)
	{
		assert(!(!job) and "invalid job");
		pWaitingRoom.add(job, priority);
		wakeupWorkers();
	}


	void QueueService::clear()
	{
		pWaitingRoom.clear();
	}


	uint QueueService::threadCount() const
	{
		MutexLocker locker(*this);
		return pThreads ? ((const ThreadArray*) pThreads)->size() : 0;
	}


	void QueueService::activitySnapshot(std::vector<std::unique_ptr<ThreadInfo>>& out)
	{
		out.clear();
		MutexLocker locker(*this);
		if (!pThreads)
			return;
		((ThreadArray*) pThreads)->foreachThread([&] (auto& thread) -> bool
		{
			auto details = std::make_unique<QueueService::ThreadInfo>();
			details->thread = thread;
			if (!(!(details->thread)))
			{
				details->job = thread->currentJob();
				if (!(!(details->job))) // job currently executing
				{
					details->hasJob = true;
					details->job->fillInformation(*details);
				}
			}
			out.emplace_back(std::move(details));
			return true;
		});
	}




} // namespace Job
} // namespace Yuni
