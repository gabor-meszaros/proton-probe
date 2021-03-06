cmake_minimum_required(VERSION 3.21)
project(test)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/d4e3b8cedf4f57922efa586cb3c796c8fee2d3f3.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
	job_scheduler_test
	"test_job_scheduler.cpp"
)

target_link_libraries(
	job_scheduler_test
	job_scheduler_lib
	gmock_main
)

add_dependencies(
	job_scheduler_test
	job_scheduler_lib
)


target_compile_features(job_scheduler_test PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(job_scheduler_test)