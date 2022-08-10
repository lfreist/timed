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
  if (_stopped) reset();
  auto now = std::chrono::steady_clock::now();
  _intervals.emplace_back(now, now);
}

// _____________________________________________________________________________________________________________________
void WallTimer::pause() {
  auto now = std::chrono::steady_clock::now();
  _intervals.back().second = now;
}

// _____________________________________________________________________________________________________________________
void WallTimer::stop() {
  auto now = std::chrono::steady_clock::now();
  _intervals.back().second = now;
  _stopped = true;
}

// _____________________________________________________________________________________________________________________
utils::Time WallTimer::getTime() const {
  utils::Time time;
  for (auto &interval: _intervals) {
    time += std::chrono::duration_cast<std::chrono::nanoseconds>(interval.second - interval.first).count();
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
  if (_stopped) reset();
  auto now = std::clock();
  _intervals.emplace_back(now, now);
}

// _____________________________________________________________________________________________________________________
void CPUTimer::pause() {
  auto now = std::clock();
  _intervals.back().second = now;
}

// _____________________________________________________________________________________________________________________
void CPUTimer::stop() {
  auto now = std::clock();
  _intervals.back().second = now;
  _stopped = true;
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
  return utils::Time(tvu);
}

}  // namespace timed