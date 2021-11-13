cmake_minimum_required(VERSION 3.21)

add_library(
	job_scheduler_lib STATIC
	"job_scheduler.cpp"
	"job_handle.cpp"
	"worker.cpp"
	"job_queue.cpp"
)

