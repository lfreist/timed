// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <iostream>

#include "timed/Timer.h"

namespace timed {

// ===== WallTimer =====================================================================================================
// ----- public --------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
void WallTimer::start() {
  if (_running) { return; }
  if (_stopped) reset();
  _running = true;
  auto now = std::chrono::steady_clock::now();
  _intervals.emplace_back(now, now);
}

// _____________________________________________________________________________________________________________________
utils::Time WallTimer::pause() {
  if (!_running) { return getTime(); }
  auto now = std::chrono::steady_clock::now();
  _intervals.back().second = now;
  _running = false;
  return utils::Time(0, 0, 0, 0, 0, 0,
                     std::chrono::duration_cast<std::chrono::nanoseconds>(
                         _intervals.back().second - _intervals.back().first).count());
}

// _____________________________________________________________________________________________________________________
utils::Time WallTimer::stop() {
  if (!_running) { return getTime(); }
  auto now = std::chrono::steady_clock::now();
  _intervals.back().second = now;
  _stopped = true;
  _running = false;
  return getTime();
}

// _____________________________________________________________________________________________________________________
void WallTimer::calibrate() {
  WallTimer wt;
  std::vector<utils::Time> t(1000);
  for (int i = 0; i < 1000; ++i) {
    wt.start();
    t[i] = wt.stop();
    wt.reset();
  }
  _baseLine = utils::mean(t);
}

// _____________________________________________________________________________________________________________________
utils::Time WallTimer::getTime() const {
  std::chrono::time_point<std::chrono::steady_clock> now;
  if (_running) {
    now = std::chrono::steady_clock::now();
  }
  utils::Time time;
  for (auto &interval: _intervals) {
    if (_running && interval == _intervals.back()) {
      time += std::chrono::duration_cast<std::chrono::nanoseconds>(now - interval.first).count();
    }
    else {
      time += std::chrono::duration_cast<std::chrono::nanoseconds>(interval.second - interval.first).count();
    }
  }
  return time;
}

// ===== CPUTimer ======================================================================================================
// ----- public --------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
#ifdef _WIN32
CPUTimer::CPUTimer() {
  std::cout << "WARNING: CPU Timer not supported on Windows OS. Consider using WallTimer instead." << std::endl;
}
#else
CPUTimer::CPUTimer() = default;
#endif

// _____________________________________________________________________________________________________________________
void CPUTimer::start() {
  if (_running) return;
  if (_stopped) reset();
  _running = true;
  auto now = std::clock();
  _intervals.emplace_back(now, now);
}

// _____________________________________________________________________________________________________________________
utils::Time CPUTimer::pause() {
  if (!_running) { return getTime(); }
  auto now = std::clock();
  _intervals.back().second = now;
  auto ms = static_cast<uint64_t>(1000.0 * static_cast<double>(_intervals.back().second - _intervals.back().first) /
                                  CLOCKS_PER_SEC);
  _running = false;
  return utils::Time(0, 0, 0, 0, ms, 0, 0);
}

// _____________________________________________________________________________________________________________________
utils::Time CPUTimer::stop() {
  if (!_running) { return getTime(); }
  auto now = std::clock();
  _intervals.back().second = now;
  _stopped = true;
  _running = false;
  return getTime();
}

// _____________________________________________________________________________________________________________________
void CPUTimer::calibrate() {
  CPUTimer cput;
  std::vector<utils::Time> t(1000);
  for (int i = 0; i < 1000; ++i) {
    cput.start();
    cput.stop();
    t.push_back(cput.getTime());
  }
  _baseLine = utils::mean(t);
}

// _____________________________________________________________________________________________________________________
utils::Time CPUTimer::getTime() const {
  double time = 0;
  for (auto &interval: _intervals) {
    time += 1000.0 * static_cast<double>(interval.second - interval.first) / CLOCKS_PER_SEC;
  }
  utils::TimeValueUnit tvu;
  tvu.value = time * 1000 * 1000;
  tvu.unit = "ns";
  return utils::Time(tvu) - _baseLine;
}

}  // namespace timed