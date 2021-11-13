#include "job_scheduler.h"

JobScheduler::JobIdType JobScheduler::add()
{
	return mNextId++;
}
