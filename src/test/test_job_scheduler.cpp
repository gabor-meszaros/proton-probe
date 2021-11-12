#include <gtest/gtest.h>
#include <job_scheduler/job_scheduler.h>

TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
	// Test library.
	EXPECT_EQ(test(), 42);
}
