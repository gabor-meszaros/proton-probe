#include "job_scheduler.h"

namespace ProtonProbe
{
	JobScheduler::JobScheduler(IJobMonitor& jobMonitor)
		: mJobMonitor{ jobMonitor }
		, mWorker{ mJobQueue, mJobMonitor }
		, mWorkerThread{ std::ref(mWorker) }
	{
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