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

struct TimeValueUnit {
  TimeValueUnit() = default;
  explicit TimeValueUnit(double v, std::string u);
  double value = 0;
  std::string unit = "ns";
};


class Time {
 public:
  explicit Time(uint64_t days = 0, uint64_t hours = 0, uint64_t minutes = 0, uint64_t seconds = 0,
                uint64_t milliseconds = 0, uint64_t microseconds = 0, uint64_t nanoseconds = 0);

  explicit Time(const TimeValueUnit& timeVU);

  Time(const std::string& time, const std::string& fmt);

  void reset();

  void parseTime(const std::string& time, const std::string& fmt);

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
  Time operator*(Numeric value);
  Time operator*(const Time& t) const;

  template<typename Numeric>
  Time operator/(Numeric value);

  Time& operator+=(const Time& t);
  Time& operator+=(uint64_t nanoseconds);
  Time& operator-=(const Time& t);

  template<typename Numeric>
  Time& operator*=(Numeric value);

  template<typename Numeric>
  Time& operator/=(Numeric value);

  bool operator==(const Time& t) const;
  bool operator!=(const Time& t) const;
  bool operator>(const Time& t) const;
  bool operator>=(const Time& t) const;
  bool operator<(const Time& t) const;
  bool operator<=(const Time& t) const;

  explicit operator uint64_t() const;
  explicit operator long() const;
  explicit operator double() const;

 private:
  void _normalize();

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

}  // namespace timed

#endif  // TIMED_UTILS_TIMECONVERTER_H_