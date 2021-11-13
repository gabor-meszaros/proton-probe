#pragma once

#include "ijob.h"
#include "ijob_monitor.h"

namespace ProtonProbe
{
	class JobScheduler
	{
	public:
		const static IJob::IdType INVALID_JOB_ID{ -1 };

		JobScheduler(IJobMonitor& monitor);
		IJob::IdType add(IJob& job);

	private:
		IJob::IdType mNextId{ 0 };
		IJobMonitor& mMonitor;
	};
}