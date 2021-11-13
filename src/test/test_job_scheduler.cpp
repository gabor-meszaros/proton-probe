#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <job_scheduler/job_scheduler.h>
#include "mock_job.h"
#include "mock_job_monitor.h"

using namespace testing;
using namespace ProtonProbe;

class ProtonJob : public IJob
{
	// Inherited via IJob
	virtual void execute() override
	{
	}
};

TEST(AJobScheduler, ReturnsValidIdWhenAddingAJobWithoutError) {
	NiceMock<MockJobMonitor> jobMonitor;
	JobScheduler scheduler(jobMonitor);

	NiceMock<MockJob> job;
	const IJob::IdType arbitraryId{ scheduler.add(job) };

	ASSERT_THAT(arbitraryId, Ne(IJob::INVALID_JOB_ID));
}

TEST(AJobScheduler, ReturndsUniqueJobIds) {
	NiceMock<MockJobMonitor> jobMonitor;
	JobScheduler scheduler(jobMonitor);

	NiceMock<MockJob> job;
	const IJob::IdType firstCallReturn{ scheduler.add(job) };
	const IJob::IdType secondCallReturn{ scheduler.add(job) };

	ASSERT_THAT(firstCallReturn, Ne(secondCallReturn));
}

TEST(AJobScheduler, ExecutesTheReceivedJob) {
	NiceMock<MockJobMonitor> jobMonitor;
	JobScheduler scheduler(jobMonitor);

	MockJob job;
	EXPECT_CALL(job, execute())
		.Times(1);
	
	scheduler.add(job);
}

TEST(AJobScheduler, AcceptsDifferentTypeOfJobs) {
	NiceMock<MockJobMonitor> jobMonitor;
	JobScheduler scheduler(jobMonitor);

	NiceMock<MockJob> aJobWithTypeA;
	ProtonJob aJobWithTypeB;

	ASSERT_NO_THROW(scheduler.add(aJobWithTypeA));
	ASSERT_NO_THROW(scheduler.add(aJobWithTypeB));
}

TEST(AJobScheduler, MonitorsJobExecutionStart) {
	MockJobMonitor jobMonitor;
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(1);
	
	JobScheduler scheduler(jobMonitor);

	NiceMock<MockJob> job;
	scheduler.add(job);
}