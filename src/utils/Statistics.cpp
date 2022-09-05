// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include "timed/utils/Statistics.h"

namespace timed {
namespace utils {

Time min(const std::vector<Time>& vec) {
  if (vec.empty()) { return Time(); }
  Time minTime(vec[0]);
  for (auto& t: vec) {
    if (t < minTime) {
      minTime = t;
    }
  }
  return minTime;
}

Time max(const std::vector<Time>& vec) {
  if (vec.empty()) { return Time(); }
  Time minTime(vec[0]);
  for (auto& t: vec) {
    if (t > minTime) {
      minTime = t;
    }
  }
  return minTime;
}

Time mean(const std::vector<Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](Time t) { return t.getNanoseconds(); });
  TimeValueUnit tvu;
  tvu.value = mean(nsVec);
  tvu.unit = "ns";
  return Time(tvu);
}

Time stddev(const std::vector<Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](Time t) { return t.getNanoseconds(); });
  TimeValueUnit tvu;
  tvu.value = medianAbsolutePercentError(nsVec);
  tvu.unit = "ns";
  return Time(tvu);
}

Time median(const std::vector<Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](Time t) { return t.getNanoseconds(); });
  TimeValueUnit tvu;
  tvu.value = static_cast<double>(median(nsVec));
  tvu.unit = "ns";
  return Time(tvu);
}

double medianAbsolutePercentError(const std::vector<Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](Time t) { return t.getNanoseconds(); });
  return medianAbsolutePercentError(nsVec);
}

}  // namespace utils
}  // namespace timed
