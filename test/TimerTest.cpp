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
    SLEEP_MS(100);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 100, 1);
  }
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.stop();
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 100, 1);
  }
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP_MS(100);
    auto time = wallTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 100, 1);
  }
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.stop();
    wallTimer.start();
    SLEEP_MS(100);
    auto time = wallTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 100, 1);
  }
  {
    WallTimer wallTimer;
    auto time = wallTimer.stop();
    ASSERT_EQ(time.getNanoseconds(), 0);
  }
}

TEST(WallTimerTest, start_pause) {
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP_MS(100);
    auto time = wallTimer.pause();
    ASSERT_NEAR(time.getMilliseconds(), 100, 1);
    SLEEP_MS(100);
    wallTimer.start();
    SLEEP_MS(100);
    time = wallTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 200, 1);
  }
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.pause();
    SLEEP_MS(100);
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 200, 1);
  }
  {
    WallTimer wallTimer;
    auto time = wallTimer.pause();
    ASSERT_EQ(time.getNanoseconds(), 0);
  }
}

TEST(WallTimerTest, multithreaded_task) {
  {
    auto threadedBusyWait = []() -> void {
      std::thread t1 = std::thread(BUSY_WAIT_MS, 100);
      std::thread t2 = std::thread(BUSY_WAIT_MS, 100);
      t1.join();
      t2.join();
    };
    WallTimer wallTimer;
    wallTimer.start();
    threadedBusyWait();
    auto time = wallTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 100, 1);
  }
}

#ifdef _WIN32
#else
TEST(CPUTimerTest, start_stop) {
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    SLEEP_MS(100);
    cpuTimer.stop();
    ASSERT_NEAR(cpuTimer.getTime().getMilliseconds(), 0, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.stop();
    ASSERT_NEAR(cpuTimer.getTime().getMilliseconds(), 100, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.stop();
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.stop();
    ASSERT_NEAR(cpuTimer.getTime().getMilliseconds(), 100, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    SLEEP_MS(100);
    auto time = cpuTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 0, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    SLEEP_MS(100);
    cpuTimer.stop();
    cpuTimer.start();
    SLEEP_MS(100);
    auto time = cpuTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 0, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.stop();
    ASSERT_NEAR(cpuTimer.getTime().getMilliseconds(), 100, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.stop();
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    auto time = cpuTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 100, 1);
  }
  {
    CPUTimer cpuTimer;
    auto time = cpuTimer.stop();
    ASSERT_EQ(time.getNanoseconds(), 0);
  }
}

TEST(CPUTimerTest, start_pause) {
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.pause();
    BUSY_WAIT_MS(100);
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    cpuTimer.stop();
    ASSERT_NEAR(cpuTimer.getTime().getMilliseconds(), 200, 1);
  }
  {
    CPUTimer cpuTimer;
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    auto time = cpuTimer.pause();
    ASSERT_NEAR(time.getMilliseconds(), 100, 1);
    BUSY_WAIT_MS(100);
    cpuTimer.start();
    BUSY_WAIT_MS(100);
    time = cpuTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 200, 1);
  }
  {
    CPUTimer cpuTimer;
    auto time = cpuTimer.pause();
    ASSERT_EQ(time.getNanoseconds(), 0);
  }
}

TEST(CPUTimerTest, multithreaded_task) {
  {
    auto threadedBusyWait = []() -> void {
      std::thread t1 = std::thread(BUSY_WAIT_MS, 100);
      std::thread t2 = std::thread(BUSY_WAIT_MS, 100);
      t1.join();
      t2.join();
    };
    CPUTimer cpuTimer;
    cpuTimer.start();
    threadedBusyWait();
    auto time = cpuTimer.stop();
    ASSERT_NEAR(time.getMilliseconds(), 200, 1);
  }
}

#endif  // _WIN32

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}