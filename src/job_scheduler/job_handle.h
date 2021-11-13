#pragma once

#include "ijob.h"

namespace ProtonProbe
{
	struct JobHandle
	{
	public:
		JobHandle(IJob::IdType id, IJob& job);
		
		const IJob::IdType id{ IJob::INVALID_JOB_ID };
		IJob& job;
	};
}