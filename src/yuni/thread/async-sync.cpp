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
#include "utility.h"
#include "../job/queue/service.h"
#include "../job/taskgroup.h"


namespace Yuni
{

	struct AsyncJob final : public Job::IJob
	{
		AsyncJob(const Bind<void ()>& callback) :
			pCallback(callback)
		{}


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


	Job::IJob::Ptr  async(const Bind<void ()>& callback)
	{
		return new AsyncJob(callback);
	}


	void async(Job::Taskgroup& task, const Bind<bool (Job::IJob&)>& callback)
	{
		task.add(callback);
	}


} // namespace Yuni
