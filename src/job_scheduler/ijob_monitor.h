#pragma once

#include "ijob.h"

namespace ProtonProbe
{
	class IJobMonitor
	{
	public:
		virtual ~IJobMonitor() {}
		virtual void jobExecutionStarted(const IJob::IdType job) = 0;
	};
}