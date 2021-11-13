#pragma once

#include <queue>
#include <mutex>

#include "ijob.h"

#include "job_handle.h"

namespace ProtonProbe
{
	class JobQueue
	{
	public:
		void enqueue(const JobHandle& item);
		JobHandle& pop();
		bool empty();
		void cancel(const IJob::IdType job);

	private:
		std::queue<JobHandle> mQueue;
		std::mutex mQueueMutex;
	};
}