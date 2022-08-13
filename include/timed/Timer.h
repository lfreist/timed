// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#ifndef TIMED_H_
#define TIMED_H_

#pragma once

#include <chrono>
#include <ctime>
#include <utility>
#include <vector>

#include "timed/utils/TimeUtils.h"
#include "timed/utils/Statistics.h"

// ----- sleep macros --------------------------------------------------------------------------------------------------
#define SLEEP_S(x) std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define SLEEP_US(x) std::this_thread::sleep_for(std::chrono::microseconds(x))
#define SLEEP_NS(x) std::this_thread::sleep_for(std::chrono::nanoseconds (x))

#define SLEEP(x) SLEEP_S(x)

#define BUSY_WAIT_S [](int y) {                                  \
                         WallTimer wt;                           \
                         wt.start();                             \
                         while(true) {                           \
                           if (wt.getTime().getSeconds() >= y) { \
                             wt.stop();                          \
                             return;                             \
                           }                                     \
                         }                                       \
                       }

#define BUSY_WAIT_MS [](int y) {                                       \
                          WallTimer wt;                                \
                          wt.start();                                  \
                          while(true) {                                \
                            if (wt.getTime().getMilliseconds() >= y) { \
                              wt.stop();                               \
                              return;                                  \
                            }                                          \
                          }                                            \
                        }

#define BUSY_WAIT_US [](int y) {                                       \
                          WallTimer wt;                                \
                          wt.start();                                  \
                          while(true) {                                \
                            if (wt.getTime().getMicroseconds() >= y) { \
                              wt.stop();                               \
                              return;                                  \
                            }                                          \
                          }                                            \
                        }

#define BUSY_WAIT_NS [](int y) {                                      \
                          WallTimer wt;                               \
                          wt.start();                                 \
                          while(true) {                               \
                            if (wt.getTime().getNanoseconds() >= y) { \
                              wt.stop();                              \
                              return;                                 \
                            }                                         \
                          }                                           \
                        }

// ---------------------------------------------------------------------------------------------------------------------

namespace timed {

using SteadyClockInterval = std::pair<
    std::chrono::time_point<std::chrono::steady_clock>,
    std::chrono::time_point<std::chrono::steady_clock>
    >;
using ClockTInterval = std::pair<std::clock_t, std::clock_t>;

/**
 * Base class for WallTimer and CPUTimer.
 * Templates for either pair of std::chrono::steady_clock values (WallTimer) or pair of std::clock_t values (CPUTimer).
 * implements:
 *  - reset();
 *  - getIntervals();
 * @tparam Interval
 */
template<typename Interval>
class Timer {
 public:
  virtual ~Timer() = default;

  /**
   * Start the timer. If timer was paused, start() continues the timer.
   */
  virtual void start() = 0;

  /**
   * Pauses the timer. Timer can be resumed using start()
   */
  virtual utils::Time pause() = 0;

  /**
   * Stops the timer. If start() is called subsequent, it will remove previous data
   */
  virtual utils::Time stop() = 0;

  virtual void calibrate() = 0;

  /**
   * Reset the timer
   */
  void reset() {
    _stopped = false;
    _running = false;
    _intervals.clear();
  };

  /**
   * Get elapsed nanoseconds (ns) of collected intervals
   * @return long int: in nanoseconds
   */
  [[nodiscard]] virtual utils::Time getTime() const = 0;

  /**
   * Returns all collected intervals
   * @return Interval
   */
  [[nodiscard]] std::vector<Interval> &getIntervals() const {
    return _intervals;
  }

 protected:
  bool _running = false;
  bool _stopped = false;
  std::vector<Interval> _intervals;
  utils::Time _baseLine;
};

/**
 * WallTimer: simple timer to measure wall time from start() to stop()/pause() call.
 */
class WallTimer : Timer<SteadyClockInterval> {
 public:
  WallTimer() = default;

  void start() override;

  utils::Time pause() override;

  utils::Time stop() override;

  void calibrate() override;

  [[nodiscard]] utils::Time getTime() const override;
};


/**
 * CPUTimer: timer to measure CPU time from start() to stop()/pause() call.
 */
class CPUTimer : Timer<ClockTInterval> {
 public:
  CPUTimer();

  void start() override;

  utils::Time pause() override;

  utils::Time stop() override;

  void calibrate() override;

  [[nodiscard]] utils::Time getTime() const override;
};

}  // namespace timed

#endif  // TIMED_H_