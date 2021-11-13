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
		while (mWork || (mFinishRemainingJobs && !mJobQueue.empty()))
		{
			if (!mJobQueue.empty())
			{
				const JobHandle& handle{ mJobQueue.pop() };
				mJobMonitor.jobExecutionStarted(handle.id);
				if (handle.job.execute())
				{
					mJobMonitor.jobSucceed(handle.id);
				}
				mJobMonitor.jobExecutionFinished(handle.id);
			}
		}
	}

	void Worker::stop(bool finishRemaininJobs)
	{
		mWork = false;
		mFinishRemainingJobs = finishRemaininJobs;
	}
}