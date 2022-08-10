// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <vector>

#include "timed/utils/TimeUtils.h"


#ifndef TIMED_UTILS_STATISTICS_H_
#define TIMED_UTILS_STATISTICS_H_

namespace timed {
namespace utils {

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Numeric min(const std::vector<Numeric> &vec);

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Numeric max(const std::vector<Numeric> &vec);

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
double mean(const std::vector<Numeric> &vec);

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
double stddev(const std::vector<Numeric> &vec);

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Numeric median(std::vector<Numeric> vec);

template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
double medianAbsolutePercentError(std::vector<Numeric> vec);

utils::Time min(const std::vector<utils::Time>& vec);

utils::Time max(const std::vector<utils::Time>& vec);

utils::Time mean(const std::vector<utils::Time>& vec);

utils::Time stddev(const std::vector<utils::Time>& vec);

utils::Time median(const std::vector<utils::Time>& vec);

double medianAbsolutePercentError(const std::vector<utils::Time>& vec);

}  // namespace utils
}  // namespace timed

#endif  // TIMED_UTILS_STATISTICS_H_