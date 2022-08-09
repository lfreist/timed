// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

#ifndef TIMED_UTILS_STATISTICS_H_
#define TIMED_UTILS_STATISTICS_H_

namespace timed {
namespace utils {

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Numeric min(const std::vector<Numeric> &vec) {
  return *std::min_element(vec.begin(), vec.end());
}

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Numeric max(const std::vector<Numeric> &vec) {
  return *std::max_element(vec.begin(), vec.end());
}

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
double mean(const std::vector<Numeric> &vec) {
  return std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
}

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
double stddev(const std::vector<Numeric> &vec) {
  double m = mean<Numeric>(vec);
  std::vector<double> diff(vec.size());
  std::transform(vec.begin(), vec.end(), diff.begin(), [m](Numeric x) { return (x - m) * (x - m); });
  return std::sqrt(std::accumulate(diff.begin(), diff.end(), 0.0) / diff.size());
}

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Numeric median(std::vector<Numeric> vec) {
  if (vec.empty()) return 0.0;
  std::sort(vec.begin(), vec.end());
  auto mid = vec.size() / 2U;
  if (1U == (vec.size() & 1U)) {
    return vec[mid];
  }
  return (vec[mid - 1U] + vec[mid]) / 2U;
}

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
double medianAbsolutePercentError(std::vector<Numeric> vec) {
  double med = median<Numeric>(vec);
  std::vector<double> errVec(vec.size());
  for (unsigned i = 0; i < vec.size(); ++i) {
    double tmp = (vec[i] - med) / vec[i];
    if (tmp < 0) tmp = -tmp;
    errVec[i] = tmp;
  }
  return median<double>(errVec);
}

}  // namespace utils
}  // namespace timed

#endif  // TIMED_UTILS_STATISTICS_H_