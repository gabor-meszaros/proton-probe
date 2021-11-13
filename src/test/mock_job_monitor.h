#pragma once

#include <gmock/gmock.h>

#include <job_scheduler/ijob_monitor.h>

class MockJobMonitor : public ProtonProbe::IJobMonitor {
public:
	MOCK_METHOD(void, jobExecutionStarted, (const ProtonProbe::IJob::IdType job), (override));
	MOCK_METHOD(void, jobExecutionFinished, (const ProtonProbe::IJob::IdType job), (override));
	MOCK_METHOD(void, jobSucceed, (const ProtonProbe::IJob::IdType job), (override));
};
