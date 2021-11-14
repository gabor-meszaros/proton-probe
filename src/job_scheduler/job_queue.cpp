#include "job_queue.h"

namespace ProtonProbe
{
	void JobQueue::enqueue(const JobHandle& item)
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);
		
		mQueue.push(item);
	}

	std::optional<JobHandle> JobQueue::pop()
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);

		if (mQueue.empty())
		{
			return {};
		}

		JobHandle& element{ mQueue.front() };
		mQueue.pop();
		return element;
	}
	
	bool JobQueue::empty()
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);

		return mQueue.empty();
	}

	void JobQueue::cancel(const IJob::IdType job)
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);

		std::queue<JobHandle> filteredQueue;
		while (!mQueue.empty())
		{
			JobHandle& item{ mQueue.front() };
			if (item.id != job)
			{
				filteredQueue.push(item);
			}
			mQueue.pop();
		}
		while (!filteredQueue.empty())
		{
			mQueue.push(filteredQueue.front());
			filteredQueue.pop();
		}
	}

	bool JobQueue::contains(const IJob::IdType job)
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);

		bool found{ false };
		std::queue<JobHandle> temporaryQueue;
		while (!mQueue.empty())
		{
			JobHandle& item{ mQueue.front() };
			if (item.id == job)
			{
				found = true;
			}
			temporaryQueue.push(item);
			mQueue.pop();
		}
		while (!temporaryQueue.empty())
		{
			mQueue.push(temporaryQueue.front());
			temporaryQueue.pop();
		}
		return found;
	}
}