#include "job_scheduler.h"

#include <stdexcept>

namespace ProtonProbe
{
	JobScheduler::JobScheduler(IJobMonitor& jobMonitor, unsigned int numberOfWorkers)
		: mJobMonitor{ jobMonitor }
		, mWorker{ mJobQueue, mJobMonitor }
		, mWorkerThread{ std::ref(mWorker) }
	{
		if (numberOfWorkers < 1)
		{
			throw std::invalid_argument("The number of workers must be larger than zero");
		}
	}

	IJob::IdType JobScheduler::add(IJob& job)
	{
		const IJob::IdType id{ mNextId++ };
		JobHandle item{ id, job };
		mJobQueue.enqueue(item);
		return id;
	}

	void JobScheduler::stop(bool finishRemaininJobs)
	{
		mWorker.stop(finishRemaininJobs);
		if (mWorkerThread.joinable())
		{
			mWorkerThread.join();
		}
	}

	JobScheduler::~JobScheduler()
	{
		mWorker.stop(false);
		if (mWorkerThread.joinable())
		{
			mWorkerThread.join();
		}
	}
}