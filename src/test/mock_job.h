#pragma once

#include <gmock/gmock.h>

#include <job_scheduler/ijob.h>

class MockJob : public IJob {
public:
	MOCK_METHOD(void, execute, (), (override));
};
