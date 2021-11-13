#include "job_scheduler.h"

namespace ProtonProbe
{
	JobScheduler::JobScheduler(IJobMonitor& monitor)
		: mMonitor{ monitor }
	{
	}

	IJob::IdType JobScheduler::add(IJob& job)
	{
		const IJob::IdType id{ mNextId++ };
		mMonitor.jobExecutionStarted(id);
		job.execute();
		return id;
	}
}