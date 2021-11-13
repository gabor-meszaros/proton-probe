#include "job_scheduler.h"

JobScheduler::JobIdType JobScheduler::add(IJob& job)
{
	job.execute();
	return mNextId++;
}
