#include "job_scheduler.h"

#include <stdexcept>

namespace ProtonProbe
{
	JobScheduler::JobScheduler(IJobMonitor& jobMonitor, unsigned int numberOfWorkers)
		: mJobMonitor{ jobMonitor }
	{
		if (numberOfWorkers < 1)
		{
			throw std::invalid_argument("The number of workers must be larger than zero");
		}

		for (unsigned int i{ 0 }; i < numberOfWorkers; ++i)
		{
			mWorkers.emplace_back(mJobQueue, mJobMonitor);
		}
		for (auto& worker : mWorkers)
		{
			mWorkerThreads.emplace_back(std::ref(worker));
		}
	}

	IJob::IdType JobScheduler::add(IJob& job)
	{
		const IJob::IdType id{ mNextId++ };
		JobHandle item{ id, job };
		mJobQueue.enqueue(item);
		return id;
	}

	void JobScheduler::stop(bool finishRemaininJobs)
	{
		for (auto& worker : mWorkers)
		{
			worker.stop(finishRemaininJobs);
		}
		for (auto& thread : mWorkerThreads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	void JobScheduler::cancel(const IJob::IdType job)
	{
		if (IJob::INVALID_JOB_ID == job)
		{
			throw std::invalid_argument("The ID for the cancellation must be valid");
		}

		mJobQueue.cancel(job);
		for (auto& worker : mWorkers)
		{
			worker.cancel(job);
		}
	}

	bool JobScheduler::hasProcessed(const IJob::IdType job)
	{
		if (IJob::INVALID_JOB_ID == job)
		{
			throw std::invalid_argument("The ID for checking the processed state of a job must be valid");
		}

		bool found{ mJobQueue.contains(job) };
		if (found)
		{
			return false;
		}
		for (auto& worker : mWorkers)
		{
			found |= worker.executes(job);
		}
		return !found;
	}

	JobScheduler::~JobScheduler()
	{
		for (auto& worker : mWorkers)
		{
			worker.stop(false);
		}
		for (auto& thread : mWorkerThreads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}
}