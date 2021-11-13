#pragma once

#include "ijob_monitor.h"

#include "job_handle.h"
#include "job_queue.h"

namespace ProtonProbe
{
	class Worker
	{
	public:
		Worker(JobQueue& jobQueue, IJobMonitor& jobMonitor);
		void operator()();
		void stop(bool finishRemaininJobs);
	private:
		const static int MAX_NUMBER_OF_RETRIES_ON_FAILURE{ 5 };

		JobQueue& mJobQueue;
		IJobMonitor& mJobMonitor;
		bool mWork{ true };
		bool mFinishRemainingJobs{ true };
	};
}