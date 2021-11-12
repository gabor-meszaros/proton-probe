#pragma once

class JobScheduler
{
public:
	typedef int JobIdType;

	const static JobIdType INVALID_JOB_ID{ -1 };

	JobIdType add();
};
