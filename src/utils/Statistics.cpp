// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <numeric>
#include <cmath>
#include <algorithm>

#include "timed/utils/Statistics.h"

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
  double m = mean(vec);
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
  return median(errVec);
}

utils::Time min(const std::vector<utils::Time>& vec) {
  if (vec.empty()) { return utils::Time(); }
  utils::Time minTime(vec[0]);
  for (auto& t: vec) {
    if (t < minTime) {
      minTime = t;
    }
  }
  return minTime;
}

utils::Time max(const std::vector<utils::Time>& vec) {
  if (vec.empty()) { return utils::Time(); }
  utils::Time minTime(vec[0]);
  for (auto& t: vec) {
    if (t > minTime) {
      minTime = t;
    }
  }
  return minTime;
}

utils::Time mean(const std::vector<utils::Time>& vec) {
  return std::accumulate(vec.begin(), vec.end(), utils::Time()) / vec.size();
}

utils::Time stddev(const std::vector<utils::Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  utils::TimeValueUnit tvu {
    medianAbsolutePercentError(nsVec),
    "ns"
  };
  return utils::Time(tvu);
}

utils::Time median(const std::vector<utils::Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  utils::TimeValueUnit tvu {
    static_cast<double>(median(nsVec)),
    "ns"
  };
  return utils::Time(tvu);
}

double medianAbsolutePercentError(const std::vector<utils::Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  return medianAbsolutePercentError(nsVec);
}

}  // namespace utils
}  // namespace timed
