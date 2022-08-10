// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include "timed/utils/TimeUtils.h"

namespace timed {
namespace utils {

// ===== Time ====================================================================================================
// ----- public ________________________________________________________________________________________________________
// _____________________________________________________________________________________________________________________
Time::Time(unsigned int days,
                       unsigned int hours,
                       unsigned int minutes,
                       unsigned int seconds,
                       unsigned int milliseconds,
                       unsigned int microseconds,
                       unsigned int nanoseconds) {
  _days = days;
  _hours = hours;
  _minutes = minutes;
  _seconds = seconds;
  _milliseconds = milliseconds;
  _microseconds = microseconds;
  _nanoseconds = nanoseconds;
  normalize();
}

// _____________________________________________________________________________________________________________________
Time::Time(const TimeValueUnit& timeVU) {
  if (timeVU.unit == "days" || timeVU.unit == "d") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value * 24 * 60 * 60 * 1000 * 1000 * 1000));
  }
  if (timeVU.unit == "hours" || timeVU.unit == "h") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value * 60 * 60 * 1000 * 1000 * 1000));
  }
  if (timeVU.unit == "minutes" || timeVU.unit == "m") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value * 60 * 1000 * 1000 * 1000));
  }
  if (timeVU.unit == "seconds" || timeVU.unit == "s") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value * 1000 * 1000 * 1000));
  }
  if (timeVU.unit == "milliseconds" || timeVU.unit == "ms") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value * 1000 * 1000));
  }
  if (timeVU.unit == "microseconds" || timeVU.unit == "us") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value * 1000));
  }
  if (timeVU.unit == "nanoseconds" || timeVU.unit == "ns") {
    _nanoseconds = static_cast<uint64_t>(std::round(timeVU.value));
  }
}

// _____________________________________________________________________________________________________________________
Time::Time(const std::string& time, const std::string& fmt) {
  parseTime(time, fmt);
  normalize();
}

// _____________________________________________________________________________________________________________________
void Time::parseTime(const std::string& time, const std::string& fmt) {
  bool lastPercent = false;
  int timeIndex = 0;

  auto toValue = [&]() -> int {
    std::string value;
    while (true) {
      char t = time[timeIndex++];
      if (t > 0x29 && t < 0x3a) { value += t; }
      else { break; }
    }
    if (value.empty()) { value = "0"; }
    return std::stoi(value);
  };

  for (int i = 0; i < fmt.size(); ++i) {
    char c = fmt[i];
    switch (c) {
      case '%': {
        if (lastPercent) { throw std::runtime_error("Invalid time format: " + fmt); }
        lastPercent = true;
        break;
      }
      case 'd': {
        if (!lastPercent) { throw std::runtime_error("Invalid time format: " + fmt); }
        _days = toValue();
        lastPercent = false;
        break;
      }
      case 'h': {
        if (!lastPercent) { throw std::runtime_error("Invalid time format: " + fmt); }
        _hours = toValue();
        lastPercent = false;
        break;
      }
      case 'm': {
        if (!lastPercent) { throw std::runtime_error("Invalid time format: " + fmt); }
        // catch milliseconds
        if (fmt[i+1] == 's') {
          i++;
          _milliseconds = toValue();
        }
        else {
          _minutes = toValue();
        }
        lastPercent = false;
        break;
      }
      case 's': {
        if (!lastPercent) { throw std::runtime_error("Invalid time format: " + fmt); }
        _seconds = toValue();
        lastPercent = false;
        break;
      }
      case 'u': {
        if (!lastPercent || fmt[i+1] != 's') { throw std::runtime_error("Invalid time format: " + fmt); }
        i++;
        _microseconds = toValue();
        lastPercent = false;
        break;
      }
      case 'n': {
        if (!lastPercent || fmt[i+1] != 's') { throw std::runtime_error("Invalid time format: " + fmt); }
        _nanoseconds = toValue();
        lastPercent = false;
        break;
      }
      default: {
        if (lastPercent) { throw std::runtime_error("Invalid time format: " + fmt); }
        break;
      }
    }
  }
}

// _____________________________________________________________________________________________________________________
void Time::normalize() {
  uint64_t ns = getNanoseconds();
  _microseconds = ns / 1000;
  _nanoseconds = _nanoseconds - (_microseconds * 1000);
  _milliseconds = _microseconds / 1000;
  _microseconds = _microseconds - (_milliseconds * 1000);
  _seconds = _milliseconds / 1000;
  _milliseconds = _milliseconds - (_seconds * 1000);
  _minutes = _seconds / 60;
  _seconds = _seconds - (_minutes * 60);
  _hours = _minutes / 60;
  _minutes = _minutes - (_hours * 60);
  _days = _hours / 24;
  _hours = _hours - (_days * 24);
}

