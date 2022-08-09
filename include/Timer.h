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

namespace timed {

using SteadyClockInterval = std::pair<std::chrono::time_point<std::chrono::steady_clock>, std::chrono::time_point<std::chrono::steady_clock>>;
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
  virtual void pause() = 0;

  /**
   * Stops the timer. If start() is called subsequent, it will remove previous data
   */
  virtual void stop() = 0;

  /**
   * Reset the timer
   */
  void reset() {
    _stopped = false;
    _intervals.clear();
  };

  /**
   * Get elapsed nanoseconds (ns) of collected intervals
   * @return long int: in nanoseconds
   */
  [[nodiscard]] virtual long elapsedNanoseconds() const = 0;

  /**
   * Returns all collected intervals
   * @return Interval
   */
  [[nodiscard]] std::vector<Interval>& getIntervals() const {
    return _intervals;
  }

 protected:
  bool _stopped = false;
  std::vector<Interval> _intervals;
};

/**
 * WallTimer: simple timer to measure wall time from start() to stop()/pause() call.
 */
class WallTimer : Timer<SteadyClockInterval> {
 public:
  WallTimer() = default;

  void start() override;
  void pause() override;
  void stop() override;

  [[nodiscard]] long elapsedNanoseconds() const override;
};


/**
 * CPUTimer: timer to measure CPU time from start() to stop()/pause() call.
 */
class CPUTimer : Timer<ClockTInterval> {
 public:
  CPUTimer() = default;

  void start() override;
  void pause() override;
  void stop() override;

  [[nodiscard]] long elapsedNanoseconds() const override;
};

}  // namespace timed

#endif  // TIMED_H_