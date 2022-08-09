// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <stdexcept>

#include "../include/Timer.h"

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
long WallTimer::elapsedNanoseconds() const {
  long elapsed = 0;
  for (auto& interval: _intervals) {
    elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(interval.second - interval.first).count();
  }
  return elapsed;
}

// ===== CPUTimer ======================================================================================================
// ----- public --------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
void CPUTimer::start() {
#ifdef _WIN32
  std::cout << "WARNING: Windows OS does not support CPU time measuring!" << std::endl;
#endif
  if (_stopped) reset();
  auto now = std::clock();
  _intervals.emplace_back(now, now);
}

// _____________________________________________________________________________________________________________________
void CPUTimer::pause() {
#ifdef _WIN32
  std::cout << "WARNING: Windows OS does not support CPU time measuring!" << std::endl;
#endif
  auto now = std::clock();
  _intervals.back().second = now;
}

// _____________________________________________________________________________________________________________________
void CPUTimer::stop() {
#ifdef _WIN32
  std::cout << "WARNING: Windows OS does not support CPU time measuring!" << std::endl;
#endif
  auto now = std::clock();
  _intervals.back().second = now;
  _stopped = true;
}

// _____________________________________________________________________________________________________________________
long CPUTimer::elapsedNanoseconds() const {
#ifdef _WIN32
  std::cout << "WARNING: Windows OS does not support CPU time measuring!" << std::endl;
#endif
  double elapsed = 0;
  for (auto& interval: _intervals) {
    elapsed += 1000.0 * static_cast<double>(interval.second - interval.first) / CLOCKS_PER_SEC;
  }
  return static_cast<long>(elapsed * 1000 * 1000);
}


}  // namespace timed