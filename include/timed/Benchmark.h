// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#ifndef TIMED_BENCHMARK_H_
#define TIMED_BENCHMARK_H_

#pragma once

#include <string>
#include <algorithm>
#include <ostream>
#include <functional>

#include "timed/Timer.h"
#include "timed/TimeUtils.h"

#define BENCHMARK(func, ...) [](auto&& func, auto&& ...__VA_ARGS__)

namespace timed {
namespace benchmark {


struct Config {
  std::string title = "Benchmark";
  std::string info;
  unsigned iterations = 1;
};

std::ostream &operator<<(std::ostream &os, const Config &config);


struct Result {
  std::string title = "Benchmark";
  std::string info;
  std::vector<Time> wallTimes;
  std::vector<Time> cpuTimes;
  Time wallTimeBaseline;
  Time cpuTimeBaseline;

  void addWallTime(Time time);

  void addCpuTime(Time time);

  [[nodiscard]] std::vector<Time> adjustedWallTimes() const;

  [[nodiscard]] std::vector<Time> adjustedCPUTimes() const;
};

std::ostream &operator<<(std::ostream &os, const Result &result);


class Benchmark {
 public:
  explicit Benchmark(std::function<void()> op);

  Benchmark(std::function<void()> op, std::function<void()> precedentOp);

  Benchmark(Config &config, std::function<void()> op);

  Benchmark(Config &config, std::function<void()> op, std::function<void()> precedentOp);

  Result &run(bool verbose = false);

  Result &getResult();

  const Result& getResult() const;

  const Config &getConfig() const;

 private:

  void setTimerBaselines();

  // operation that will be benchmarked
  std::function<void()> _op;
  // operation that is run before each iteration to clean up and/or reset things
  std::function<void()> _precedentOp;
  Config _config;
  Result _result;
  bool _run = false;

  friend std::ostream &operator<<(std::ostream &os, const Benchmark &benchmark);
};

std::ostream &operator<<(std::ostream &os, const Benchmark &benchmark);


Result timed(std::function<void()> op, unsigned iterations = 5, std::function<void()> cleanOp = [](){});

}  // namespace timed
}  // namespace benchmark

#endif  // TIMED_BENCHMARK_H_