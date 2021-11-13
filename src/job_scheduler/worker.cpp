#include <iostream>

#include "worker.h"

namespace ProtonProbe
{
	Worker::Worker(JobQueue& jobQueue, IJobMonitor& jobMonitor)
		: mJobQueue{ jobQueue }
		, mJobMonitor{ jobMonitor }
	{
	}

	void Worker::operator()()
	{
		while (!mStopWorking || (mStopWorking && mFinishRemainingJobs && !mJobQueue.empty()))
		{
			if (!mJobQueue.empty())
			{
				const JobHandle& handle{ mJobQueue.pop()};
				mJobMonitor.jobExecutionStarted(handle.id);
				handle.job.execute();
			}
		}
	}

	void Worker::stop(bool finishRemaininJobs)
	{
		mStopWorking = true;
		mFinishRemainingJobs = finishRemaininJobs;
	}
}