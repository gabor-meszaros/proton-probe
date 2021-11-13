#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <job_scheduler/job_scheduler.h>

using namespace testing;

TEST(AJobScheduler, ReturnsValidIdWhenAddingAJobWithoutError) {
	JobScheduler scheduler;

	const JobScheduler::JobIdType arbitraryId{ scheduler.add() };

	ASSERT_THAT(arbitraryId, Ne(JobScheduler::INVALID_JOB_ID));
}

TEST(AJobScheduler, ReturndsUniqueJobIds) {
	JobScheduler scheduler;

	const JobScheduler::JobIdType firstCallReturn{ scheduler.add() };
	const JobScheduler::JobIdType secondCallReturn{ scheduler.add() };

	ASSERT_THAT(firstCallReturn, Ne(secondCallReturn));
}
