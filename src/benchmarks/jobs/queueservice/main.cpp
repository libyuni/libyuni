
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

