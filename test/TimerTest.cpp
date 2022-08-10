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
    ASSERT_NEAR(wallTimer.getTime().getSeconds(), 1, 0.01);
  }
}