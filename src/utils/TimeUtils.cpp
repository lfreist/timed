// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>
//
// This file is part of the "timed"-library which is licenced under the MIT-license. For more detail read LICENCE.

#include <cmath>

#include "timed/utils/TimeUtils.h"

namespace timed {
namespace utils {


// ===== Time ====================================================================================================
// ----- public ________________________________________________________________________________________________________
// _____________________________________________________________________________________________________________________
TimeValueUnit::TimeValueUnit(double v, std::string u) {
  value = v;
  unit = std::move(u);
}

// ===== Time ====================================================================================================
// ----- public ________________________________________________________________________________________________________
// _____________________________________________________________________________________________________________________
Time::Time(uint64_t days,
           uint64_t hours,
           uint64_t minutes,
           uint64_t seconds,
           uint64_t milliseconds,
           uint64_t microseconds,
           uint64_t nanoseconds) {
  _days = days;
  _hours = hours;
  _minutes = minutes;
  _seconds = seconds;
  _milliseconds = milliseconds;
  _microseconds = microseconds;
  _nanoseconds = nanoseconds;
  _normalize();
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
  _normalize();
}

// _____________________________________________________________________________________________________________________
void Time::reset() {
  _days = 0;
  _hours = 0;
  _minutes = 0;
  _seconds = 0;
  _milliseconds = 0;
  _microseconds = 0;
  _nanoseconds = 0;
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
        lastPercent = !lastPercent;
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
        break;
      }
    }
  }
}

// _____________________________________________________________________________________________________________________
// TODO: fix this: if no superior unit of a provided unit is provided -> calculate superior units into unit.
//  e.g.: h is provided but d is not provided -> consider d as part of h: (h: 2, d: 1 -> h = 2 + 24 = 26)
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
          if (fmt.find("%d") != std::string::npos) {
            ret += std::to_string(_days * 24 + _hours);
          }
          else {
            ret += std::to_string(_hours);
          }
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
            if (fmt.find("%d") != std::string::npos && fmt.find("%h") != std::string::npos) {
              ret += std::to_string(_days * 24 * 60 + _hours * 60 + _minutes);
            }
            else if (fmt.find("%h") != std::string::npos) {
              ret += std::to_string(_hours * 60 + _minutes);
            }
            else {
              ret += std::to_string(_minutes);
            }
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
  TimeValueUnit tvu;
  if (_days > 4) {
    tvu.value = getDays();
    tvu.unit = "d";
    return tvu;
  }
  if (_days > 0 || _hours > 12) {
    tvu.value = getHours();
    tvu.unit = "h";
    return tvu;
  }
  if (_minutes > 10) {
    tvu.value = getMinutes();
    tvu.unit = "m";
    return tvu;
  }
  if (_seconds > 10) {
    tvu.value = getSeconds();
    tvu.unit = "s";
    return tvu;
  }
  if (_milliseconds > 500) {
    tvu.value = getMilliseconds();
    tvu.unit = "ms";
    return tvu;
  }
  if (_microseconds > 500) {
    tvu.value = getMicroseconds();
    tvu.unit = "us";
    return tvu;
  }
  tvu.value = static_cast<double>(getNanoseconds());
  tvu.unit = "ns";
  return tvu;
}

// _____________________________________________________________________________________________________________________
double Time::getDays() const {
  auto d = static_cast<double>(_days);
  d += static_cast<double>(_hours) / 24;
  d += static_cast<double>(_minutes) / 24 / 60;
  d += static_cast<double>(_seconds) / 24 / 60 / 60;
  d += static_cast<double>(_milliseconds) / 24 / 60 / 60 / 1000;
  d += static_cast<double>(_microseconds) / 24 / 60 / 60 / 1000 / 1000;
  d += static_cast<double>(_nanoseconds) / 24 / 60 / 60 / 1000 / 1000 / 1000;
  return d;
}

