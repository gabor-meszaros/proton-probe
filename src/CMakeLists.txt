# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.21)

include_directories(.)

add_subdirectory("job_scheduler")
add_subdirectory("test")