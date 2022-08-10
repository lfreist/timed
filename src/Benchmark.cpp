// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <iostream>

#include "timed/Benchmark.h"
#include "timed/utils/Statistics.h"

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
void Result::addCpuTime(utils::Time time) {
  cpuTimes.push_back(time);
}

// _____________________________________________________________________________________________________________________
void Result::addWallTime(utils::Time time) {
  wallTimes.push_back(time);
}

// _____________________________________________________________________________________________________________________
std::vector<utils::Time> Result::adjustedCPUTimes() const {
  std::vector<utils::Time> adjustedTimes(cpuTimes.size());
  utils::Time baseline = cpuTimeBaseline;
  std::transform(cpuTimes.begin(),
                 cpuTimes.end(),
                 adjustedTimes.begin(),
                 [baseline](utils::Time x) { return x - baseline; });
  return adjustedTimes;
}

// _____________________________________________________________________________________________________________________
std::vector<utils::Time> Result::adjustedWallTimes() const {
  std::vector<utils::Time> adjustedTimes(wallTimes.size());
  utils::Time baseline = wallTimeBaseline;
  std::transform(wallTimes.begin(),
                 wallTimes.end(),
                 adjustedTimes.begin(),
                 [baseline](utils::Time x) { return x - baseline; });
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
  os << " WallTime:\n";
  os << "  min:       " << utils::min(adjustedWallTimes) << "\n";
  os << "  max:       " << utils::max(adjustedWallTimes) << "\n";
  os << "  mean:      " << utils::mean(adjustedWallTimes) << "\n";
  os << "  SD:        " << utils::stddev(adjustedWallTimes) << "\n";
  os << "  median:    " << utils::median(adjustedWallTimes) << "\n";
  os << "  %err:      " << utils::medianAbsolutePercentError(adjustedWallTimes) << "\n";
  os << " CPUTime:\n";
  os << "  min:       " << utils::min(adjustedCPUTimes) << "\n";
  os << "  max:       " << utils::max(adjustedCPUTimes) << "\n";
  os << "  mean:      " << utils::mean(adjustedCPUTimes) << "\n";
  os << "  SD:        " << utils::stddev(adjustedCPUTimes) << "\n";
  os << "  median:    " << utils::median(adjustedCPUTimes) << "\n";
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
    _result.addWallTime(wallTimer.getTime());
    _result.addCpuTime(cpuTimer.getTime());
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
    baselineResults.addWallTime(wallTimer.getTime());
    baselineResults.addCpuTime(cpuTimer.getTime());
  }
  _result.wallTimeBaseline = utils::mean(baselineResults.wallTimes);
  _result.cpuTimeBaseline = utils::mean(baselineResults.cpuTimes);
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