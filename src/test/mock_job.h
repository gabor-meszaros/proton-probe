#pragma once

#include <gmock/gmock.h>

#include <job_scheduler/ijob.h>

class MockJob : public ProtonProbe::IJob {
public:
	MOCK_METHOD(bool, execute, (const bool& stopExecution), (override));
};