// _____________________________________________________________________________________________________________________
std::string Time::format(const std::string& fmt) const {
  if (fmt == "auto") {
    std::string autofmt;
    if (_days > 0 || _hours > 0) {
      autofmt = "%dd:%hh:%mm-%ss";
      return format(autofmt);
    }
    if (_seconds > 0 || _milliseconds > 0) {
      autofmt = "%mm%ss%msms";
      return format(autofmt);
    }
    return format("%nsns");
  }

  std::string ret;
  bool lastPercent = false;
  for (int i = 0; i < fmt.size(); ++i) {
    char c = fmt[i];
    switch (c) {
      case '%': {
        lastPercent = true;
        break;
      }
      case 'd': {
        if (lastPercent) {
          ret += std::to_string(_days);
          lastPercent = false;
          break;
        }
      }
      case 'h': {
        if (lastPercent) {
          ret += std::to_string(_hours);
          lastPercent = false;
          break;
        }
      }
      case 'm': {
        if (lastPercent) {
          // catch milliseconds
          if (fmt[i + 1] == 's') {
            i++;
            ret += std::to_string(_milliseconds);
          } else {
            ret += std::to_string(_minutes);
          }
          lastPercent = false;
          break;
        }
      }
      case 's': {
        if (lastPercent) {
          ret += std::to_string(_seconds);
          lastPercent = false;
          break;
        }
      }
      case 'u': {
        if (lastPercent && fmt[i+1] == 's') {
          ++i;
          ret += std::to_string(_microseconds);
          lastPercent = false;
          break;
        }
      }
      case 'n': {
        if (lastPercent && fmt[i+1] == 's') {
          ++i;
          ret += std::to_string(_nanoseconds);
          lastPercent = false;
          break;
        }
      }
      default: {
        ret += c;
        break;
      }
    }
  }
  return ret;
}

// _____________________________________________________________________________________________________________________
double Time::timeInUnit(const std::string& unit) const {
  if (unit == "d" || unit == "days") { return getDays(); }
  if (unit == "h" || unit == "hours") { return getHours(); }
  if (unit == "m" || unit == "minutes") { return getMinutes(); }
  if (unit == "s" || unit == "seconds") { return getSeconds(); }
  if (unit == "ms" || unit == "milliseconds") { return getMilliseconds(); }
  if (unit == "us" || unit == "microseconds") { return getMicroseconds(); }
  if (unit == "ns" || unit == "nanoseconds") { return static_cast<double>(getNanoseconds()); }
  throw std::runtime_error("Unknown unit: " + unit);
}

// _____________________________________________________________________________________________________________________
TimeValueUnit Time::getTime() const {
  if (_days > 4) {
    return {getDays(), "d"};
  }
  if (_days > 0 || _hours > 12) {
    return {getHours(), "h"};
  }
  if (_minutes > 10) {
    return {getMinutes(), "m"};
  }
  if (_seconds > 10) {
    return {getSeconds(), "s"};
  }
  if (_milliseconds > 500) {
    return {getMilliseconds(), "ms"};
  }
  if (_microseconds > 500) {
    return {getMicroseconds(), "us"};
  }
  return {static_cast<double>(getNanoseconds()), "ns"};
}

// _____________________________________________________________________________________________________________________
double Time::getDays() const {
  double d = _days;
  d += _hours / 24;
  d += _minutes / 24 / 60;
  d += _seconds / 24 / 60 / 60;
  d += _milliseconds / 24 / 60 / 60 / 1000;
  d += _microseconds / 24 / 60 / 60 / 1000 / 1000;
  d += _nanoseconds / 24 / 60 / 60 / 1000 / 1000 / 1000;
  return d;
}

// _____________________________________________________________________________________________________________________
double Time::getHours() const {
  double h = _hours;
  h += _days * 24;
  h += _minutes / 60;
  h += _seconds / 60 / 60;
  h += _milliseconds / 60 / 60 / 1000;
  h += _microseconds / 60 / 60 / 1000 / 1000;
  h += _nanoseconds / 60 / 60 / 1000 / 1000 / 1000;
  return h;
}

// _____________________________________________________________________________________________________________________
double Time::getMinutes() const {
  double m = _minutes;
  m += _days * 24 * 60;
  m += _hours * 60;
  m += _seconds / 60;
  m += _milliseconds / 60 / 1000;
  m += _microseconds / 60 / 1000 / 1000;
  m += _nanoseconds / 60 / 1000 / 1000;
  return m;
}

// _____________________________________________________________________________________________________________________
double Time::getSeconds() const {
  double s = _seconds;
  s += _days * 24 * 60 * 60;
  s += _hours * 60 * 60;
  s += _minutes * 60;
  s += _milliseconds / 1000;
  s += _microseconds / 1000 / 1000;
  s += _nanoseconds / 1000 / 1000 / 1000;
  return s;
}

// _____________________________________________________________________________________________________________________
double Time::getMilliseconds() const {
  double ms = _milliseconds;
  ms += _days * 24 * 60 * 60 * 1000;
  ms += _hours * 60 * 60 * 1000;
  ms += _minutes * 60 * 1000;
  ms += _seconds * 1000;
  ms += _microseconds / 1000;
  ms += _nanoseconds / 1000 / 1000;
  return ms;
}

