#pragma once

#include <thread>

#include "ijob.h"
#include "ijob_monitor.h"

#include "job_handle.h"
#include "job_queue.h"
#include "worker.h"

namespace ProtonProbe
{
	class JobScheduler
	{
	public:
		JobScheduler(IJobMonitor& jobMonitor, unsigned int numberOfWorkers = 1);
		IJob::IdType add(IJob& job);
		void stop(bool finishRemaininJobs = true);
		~JobScheduler();

	private:
		IJob::IdType mNextId{ 0 };
		JobQueue mJobQueue;
		IJobMonitor& mJobMonitor;
		Worker mWorker;
		std::thread mWorkerThread;
	};
}