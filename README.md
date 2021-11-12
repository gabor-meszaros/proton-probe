# Proton Probe

This repository contains the implementation of the C++ task that I completed for a Proton Technologies AG interview.

## The task

The task has to be completed in 3 days. The Proton provided description is the following

> Design and implement a generic job system in C++. The system should allow submitting multiple types of jobs, which are placed in a queue. Multiple worker threads consume the queue and run the jobs to completion. Jobs can be cancelled.
> 
> Implementation notes:
> 
>  * The system should be implemented as a single multi-threaded process (the queue should be thread-safe)
>  * The system should accept multiple types of jobs
>  * Should use the classes for multi-threading and concurrency from the C++11 standard library
>  * Jobs should be retried a specified number of times in case of failure
>  * Record basic stats (success, start, and end timestamps, retries, etc.) for completed tasks
>  * The code should be platform independent (though Visual Studio or Xcode can be used for the solution, it should compile and run correctly on multiple platforms)
>  * The solution should use CMake/Ninja for configuration and building
> 
> Bonus points:
> 
>  * Use modern C++ features (C++17) where appropriate and useful
>  * Implement job priorities
>  * Offer the ability to pause running jobs by splitting jobs into multiple subtasks/chunks
>  * Implement possible dependencies between jobs

Because I only had less than a weekend for the implementation, I did not consult Proton about the unclear points but I had to make the decisions myself:

 * *"Design and implement a generic job system in C++."* : The *"system"* is a static library.
 * *"Jobs can be cancelled."* : The cancellation will be graceful. The tasks are asked to stop their work, they are not forcefully interrupted.
 * *"The system should be implemented as a single multi-threaded process"* : The provided test application is the *"process"*.
 * *"The system should accept multiple types of jobs" : The "multiple types"* are implemented through inheritance.
 * *"Jobs should be retried a specified number of times in case of failure"* : The *"specified number"* is not a parameter for the caller but a predefined number.
 * *"Record basic stats (success, start, and end timestamps, retries, etc.)"* : The *"Record"*ing is done through a monitor interface. The interface functions are called for these basic events. Whoever will implement this monitor interface can add time information at the calls, log the events to a file, display them on a screen, forward them to a log service, etc.
 * *"The code should be platform independent (though Visual Studio or Xcode can be used for the solution, it should compile and run correctly on multiple platforms)"* : I did not have other than Windows platform available for testing. However, I was not planning to use anything special that would cause any issues.
 * *"The solution should use CMake/Ninja for configuration and building"* : I am using Visual Studio 2022 Current for developmennt. Its native CMake project uses Ninja. I did not have time to investigate this topic further.
 * *"Implement job priorities"* : The number of levels are limited. (e.g. 2 level of priorities)
 * *"Offer the ability to pause running jobs by splitting jobs into multiple subtasks/chunks"* : That I could not understand after the *"by"*.