// _____________________________________________________________________________________________________________________
double Time::getMicroseconds() const {
  double us = _microseconds;
  us += _days * 24 * 60 * 60 * 1000 * 1000;
  us += _hours * 60 * 60 * 1000 * 1000;
  us += _minutes * 60 * 1000 * 1000;
  us += _seconds * 1000 * 1000;
  us += _milliseconds * 1000;
  us += _nanoseconds / 1000;
  return us;
}


// _____________________________________________________________________________________________________________________
uint64_t Time::getNanoseconds() const {
  uint64_t ns = _nanoseconds;
  ns += _days * 24 * 60 * 60 * 1000 * 1000 * 1000;
  ns += _hours * 60 * 60 * 1000 * 1000 * 1000;
  ns += _minutes * 60 * 1000 * 1000 * 1000;
  ns += _seconds * 1000 * 1000 * 1000;
  ns += _milliseconds * 1000 * 1000;
  ns += _microseconds * 1000;
  return ns;
}

// _____________________________________________________________________________________________________________________
Time Time::operator+(const Time &t) const {
  return Time(t._days + _days, t._hours + _hours, t._minutes + _minutes, t._seconds + _seconds,
                    t._milliseconds + _milliseconds, t._microseconds + _microseconds, t._nanoseconds + _nanoseconds);
}

// _____________________________________________________________________________________________________________________
Time Time::operator-(const Time &t) const {
  return Time(t._days - _days, t._hours - _hours, t._minutes - _minutes, t._seconds - _seconds,
                    t._milliseconds - _milliseconds, t._microseconds - _microseconds, t._nanoseconds - _nanoseconds);
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Time Time::operator*(Numeric value) {
  return Time(value * _days, value + _hours, value + _minutes, value + _seconds,
                    value + _milliseconds, value + _microseconds, value + _nanoseconds);
}

// _____________________________________________________________________________________________________________________
Time Time::operator*(const Time& t) const {
  uint64_t ns = getNanoseconds() * t.getNanoseconds();
  return Time(0, 0, 0, 0, 0, 0, ns);
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Time Time::operator/(Numeric value) {
  uint64_t ns = getNanoseconds() / value;
  return Time(0, 0, 0, 0, 0, 0, ns);
}

// _____________________________________________________________________________________________________________________
Time &Time::operator+=(const Time &t) {
  _days += t._days;
  _hours += t._hours;
  _minutes += t._minutes;
  _seconds += t._seconds;
  _milliseconds += t._milliseconds;
  _microseconds += t._microseconds;
  _nanoseconds += t._nanoseconds;
  normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
Time &Time::operator+=(uint64_t nanoseconds) {
  _nanoseconds += nanoseconds;
  normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
Time &Time::operator-=(const Time &t) {
  _days -= t._days;
  _hours -= t._hours;
  _minutes -= t._minutes;
  _seconds -= t._seconds;
  _milliseconds -= t._milliseconds;
  _microseconds -= t._microseconds;
  _nanoseconds -= t._nanoseconds;
  normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Time &Time::operator*=(Numeric value) {
  _days *= value;
  _hours *= value;
  _minutes *= value;
  _seconds *= value;
  _milliseconds *= value;
  _microseconds *= value;
  _nanoseconds *= value;
  normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
#if __cplusplus >= 202002L
requires std::is_integral<Numeric> || std::is_floating_point<Numeric>
#endif
Time &Time::operator/=(Numeric value) {
  _nanoseconds = std::round(getNanoseconds() / value);
  normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
bool Time::operator==(const Time &t) const {
  return (getNanoseconds() == t.getNanoseconds());
}

// _____________________________________________________________________________________________________________________
bool Time::operator>(const Time &t) const {
  return (getNanoseconds() > t.getNanoseconds());
}

// _____________________________________________________________________________________________________________________
bool Time::operator>=(const Time &t) const {
  return (getNanoseconds() >= t.getNanoseconds());
}

// _____________________________________________________________________________________________________________________
bool Time::operator<(const Time &t) const {
  return (getNanoseconds() <= t.getNanoseconds());
}

// _____________________________________________________________________________________________________________________
bool Time::operator<=(const Time &t) const {
  return (getNanoseconds() <= t.getNanoseconds());
}

// _____________________________________________________________________________________________________________________
Time::operator uint64_t () const {
  return getNanoseconds();
}

// _____________________________________________________________________________________________________________________
Time::operator long () const {
  return static_cast<long>(getNanoseconds());
}

// _____________________________________________________________________________________________________________________
Time::operator double () const {
  return static_cast<double>(getNanoseconds());
}

// _____________________________________________________________________________________________________________________
std::ostream &operator<<(std::ostream &os, const Time &t) {
  os << t.format("auto");
  return os;
}


}  // namespace utils
}  // namespace timed