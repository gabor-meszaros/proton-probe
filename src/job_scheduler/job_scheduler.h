#pragma once

#include <thread>
#include <vector>

#include "ijob.h"
#include "ijob_monitor.h"

#include "job_handle.h"
#include "job_queue.h"
#include "worker.h"

namespace ProtonProbe
{
	/// <summary>
	/// A class allowing the user to schedule job execution
	/// </summary>
	class JobScheduler
	{
	public:
		/// <summary>
		/// Allows the user to construct a JobScheduler object
		/// </summary>
		/// <param name="jobMonitor">Used to monitor the job execution</param>
		/// <param name="numberOfWorkers">Configures how many workers the scheduler should use. Less than one worker results an exception</param>
		JobScheduler(IJobMonitor& jobMonitor, unsigned int numberOfWorkers = 1);
		
		/// <summary>
		/// Adds a job to scheduler.
		/// </summary>
		/// <param name="job">A reference to the job. The scheduler does not copy the job, it expects that the object will not be destructed while it is in the system. The "hasProcessed" function can be used to query this information</param>
		/// <returns>The ID that can be used later in the communication with the scheduler to refer to this job. Once the scheduler is stopped, this call will always returns an invalid job ID</returns>
		IJob::IdType add(IJob& job);
		
		/// <summary>
		/// Stops the scheduler including its worker threads. The currently running jobs will be finished
		/// </summary>
		/// <param name="finishRemaininJobs">Controls whether the remaining jobs in the queue should be processed before finishing the threads</param>
		void stop(bool finishRemaininJobs = true);
		
		/// <summary>
		/// Cancells the given job
		/// </summary>
		/// <param name="job">The ID that was received by calling the "add" function. It throws an exception on invalid job ID</param>
		void cancel(const IJob::IdType job);
		
		/// <summary>
		/// Provides information whether the job is still in the system
		/// </summary>
		/// <param name="job">The ID that was received by calling the "add" function. It throws an exception on invalid job ID</param>
		/// <returns>Indicates the result</returns>
		bool hasProcessed(const IJob::IdType job);
		
		~JobScheduler();

	private:
		IJob::IdType mNextId{ 0 };
		JobQueue mJobQueue;
		IJobMonitor& mJobMonitor;
		std::vector<Worker> mWorkers;
		std::vector<std::thread> mWorkerThreads;
		bool mStopped{ false };
	};
}