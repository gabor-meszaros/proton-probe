#pragma once

#include "ijob.h"

namespace ProtonProbe
{
	class IJobMonitor
	{
	public:
		virtual ~IJobMonitor() {}
		virtual void jobExecutionStarted(const IJob::IdType job) = 0;
		virtual void jobExecutionFinished(const IJob::IdType job) = 0;
		virtual void jobSucceed(const IJob::IdType job) = 0;
		virtual void jobRetryOnFailure(const IJob::IdType job) = 0;
		virtual void jobFailed(const IJob::IdType job) = 0;
	};
}