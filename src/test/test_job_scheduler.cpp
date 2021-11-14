#include <chrono>
#include <stdexcept>
#include <thread>

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
	virtual bool execute(const bool& stopExecution /* We never stop */) override
	{
		return true; // We always succeed :)
	}
};

class InfiniteCancellableJob : public IJob
{
	// Inherited via IJob
	virtual bool execute(const bool& stopExecution) override
	{
		while (!stopExecution)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		return true;
	}
};

class AJobScheduler : public ::testing::Test
{
protected:
	static const unsigned int singleWorker{ 1 };
	static const unsigned int moreThanOneWorkers{ 5 };
	NiceMock<MockJobMonitor> jobMonitor;
	NiceMock<MockJob> arbitraryJob;
	InfiniteCancellableJob infiniteCancellableJob;


	static void waitUntilEveryJobWasProcessedAndStop(JobScheduler& scheduler)
	{
		constexpr bool finishJobsBeforeStop{ true };
		scheduler.stop(finishJobsBeforeStop);
	}

	IJob::IdType addInfiniteCancellableJob(JobScheduler& scheduler)
	{
		const auto id{ scheduler.add(infiniteCancellableJob) };
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait until the worker picks it up
		return id;
	}
};

TEST_F(AJobScheduler, ReturnsValidIdWhenAddingAJobWithoutError) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	const IJob::IdType arbitraryId{ scheduler.add(arbitraryJob) };

	ASSERT_THAT(arbitraryId, Ne(IJob::INVALID_JOB_ID));

	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, ReturnsUniqueJobIds) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	const IJob::IdType firstCallReturn{ scheduler.add(arbitraryJob) };
	const IJob::IdType secondCallReturn{ scheduler.add(arbitraryJob) };

	ASSERT_THAT(firstCallReturn, Ne(secondCallReturn));

	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, ExecutesTheReceivedJob) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	NiceMock<MockJob> aJobHasToBeExecuted;
	EXPECT_CALL(aJobHasToBeExecuted, execute(_))
		.Times(1)
		.WillOnce(Return(true));

	scheduler.add(aJobHasToBeExecuted);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, AcceptsDifferentTypeOfJobs) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	NiceMock<MockJob> aJobWithTypeA;
	ProtonJob aJobWithTypeB;

	ASSERT_NO_THROW(scheduler.add(aJobWithTypeA));
	ASSERT_NO_THROW(scheduler.add(aJobWithTypeB));
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, MonitorsJobExecutionStart) {
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(1);

	JobScheduler scheduler(jobMonitor, singleWorker);

	scheduler.add(arbitraryJob);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, RunsEveryJobOnceEvenUnderStress) {
	constexpr int numberOfJobs{ 10000 };
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(numberOfJobs);

	JobScheduler scheduler(jobMonitor, singleWorker);
		
	for (int i{ 0 }; i < numberOfJobs; ++i)
	{
		scheduler.add(arbitraryJob);
	}
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, ThrowsOnLessThanOneWorkerConfiguration) {
	constexpr unsigned int lessThanOneWorkerConfiguration{ 0 };

	ASSERT_THROW(JobScheduler scheduler(jobMonitor, lessThanOneWorkerConfiguration), std::invalid_argument);
}

