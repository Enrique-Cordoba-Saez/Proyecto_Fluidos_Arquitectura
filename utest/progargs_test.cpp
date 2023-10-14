#include "gtest/gtest.h"
#include "sim/progargs.hpp"

TEST(ConstructorTest, NumArgsIncorrectoTest) {
  std::vector<const char *> const args = {"10", "input.txt", "output.txt", "sobrante"};
  EXPECT_EXIT(ProgArgs const progArgs(4, args);, ::testing::ExitedWithCode(-1 + 256), ".*");
}

TEST(ConstructorTest, TimeSteps1IncorrectoTest) {
  std::vector<const char *> const args = {"a", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs const progArgs(3, args);, ::testing::ExitedWithCode(-1 + 256), ".*");
}

TEST(ConstructorTest, TimeSteps2IncorrectoTest) {
  std::vector<const char *> const args = {"-5", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs const progArgs(3, args);, ::testing::ExitedWithCode(-2 + 256), ".*");
}