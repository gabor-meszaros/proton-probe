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
		JobQueue& mJobQueue;
		IJobMonitor& mJobMonitor;
		bool mWork{ true };
		bool mFinishRemainingJobs{ true };
	};
}