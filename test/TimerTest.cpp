// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "timed/Timer.h"

using namespace timed;


TEST(WallTimerTest, start_stop) {
  {
    WallTimer wallTimer;
    wallTimer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 100, 1);
  }
  {
    WallTimer wallTimer;
    wallTimer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    wallTimer.stop();
    wallTimer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 100, 1);
  }
}

TEST(WallTimerTest, start_pause) {
  {
    WallTimer wallTimer;
    wallTimer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    wallTimer.pause();
    wallTimer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 200, 2);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}