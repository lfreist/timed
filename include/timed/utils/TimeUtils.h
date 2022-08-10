// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <string>
#include <vector>
#include <map>
#include <ostream>

#ifndef TIMED_UTILS_TIMECONVERTER_H_
#define TIMED_UTILS_TIMECONVERTER_H_

namespace timed {
namespace utils {

struct TimeValueUnit {
  double value = 0;
  std::string unit = "ns";
};


class Time {
 public:
  explicit Time(unsigned days = 0, unsigned hours = 0, unsigned minutes = 0, unsigned seconds = 0,
             unsigned milliseconds = 0, unsigned microseconds = 0, unsigned nanoseconds = 0);

  explicit Time(const TimeValueUnit& timeVU);

  Time(const std::string& time, const std::string& fmt);

  void parseTime(const std::string& time, const std::string& fmt);

  void normalize();

  std::string format(const std::string& fmt = "auto") const;

  double timeInUnit(const std::string& unit) const;

  TimeValueUnit getTime() const;

  double getDays() const;
  double getHours() const;
  double getMinutes() const;
  double getSeconds() const;
  double getMilliseconds() const;
  double getMicroseconds() const;
  uint64_t getNanoseconds() const;

  Time operator+(const Time& t) const;
  Time operator-(const Time& t) const;

  template<typename Numeric>
#if __cplusplus >= 202002L
  requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
  Time operator*(Numeric value);
  Time operator*(const Time& t) const;

  template<typename Numeric>
#if __cplusplus >= 202002L
  requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
  Time operator/(Numeric value);

  Time& operator+=(const Time& t);
  Time& operator+=(uint64_t nanoseconds);
  Time& operator-=(const Time& t);

  template<typename Numeric>
#if __cplusplus >= 202002L
  requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
  Time& operator*=(Numeric value);

  template<typename Numeric>
#if __cplusplus >= 202002L
  requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
  Time& operator/=(Numeric value);

  bool operator==(const Time& t) const;
  bool operator>(const Time& t) const;
  bool operator>=(const Time& t) const;
  bool operator<(const Time& t) const;
  bool operator<=(const Time& t) const;

  explicit operator uint64_t() const;
  explicit operator long() const;
  operator double() const;

  static const std::map<std::string, std::string> validUnits;

 private:
  uint64_t _days = 0;
  uint64_t _hours = 0;
  uint64_t _minutes = 0;
  uint64_t _seconds = 0;
  uint64_t _milliseconds = 0;
  uint64_t _microseconds = 0;
  uint64_t _nanoseconds = 0;

  friend std::ostream &operator<<(std::ostream &os, const Time &tf);
};

std::ostream &operator<<(std::ostream &os, const Time &tf);


}  // namespace utils
}  // namespace timed

#endif  // TIMED_UTILS_TIMECONVERTER_H_