// _____________________________________________________________________________________________________________________
double Time::getHours() const {
  auto h = static_cast<double>(_hours);
  h += static_cast<double>(_days) * 24;
  h += static_cast<double>(_minutes) / 60;
  h += static_cast<double>(_seconds) / 60 / 60;
  h += static_cast<double>(_milliseconds) / 60 / 60 / 1000;
  h += static_cast<double>(_microseconds) / 60 / 60 / 1000 / 1000;
  h += static_cast<double>(_nanoseconds) / 60 / 60 / 1000 / 1000 / 1000;
  return h;
}

// _____________________________________________________________________________________________________________________
double Time::getMinutes() const {
  auto m = static_cast<double>(_minutes);
  m += static_cast<double>(_days) * 24 * 60;
  m += static_cast<double>(_hours) * 60;
  m += static_cast<double>(_seconds) / 60;
  m += static_cast<double>(_milliseconds) / 60 / 1000;
  m += static_cast<double>(_microseconds) / 60 / 1000 / 1000;
  m += static_cast<double>(_nanoseconds) / 60 / 1000 / 1000;
  return m;
}

// _____________________________________________________________________________________________________________________
double Time::getSeconds() const {
  auto s = static_cast<double>(_seconds);
  s += static_cast<double>(_days) * 24 * 60 * 60;
  s += static_cast<double>(_hours) * 60 * 60;
  s += static_cast<double>(_minutes) * 60;
  s += static_cast<double>(_milliseconds) / 1000;
  s += static_cast<double>(_microseconds) / 1000 / 1000;
  s += static_cast<double>(_nanoseconds) / 1000 / 1000 / 1000;
  return s;
}

// _____________________________________________________________________________________________________________________
double Time::getMilliseconds() const {
  auto ms = static_cast<double>(_milliseconds);
  ms += static_cast<double>(_days) * 24 * 60 * 60 * 1000;
  ms += static_cast<double>(_hours) * 60 * 60 * 1000;
  ms += static_cast<double>(_minutes) * 60 * 1000;
  ms += static_cast<double>(_seconds) * 1000;
  ms += static_cast<double>(_microseconds) / 1000;
  ms += static_cast<double>(_nanoseconds) / 1000 / 1000;
  return ms;
}

// _____________________________________________________________________________________________________________________
double Time::getMicroseconds() const {
  auto us = static_cast<double>(_microseconds);
  us += static_cast<double>(_days) * 24 * 60 * 60 * 1000 * 1000;
  us += static_cast<double>(_hours) * 60 * 60 * 1000 * 1000;
  us += static_cast<double>(_minutes) * 60 * 1000 * 1000;
  us += static_cast<double>(_seconds) * 1000 * 1000;
  us += static_cast<double>(_milliseconds) * 1000;
  us += static_cast<double>(_nanoseconds) / 1000;
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
  if (t > *this) {
    return Time();
  }
  auto ns = getNanoseconds() - t.getNanoseconds();
  return Time(0, 0, 0, 0, 0, 0, ns);
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
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
  _normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
Time &Time::operator+=(uint64_t nanoseconds) {
  _nanoseconds += nanoseconds;
  _normalize();
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
  _normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
Time &Time::operator*=(Numeric value) {
  _days *= value;
  _hours *= value;
  _minutes *= value;
  _seconds *= value;
  _milliseconds *= value;
  _microseconds *= value;
  _nanoseconds *= value;
  _normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
template<typename Numeric>
Time &Time::operator/=(Numeric value) {
  _nanoseconds = std::round(getNanoseconds() / value);
  _days = 0;
  _hours = 0;
  _minutes = 0;
  _seconds = 0;
  _milliseconds = 0;
  _microseconds = 0;
  _normalize();
  return *this;
}

// _____________________________________________________________________________________________________________________
bool Time::operator==(const Time &t) const {
  return (getNanoseconds() == t.getNanoseconds());
}

// _____________________________________________________________________________________________________________________
bool Time::operator!=(const Time &t) const {
  return (getNanoseconds() != t.getNanoseconds());
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

// ----- private -------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
void Time::_normalize() {
  _nanoseconds = getNanoseconds();
  _microseconds = _nanoseconds / 1000;
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

// ----- ostream -------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
std::ostream &operator<<(std::ostream &os, const Time &t) {
  os << t.format("auto");
  return os;
}


}  // namespace utils
}  // namespace timed