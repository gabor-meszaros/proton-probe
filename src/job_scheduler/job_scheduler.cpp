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
		mJobQueue.emplace(id, job);
		const JobHandle& handle{ mJobQueue.front() };
		mJobQueue.pop();
		mMonitor.jobExecutionStarted(handle.id);
		handle.job.execute();
		return id;
	}
}