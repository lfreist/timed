// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <iostream>

#include "../include/Benchmark.h"
#include "../include/utils/Statistics.h"

namespace timed {
namespace benchmark {

// ===== Config ========================================================================================================
// _____________________________________________________________________________________________________________________
std::ostream &operator<<(std::ostream &os, const Config &config) {
  os << config.title << "(" << config.iterations;
  os << (config.iterations == 1 ? "iteration" : "iterations") << ")";
  return os;
}


// ===== Results =======================================================================================================
// _____________________________________________________________________________________________________________________
void Result::addCpuTime(long value) {
  cpuTimes.push_back(value);
}

// _____________________________________________________________________________________________________________________
void Result::addWallTime(long value) {
  wallTimes.push_back(value);
}

// _____________________________________________________________________________________________________________________
std::vector<long> Result::adjustedCPUTimes() const {
  std::vector<long> adjustedTimes(cpuTimes.size());
  long baseline = cpuTimeBaseline;
  std::transform(cpuTimes.begin(),
                 cpuTimes.end(),
                 adjustedTimes.begin(),
                 [baseline](long x) { return x - baseline; });
  return adjustedTimes;
}

// _____________________________________________________________________________________________________________________
std::vector<long> Result::adjustedWallTimes() const {
  std::vector<long> adjustedTimes(wallTimes.size());
  long baseline = wallTimeBaseline;
  std::transform(wallTimes.begin(),
                 wallTimes.end(),
                 adjustedTimes.begin(),
                 [baseline](long x) { return x - baseline; });
  return adjustedTimes;
}

// _____________________________________________________________________________________________________________________
std::ostream &operator<<(std::ostream &os, const Result &result) {
  auto adjustedWallTimes = result.adjustedWallTimes();
  auto adjustedCPUTimes = result.adjustedCPUTimes();
  os << "Benchmark: '" << result.title << "'\n";
  if (!result.info.empty()) {
    os << "Info: " << result.info << "\n";
  }
  os << " Iterations: " << result.wallTimes.size() << "\n";
  os << " WallTime [ns]:\n";
  os << "  min:       " << utils::min(adjustedWallTimes) << "\n";
  os << "  max:       " << utils::max(adjustedWallTimes) << "\n";
  os << "  mean:      " << static_cast<ulong>(std::round(utils::mean(adjustedWallTimes))) << "\n";
  os << "  SD:        " << static_cast<ulong>(std::round(utils::stddev(adjustedWallTimes))) << "\n";
  os << "  median:    " << static_cast<ulong>(std::round(utils::median(adjustedWallTimes))) << "\n";
  os << "  %err:      " << utils::medianAbsolutePercentError(adjustedWallTimes) << "\n";
  os << " CPUTime [ns]:\n";
  os << "  min:       " << utils::min(adjustedCPUTimes) << "\n";
  os << "  max:       " << utils::max(adjustedCPUTimes) << "\n";
  os << "  mean:      " << static_cast<ulong>(std::round(utils::mean(adjustedCPUTimes))) << "\n";
  os << "  SD:        " << static_cast<ulong>(std::round(utils::stddev(adjustedCPUTimes))) << "\n";
  os << "  median:    " << static_cast<ulong>(std::round(utils::median(adjustedCPUTimes))) << "\n";
  os << "  %err:      " << utils::medianAbsolutePercentError(adjustedCPUTimes) << "\n";
  return os;
}


// ===== Benchmark =====================================================================================================
// ----- public --------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
Benchmark::Benchmark(std::function<void()> op) {
  _op = std::move(op);
  _precedentOp = []() -> void {};
}

// _____________________________________________________________________________________________________________________
Benchmark::Benchmark(std::function<void()> op, std::function<void()> precedentOp) {
  _op = std::move(op);
  _precedentOp = std::move(precedentOp);
}

// _____________________________________________________________________________________________________________________
Benchmark::Benchmark(Config &config, std::function<void()> op) {
  _op = std::move(op);
  _config = config;
  _result.title = _config.title;
  _result.info = _config.info;
}

// _____________________________________________________________________________________________________________________
Benchmark::Benchmark(Config &config, std::function<void()> op, std::function<void()> precedentOp) {
  _op = std::move(op);
  _precedentOp = std::move(precedentOp);
  _config = config;
  _result.title = _config.title;
  _result.info = _config.info;
}

// _____________________________________________________________________________________________________________________
Result &Benchmark::run(bool verbose) {
  setTimerBaselines();
  WallTimer wallTimer;
  CPUTimer cpuTimer;
  for (unsigned i = 0; i < _config.iterations; ++i) {
    _precedentOp();
    if (verbose) std::cout << '\r' << i << "/" << _config.iterations << std::flush;
    wallTimer.start();
    cpuTimer.start();
    _op();
    cpuTimer.stop();
    wallTimer.stop();
    _result.addWallTime(wallTimer.elapsedNanoseconds());
    _result.addCpuTime(cpuTimer.elapsedNanoseconds());
  }
  if (verbose) std::cout << '\r' << "✅              " << std::endl;
  _run = true;
  return _result;
}

// _____________________________________________________________________________________________________________________
Result &Benchmark::getResult() {
  return _result;
}

// ----- private -------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
void Benchmark::setTimerBaselines() {
  WallTimer wallTimer;
  CPUTimer cpuTimer;
  Result baselineResults;
  auto dummyOperation = [&]() -> void { return; };
  for (int i = 0; i < 500; ++i) {
    wallTimer.start();
    cpuTimer.start();
    dummyOperation();
    cpuTimer.stop();
    wallTimer.stop();
    baselineResults.addWallTime(wallTimer.elapsedNanoseconds());
    baselineResults.addCpuTime(cpuTimer.elapsedNanoseconds());
  }
  _result.wallTimeBaseline = static_cast<long>(std::round(utils::mean(baselineResults.wallTimes)));
  _result.cpuTimeBaseline = static_cast<long>(std::round(utils::mean(baselineResults.cpuTimes)));
}

// _____________________________________________________________________________________________________________________
std::ostream &operator<<(std::ostream &os, const Benchmark &benchmark) {
  if (benchmark._run) {
    os << benchmark._result;
  } else {
    os << benchmark._config;
  }
  return os;
}


// ===== timed =========================================================================================================
// _____________________________________________________________________________________________________________________
Result timed(std::function<void()> op, unsigned iterations, std::function<void()> cleanOp) {
  Config config;
  config.iterations = iterations;
  Benchmark bm(config, std::move(op), std::move(cleanOp));
  auto result = bm.run();
  std::cout << "Wall Time: " << ": (" << utils::mean(result.adjustedWallTimes()) << "+/-"
            << utils::stddev(result.adjustedWallTimes()) << ") ns" << std::endl;
  std::cout << "CPU Time:  " << ": (" << utils::mean(result.adjustedCPUTimes()) << "+/-"
            << utils::stddev(result.adjustedCPUTimes()) << ") ns" << std::endl;
  return bm.run();
}


}  // namespace benchmark
}  // namespace timed