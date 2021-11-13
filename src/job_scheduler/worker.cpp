#include "worker.h"

namespace ProtonProbe
{
	Worker::Worker(const Worker& other)
		: mJobQueue{ other.mJobQueue }
		, mJobMonitor{ other.mJobMonitor }
		, mWork{ other.mWork }
		, mFinishRemainingJobs{ other.mFinishRemainingJobs }
	{
		std::unique_lock<std::mutex> lock_other(other.mJobExecutionContexSwitchMutex);
		mCurrentJob = other.mCurrentJob;
		mCancelCurrentJob = other.mCancelCurrentJob;
	}

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
				{
					const std::lock_guard<std::mutex> lock(mJobExecutionContexSwitchMutex);
					mCurrentJob = handle.id;
					mCancelCurrentJob = false;
				}
				mJobMonitor.jobExecutionStarted(handle.id);
				int numberOfRetries{ 0 };
				do {
					if (handle.job.execute(mCancelCurrentJob))
					{
						mJobMonitor.jobSucceed(handle.id);
						break;
					}
					else if (numberOfRetries < MAX_NUMBER_OF_RETRIES_ON_FAILURE)
					{
						mJobMonitor.jobRetryOnFailure(handle.id);
					}
				} while (numberOfRetries++ != MAX_NUMBER_OF_RETRIES_ON_FAILURE);
				if (MAX_NUMBER_OF_RETRIES_ON_FAILURE < numberOfRetries)
				{
					mJobMonitor.jobFailed(handle.id);
				}
				mJobMonitor.jobExecutionFinished(handle.id);
				{
					const std::lock_guard<std::mutex> lock(mJobExecutionContexSwitchMutex);
					mCurrentJob = IJob::INVALID_JOB_ID;
					mCancelCurrentJob = false;
				}
			}
		}
	}

	void Worker::stop(bool finishRemaininJobs)
	{
		mWork = false;
		mFinishRemainingJobs = finishRemaininJobs;
	}

	void Worker::cancel(const IJob::IdType job)
	{
		const std::lock_guard<std::mutex> lock(mJobExecutionContexSwitchMutex);

		if (mCurrentJob == job)
		{
			mCancelCurrentJob = true;
		}
	}
}