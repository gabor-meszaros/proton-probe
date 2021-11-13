#pragma once

#include <queue>
#include <mutex>

#include "job_handle.h"

namespace ProtonProbe
{
	class JobQueue
	{
	public:
		void enqueue(const JobHandle& item);
		JobHandle& pop();
		bool empty();

	private:
		std::queue<JobHandle> mQueue;
		std::mutex mQueueMutex;
	};
}