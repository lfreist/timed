// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "timed/Timer.h"

using namespace timed;


#define SLEEP_S(x) std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define SLEEP_US(x) std::this_thread::sleep_for(std::chrono::microseconds(x))
#define SLEEP_NS(x) std::this_thread::sleep_for(std::chrono::nanoseconds (x))

#define SLEEP(x) SLEEP_S(x)

void busy_wait(int seconds) {
  WallTimer wt;
  wt.start();
  while(true) {
    auto x = wt.getTime().getSeconds();
    if (x >= seconds) {
      wt.stop();
      return;
    }
  }
}

#define BUSY_WAIT_S(x) [](int y) {                                  \
                            WallTimer wt;                           \
                            wt.start();                             \
                            while(true) {                           \
                              if (wt.getTime().getSeconds() >= y) { \
                                wt.stop();                          \
                                return;                             \
                              }                                     \
                            }                                       \
                          }(x);

#define BUSY_WAIT_MS(x) [](int y) {                                      \
                            WallTimer wt;                                \
                            wt.start();                                  \
                            while(true) {                                \
                              if (wt.getTime().getMilliseconds() >= y) { \
                                wt.stop();                               \
                                return;                                  \
                              }                                          \
                            }                                            \
                          }(x);

#define BUSY_WAIT_US(x) [](int y) {                                      \
                            WallTimer wt;                                \
                            wt.start();                                  \
                            while(true) {                                \
                              if (wt.getTime().getMicroseconds() >= y) { \
                                wt.stop();                               \
                                return;                                  \
                              }                                          \
                            }                                            \
                          }(x);

#define BUSY_WAIT_NS(x) [](int y) {                                     \
                            WallTimer wt;                               \
                            wt.start();                                 \
                            while(true) {                               \
                              if (wt.getTime().getNanoseconds() >= y) { \
                                wt.stop();                              \
                                return;                                 \
                              }                                         \
                            }                                           \
                          }(x);

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
}

TEST(WallTimerTest, start_pause) {
  {
    WallTimer wallTimer;
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.pause();
    SLEEP_MS(100);
    wallTimer.start();
    SLEEP_MS(100);
    wallTimer.stop();
    ASSERT_NEAR(wallTimer.getTime().getMilliseconds(), 200, 5);
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
    SLEEP(1);
    cpuTimer.stop();
    cpuTimer.start();
    SLEEP(1);
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
}

#endif  // _WIN32

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}