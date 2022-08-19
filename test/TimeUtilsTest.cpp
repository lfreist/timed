// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.


#include <gtest/gtest.h>

#include "timed/TimeUtils.h"

using namespace timed::utils;

// ----- TimeValueUnit -------------------------------------------------------------------------------------------------
TEST(TimeValueUnitTest, constructor) {
  {
    TimeValueUnit tvu {
      10.0,
      "s"
    };
    ASSERT_EQ(tvu.value, 10);
    ASSERT_EQ(tvu.unit, "s");
  }
  {
    TimeValueUnit tvu;
    ASSERT_EQ(tvu.value, 0);
    ASSERT_EQ(tvu.unit, "ns");
  }
}

// ----- Time ----------------------------------------------------------------------------------------------------------
TEST(TimeTest, constructor) {
  {
    Time time;
    ASSERT_EQ(time.getNanoseconds(), 0);
  }
  {
    Time time(1);
    ASSERT_EQ(time.getDays(), 1);
  }
  {
    TimeValueUnit tvu {10, "s"};
    Time time(tvu);
    ASSERT_EQ(time.getSeconds(), 10);
  }
  {
    Time time("10:10:10", "%s:%ms:%us");
    ASSERT_EQ(time.getNanoseconds(), 10010010000);
  }
}

TEST(TimeTest, parseTime) {
  Time time;
  time.parseTime("10", "%s");
  ASSERT_EQ(time.getSeconds(), 10);
  time.parseTime("10%0010%", "%s%%ms");
  ASSERT_EQ(time.getMilliseconds(), 1010);
  time.parseTime("1d 10h", "%d %h");
  ASSERT_EQ(time.getHours(), 34);
  time.parseTime("1d 10h  ", "%d %h");
  ASSERT_EQ(time.getHours(), 34);
  time.parseTime("1 day and 10 hours", "%d day and %h hours");
  ASSERT_EQ(time.getHours(), 34);
}

TEST(TimeTest, format) {
  Time time(1, 5, 10, 0, 24, 200, 999);
  ASSERT_EQ(time.format("%d-%h-%m:%s"), "1-5-10:0");
  ASSERT_EQ(time.format("auto"), "1d:5h:0m-24s");
  ASSERT_EQ(time.format("%ns"), "24000000");
}

// TODO: implement missing tests:
TEST(TimeTest, timeInUnit) {}
TEST(TimeTest, getTime) {}
TEST(TimeTest, getDays) {}
TEST(TimeTest, getHours) {}
TEST(TimeTest, getMinutes) {}
TEST(TimeTest, getSeconds) {}
TEST(TimeTest, getMilliseconds) {}
TEST(TimeTest, getMicroseconds) {}
TEST(TimeTest, getNanoseconds) {}

TEST(TimeTest, plus_operator) {}
TEST(TimeTest, minus_operator) {}
TEST(TimeTest, asterix_operator) {}
TEST(TimeTest, divide_operator) {}
TEST(TimeTest, plus_eq_operator) {}
TEST(TimeTest, minus_eq_operator) {}
TEST(TimeTest, asterix_eq_operator) {}
TEST(TimeTest, divide_eq_operator) {}

TEST(TimeTest, eq_operator) {}
TEST(TimeTest, neq_operator) {}
TEST(TimeTest, gt_operator) {}
TEST(TimeTest, ge_operator) {}
TEST(TimeTest, lt_operator) {}
TEST(TimeTest, le_operator) {}

TEST(TimeTest, uint64_t_operator) {}
TEST(TimeTest, long_operator) {}
TEST(TimeTest, double_operator) {}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}