TEST_F(AJobScheduler, RunsEveryJobOnceWithMultipleWorkersEvenUnderStress) {
	constexpr int numberOfJobs{ 10000 };
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(numberOfJobs);

	JobScheduler scheduler(jobMonitor, moreThanOneWorkers);

	for (int i{ 0 }; i < numberOfJobs; ++i)
	{
		scheduler.add(arbitraryJob);
	}

	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, MonitorsJobExecutionFinish) {
	EXPECT_CALL(jobMonitor, jobExecutionFinished(_))
		.Times(1);

	JobScheduler scheduler(jobMonitor, singleWorker);

	scheduler.add(arbitraryJob);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, MonitorsJobExecutionSuccess) {
	EXPECT_CALL(jobMonitor, jobSucceed(_))
		.Times(1);

	JobScheduler scheduler(jobMonitor, singleWorker);

	NiceMock<MockJob> nonFailingJob;
	EXPECT_CALL(nonFailingJob, execute(_))
		.WillRepeatedly(Return(true));
	
	scheduler.add(nonFailingJob);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, MonitorsJobExecutionRetries) {
	EXPECT_CALL(jobMonitor, jobRetryOnFailure(_))
		.Times(AtLeast(1));

	JobScheduler scheduler(jobMonitor, singleWorker);

	NiceMock<MockJob> failingJob;
	EXPECT_CALL(failingJob, execute(_))
		.WillRepeatedly(Return(false));
	
	scheduler.add(failingJob);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, RetriesFailedJobExecutionFiveTimes) {
	constexpr int numberOfRetries{ 5 };
	EXPECT_CALL(jobMonitor, jobRetryOnFailure(_))
		.Times(numberOfRetries);

	JobScheduler scheduler(jobMonitor, singleWorker);

	NiceMock<MockJob> failingJob;
	EXPECT_CALL(failingJob, execute(_))
		.Times(numberOfRetries + 1)
		.WillRepeatedly(Return(false));
	
	scheduler.add(failingJob);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, MonitorsJobExecutionFailure) {
	EXPECT_CALL(jobMonitor, jobFailed(_))
		.Times(1);

	JobScheduler scheduler(jobMonitor, singleWorker);

	NiceMock<MockJob> failingJob;
	EXPECT_CALL(failingJob, execute(_))
		.WillRepeatedly(Return(false));
	
	scheduler.add(failingJob);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, CanCancelJobsWhileTheyAreExecuted) {
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(1);

	JobScheduler scheduler(jobMonitor, singleWorker);

	const IJob::IdType jobTheWorkerIsWorkingOn{ addInfiniteCancellableJob(scheduler) };
	
	scheduler.cancel(jobTheWorkerIsWorkingOn);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, CanCancelANotYetExecutedJob) {
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(1);

	JobScheduler scheduler(jobMonitor, singleWorker);

	const IJob::IdType jobForWorker{ addInfiniteCancellableJob(scheduler) };
	
	NiceMock<MockJob> jobForTheQueue;
	EXPECT_CALL(jobForTheQueue, execute(_))
		.Times(0);
	
	const IJob::IdType jobForQueue{ scheduler.add(jobForTheQueue) };
	
	scheduler.cancel(jobForQueue);
	scheduler.cancel(jobForWorker);
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, RunsEveryJobOnceEvenRunningOftenOnEmptyQueue) {
	constexpr int numberOfJobs{ 150 };
	EXPECT_CALL(jobMonitor, jobExecutionStarted(_))
		.Times(numberOfJobs);

	JobScheduler scheduler(jobMonitor, moreThanOneWorkers);

	for (int i{ 0 }; i < numberOfJobs; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		scheduler.add(arbitraryJob);
	}
	
	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, ExecutesTheJobsInFifoOrder) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	const IJob::IdType blockingJobUntilFinishingSetup{ addInfiniteCancellableJob(scheduler) };

	const IJob::IdType firstJobId{ scheduler.add(arbitraryJob) };
	const IJob::IdType secondJobId{ scheduler.add(arbitraryJob) };
	const IJob::IdType thirdJobId{ scheduler.add(arbitraryJob) };

	{
		InSequence s;
		EXPECT_CALL(jobMonitor, jobExecutionStarted(firstJobId))
			.Times(1);
		EXPECT_CALL(jobMonitor, jobExecutionStarted(secondJobId))
			.Times(1);
		EXPECT_CALL(jobMonitor, jobExecutionStarted(thirdJobId))
			.Times(1);
	}

	scheduler.cancel(blockingJobUntilFinishingSetup);

	waitUntilEveryJobWasProcessedAndStop(scheduler);
}

TEST_F(AJobScheduler, CanTellIfAJobHasBeenProcessed) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	const IJob::IdType blockingJob{ addInfiniteCancellableJob(scheduler) };
	const IJob::IdType jobForQueue{ scheduler.add(arbitraryJob) };

	ASSERT_THAT(scheduler.hasProcessed(blockingJob), Eq(false));
	ASSERT_THAT(scheduler.hasProcessed(jobForQueue), Eq(false));

	scheduler.cancel(blockingJob);
	waitUntilEveryJobWasProcessedAndStop(scheduler);

	ASSERT_THAT(scheduler.hasProcessed(blockingJob), Eq(true));
	ASSERT_THAT(scheduler.hasProcessed(jobForQueue), Eq(true));
}

TEST_F(AJobScheduler, ThrowsOnCancellingAnInvalidJobId) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	ASSERT_THROW(scheduler.cancel(IJob::INVALID_JOB_ID), std::invalid_argument);
}

TEST_F(AJobScheduler, ThrowsOnCheckingJobProcessedStateWithInvalidJobId) {
	JobScheduler scheduler(jobMonitor, singleWorker);

	ASSERT_THROW(scheduler.hasProcessed(IJob::INVALID_JOB_ID), std::invalid_argument);
}

TEST_F(AJobScheduler, AddFunctionReturnsInvalidJobIdAfterStop) {
	JobScheduler stoppedScheduler(jobMonitor, singleWorker);
	stoppedScheduler.stop(false /* Do not process the remainig jobs in the queue */);

	const IJob::IdType stoppedSchedulerAddJobId{ stoppedScheduler.add(arbitraryJob) };

	ASSERT_THAT(stoppedSchedulerAddJobId, Eq(IJob::INVALID_JOB_ID));
}