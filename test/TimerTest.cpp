// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <gtest/gtest.h>

#include "timed/Timer.h"

using namespace timed;

#ifdef _WIN32
#define SLEEP(x) Sleep(x * 1000)
#else
#define SLEEP(x) sleep(x)
#endif

TEST(WallTimerTest, start_stop) {
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP(1);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 1000, 1);
  }
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP(1);
    wallTimer.stop();
    wallTimer.start();
    SLEEP(1);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 1000, 1);
  }
}

TEST(WallTimerTest, start_pause) {
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP(1);
    wallTimer.pause();
    wallTimer.start();
    SLEEP(1);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 2000, 2);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}