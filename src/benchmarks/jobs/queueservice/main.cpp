/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#include <yuni/yuni.h>
#include <yuni/job/queue/service.h>
#include <yuni/thread/utility.h>
#include <yuni/core/logs.h>

using namespace Yuni;



static Yuni::Logs::Logger<>  logs;




int main()
{
	Job::QueueService queueservice;

	Mutex mutexCounter;
	uint howMany = 0;

	logs.info() << "starting the queueservice";
	queueservice.start();

	logs.info() << "creating jobs...";
	for (uint i = 0; i != 2000000; ++i)
	{
		async(queueservice, [&, i]()
		{
			// do some calculations just for fun
			{
				std::vector<uint> list;
				list.resize(1000);
				for (uint z = 0; z != (uint) list.size(); ++z)
					list[z] = z;

				uint64 sum = 0;
				for (uint z = 0; z != (uint) list.size(); ++z)
					sum += list[z];
			}

			// ahah ! wanna stop just for fun ?
			if (false and i == 10000)
			{
				queueservice.gracefulStop();
				logs.warning() << "stopping the queueservice because I am not happy !";
			}

			MutexLocker locker(mutexCounter);
			++howMany;
		});
	}

	logs.info() << "waiting for the queueservice...";
	queueservice.wait(qseIdle);
	logs.info() << "removing all jobs (executed: " << howMany << ", remaining: " << queueservice.waitingJobsCount() << ")";
	queueservice.clear();

	return 0;
}

