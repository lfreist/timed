// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include "timed/utils/Statistics.h"

namespace timed {
namespace utils {

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
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  utils::TimeValueUnit tvu;
  tvu.value = mean(nsVec);
  tvu.unit = "ns";
  return utils::Time(tvu);
}

utils::Time stddev(const std::vector<utils::Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  utils::TimeValueUnit tvu;
  tvu.value = medianAbsolutePercentError(nsVec);
  tvu.unit = "ns";
  return utils::Time(tvu);
}

utils::Time median(const std::vector<utils::Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  utils::TimeValueUnit tvu;
  tvu.value = static_cast<double>(median(nsVec));
  tvu.unit = "ns";
  return utils::Time(tvu);
}

double medianAbsolutePercentError(const std::vector<utils::Time>& vec) {
  std::vector<uint64_t> nsVec(vec.size());
  std::transform(vec.begin(), vec.end(), nsVec.begin(), [](utils::Time t) { return t.getNanoseconds(); });
  return medianAbsolutePercentError(nsVec);
}

}  // namespace utils
}  // namespace timed
