#pragma once

#include <thread>
#include <vector>

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
		void cancel(const IJob::IdType job);
		~JobScheduler();

	private:
		IJob::IdType mNextId{ 0 };
		JobQueue mJobQueue;
		IJobMonitor& mJobMonitor;
		std::vector<Worker> mWorkers;
		std::vector<std::thread> mWorkerThreads;
	};
}