#include "job_scheduler.h"

namespace ProtonProbe
{
	JobScheduler::JobIdType JobScheduler::add(IJob& job)
	{
		job.execute();
		return mNextId++;
	}
}