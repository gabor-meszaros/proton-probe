#include "job_queue.h"

namespace ProtonProbe
{
	void JobQueue::enqueue(const JobHandle& item)
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);
		
		mQueue.push(item);
	}

	JobHandle& JobQueue::pop()
	{
		const std::lock_guard<std::mutex> lock(mQueueMutex);

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
}