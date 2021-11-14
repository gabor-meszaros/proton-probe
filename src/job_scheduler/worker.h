#pragma once

#include <mutex>

#include "ijob.h"
#include "ijob_monitor.h"

#include "job_handle.h"
#include "job_queue.h"

namespace ProtonProbe
{
	class Worker
	{
	public:
		Worker(const Worker& other);
		Worker(JobQueue& jobQueue, IJobMonitor& jobMonitor);
		void operator()();
		void stop(bool finishRemaininJobs);
		void cancel(const IJob::IdType job);
		bool executes(const IJob::IdType job);

	private:
		const static int MAX_NUMBER_OF_RETRIES_ON_FAILURE{ 5 };

		JobQueue& mJobQueue;
		IJobMonitor& mJobMonitor;
		bool mWork{ true };
		bool mFinishRemainingJobs{ true };
		IJob::IdType mCurrentJob{ IJob::INVALID_JOB_ID };
		bool mCancelCurrentJob{ false };
		mutable std::mutex mJobExecutionContexSwitchMutex; // For mCurrentJob and mCancelCurrentJob
	};
}