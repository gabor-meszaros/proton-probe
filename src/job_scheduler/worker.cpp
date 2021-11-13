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
				int numberOfRetries{ 0 };
				do {
					if (handle.job.execute())
					{
						mJobMonitor.jobSucceed(handle.id);
						break;
					}
					else if (numberOfRetries < MAX_NUMBER_OF_RETRIES_ON_FAILURE)
					{
						mJobMonitor.jobRetryOnFailure(handle.id);
					}
				} while (numberOfRetries++ != MAX_NUMBER_OF_RETRIES_ON_FAILURE);
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