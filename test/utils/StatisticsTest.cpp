// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <vector>
#include <cmath>

#include <gtest/gtest.h>

#include "timed/utils/Statistics.h"

namespace stats = timed::utils;

TEST(StatisticsTest, min) {
  {
    std::vector<int> v {1, 10, 5, 1};
    ASSERT_EQ(1, stats::min(v));
    v.push_back(0);
    ASSERT_EQ(0, stats::min(v));
    v.push_back(-5);
    ASSERT_EQ(-5, stats::min(v));
    v.push_back(-3);
    ASSERT_EQ(-5, stats::min(v));
  }
  {
    std::vector<double> v {1.0, 10.5, 5.1, 0.8, 1.1};
    ASSERT_FLOAT_EQ(0.8, stats::min(v));
    v.push_back(0.8001);
    ASSERT_FLOAT_EQ(0.8, stats::min(v));
    v.push_back(0.4);
    ASSERT_FLOAT_EQ(0.4, stats::min(v));
    v.push_back(-0.1);
    ASSERT_FLOAT_EQ(-0.1, stats::min(v));
    v.push_back(0.0);
    ASSERT_FLOAT_EQ(-0.1, stats::min(v));
  }
}

TEST(StatisticsTest, max) {
  {
    std::vector<int> v {1, 10, 5, 1};
    ASSERT_EQ(10, stats::max(v));
    v.push_back(0);
    ASSERT_EQ(10, stats::max(v));
    v.push_back(-5);
    ASSERT_EQ(10, stats::max(v));
    v.push_back(11);
    ASSERT_EQ(11, stats::max(v));
  }
  {
    std::vector<double> v {1.0, 1.1, 0.8, 1.1};
    ASSERT_FLOAT_EQ(1.1, stats::max(v));
    v.push_back(0.8001);
    ASSERT_FLOAT_EQ(1.1, stats::max(v));
    v.push_back(1.10001);
    ASSERT_FLOAT_EQ(1.10001, stats::max(v));
    v.push_back(-0.1);
    ASSERT_FLOAT_EQ(1.10001, stats::max(v));
    v.push_back(3.1);
    ASSERT_FLOAT_EQ(3.1, stats::max(v));
  }
}

TEST(StatisticsTest, mean) {
  {
    std::vector<int> v {2, 2, 2, 4};
    ASSERT_FLOAT_EQ(2.5, stats::mean(v));
  }
  // TODO: add more
}

TEST(StatisticsTest, stddev) {
  {
    std::vector<int> v {2, 2, 2, 4};
    ASSERT_FLOAT_EQ(sqrt(0.75), stats::stddev(v));
  }
  // TODO: add more
}

TEST(StatisticsTest, median) {
  {
    std::vector<int> v {1, 2, 3, 4};
    ASSERT_EQ(2, stats::median(v));
    v.push_back(5);
    ASSERT_EQ(3, stats::median(v));
  }
}

// TODO: add missing tests:
TEST(StatisticsTest, MAPE) {}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}