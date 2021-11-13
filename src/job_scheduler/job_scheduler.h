#pragma once

#include <queue>

#include "ijob.h"
#include "ijob_monitor.h"

#include "job_handle.h"

namespace ProtonProbe
{
	class JobScheduler
	{
	public:
		JobScheduler(IJobMonitor& monitor);
		IJob::IdType add(IJob& job);

	private:
		IJob::IdType mNextId{ 0 };
		std::queue<JobHandle> mJobQueue;
		IJobMonitor& mMonitor;
	};
}