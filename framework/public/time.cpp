#include "time.h"
#include <sys/time.h>

namespace framework {

// static
constexpr TimeSlice TimeSlice::FromDays(int days) {
  return days == std::numeric_limits<int>::max()
             ? Max()
             : TimeSlice(days * Time::kMicrosecondsPerDay);
}

// static
constexpr TimeSlice TimeSlice::FromHours(int hours) {
  return hours == std::numeric_limits<int>::max()
             ? Max()
             : TimeSlice(hours * Time::kMicrosecondsPerHour);
}

// static
constexpr TimeSlice TimeSlice::FromMinutes(int minutes) {
  return minutes == std::numeric_limits<int>::max()
             ? Max()
             : TimeSlice(minutes * Time::kMicrosecondsPerMinute);
}

// static
constexpr TimeSlice TimeSlice::FromSeconds(int64_t secs) {
  return FromProduct(secs, Time::kMicrosecondsPerSecond);
}

// static
constexpr TimeSlice TimeSlice::FromMilliseconds(int64_t ms) {
  return FromProduct(ms, Time::kMicrosecondsPerMillisecond);
}

// static
constexpr TimeSlice TimeSlice::FromMicroseconds(int64_t us) {
  return TimeSlice(us);
}

// static
constexpr TimeSlice TimeSlice::FromNanoseconds(int64_t ns) {
  return TimeSlice(ns / Time::kNanosecondsPerMicrosecond);
}

// static
constexpr TimeSlice TimeSlice::FromSecondsD(double secs) {
  return FromDouble(secs * Time::kMicrosecondsPerSecond);
}

// static
constexpr TimeSlice TimeSlice::FromMillisecondsD(double ms) {
  return FromDouble(ms * Time::kMicrosecondsPerMillisecond);
}

// static
constexpr TimeSlice TimeSlice::FromMicrosecondsD(double us) {
  return FromDouble(us);
}

// static
constexpr TimeSlice TimeSlice::FromNanosecondsD(double ns) {
  return FromDouble(ns / Time::kNanosecondsPerMicrosecond);
}

// static
constexpr TimeSlice TimeSlice::Max() {
  return TimeSlice(std::numeric_limits<int64_t>::max());
}

// static
constexpr TimeSlice TimeSlice::Min() {
  return TimeSlice(std::numeric_limits<int64_t>::min());
}

// static
constexpr TimeSlice TimeSlice::FromDouble(double value) {
  return value > std::numeric_limits<int64_t>::max()
             ? Max()
             : value < std::numeric_limits<int64_t>::min()
                   ? Min()
                   : TimeSlice(static_cast<int64_t>(value));
}

// static
constexpr TimeSlice TimeSlice::FromProduct(int64_t value,
                                           int64_t positive_value) {
  return value > std::numeric_limits<int64_t>::max() / positive_value
             ? Max()
             : value < std::numeric_limits<int64_t>::min() / positive_value
                   ? Min()
                   : TimeSlice(value * positive_value);
}

int TimeSlice::InDays() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(timeslice_ / Time::kMicrosecondsPerDay);
}

int TimeSlice::InHours() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(timeslice_ / Time::kMicrosecondsPerHour);
}

int TimeSlice::InMinutes() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int>::max();
  }
  return static_cast<int>(timeslice_ / Time::kMicrosecondsPerMinute);
}

double TimeSlice::InSecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(timeslice_) / Time::kMicrosecondsPerSecond;
}

int64_t TimeSlice::InSeconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return timeslice_ / Time::kMicrosecondsPerSecond;
}

double TimeSlice::InMillisecondsF() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<double>::infinity();
  }
  return static_cast<double>(timeslice_) / Time::kMicrosecondsPerMillisecond;
}

int64_t TimeSlice::InMilliseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return timeslice_ / Time::kMicrosecondsPerMillisecond;
}

int64_t TimeSlice::InMillisecondsRoundedUp() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return (timeslice_ + Time::kMicrosecondsPerMillisecond - 1) /
      Time::kMicrosecondsPerMillisecond;
}

int64_t TimeSlice::InMicroseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return timeslice_;
}

int64_t TimeSlice::InNanoseconds() const {
  if (is_max()) {
    // Preserve max to prevent overflow.
    return std::numeric_limits<int64_t>::max();
  }
  return timeslice_ * Time::kNanosecondsPerMicrosecond;
}

////////////////////////////////////////////////////////////////////////////////

//static
Time Time::Now() {
  struct timeval tv;
  struct timezone tz = {0, 0};  // UTC
  gettimeofday(&tv, &tz);
  
  return Time() + TimeSlice::FromMicroseconds(
      (tv.tv_sec * Time::kMicrosecondsPerSecond + tv.tv_usec) +
      Time::kTimeTToMicrosecondsOffset);
}

// static
Time Time::FromTimeT(time_t tt) {
  if (tt == 0)
    return Time();
  if (tt == std::numeric_limits<time_t>::max())
    return Max();
  return Time(kTimeTToMicrosecondsOffset) + TimeSlice::FromSeconds(tt);
}

time_t Time::ToTimeT() const {
  if (is_null())
    return 0;
  if (is_max()) {
    return std::numeric_limits<time_t>::max();
  }
  if (std::numeric_limits<int64_t>::max() - kTimeTToMicrosecondsOffset <= us_) {
    return std::numeric_limits<time_t>::max();
  }
  return (us_ - kTimeTToMicrosecondsOffset) / kMicrosecondsPerSecond;
}

//static
Time Time::FromTimeVal(struct timeval t) {
  if (t.tv_usec == 0 && t.tv_sec == 0)
    return Time();
  if (t.tv_usec == static_cast<suseconds_t>(Time::kMicrosecondsPerSecond) - 1 &&
      t.tv_sec == std::numeric_limits<time_t>::max())
    return Max();
  return Time((static_cast<int64_t>(t.tv_sec) * Time::kMicrosecondsPerSecond) +
                              t.tv_usec + kTimeTToMicrosecondsOffset);
}

struct timeval Time::ToTimeVal() const {
  struct timeval result;
  if (is_null()) {
    result.tv_sec = 0;
    result.tv_usec = 0;
    return result;
  }
  if (is_max()) {
    result.tv_sec = std::numeric_limits<time_t>::max();
    result.tv_usec = static_cast<suseconds_t>(Time::kMicrosecondsPerSecond) - 1;
    return result;
  }
  int64_t us = us_ - kTimeTToMicrosecondsOffset;
  result.tv_sec = us / Time::kMicrosecondsPerSecond;
  result.tv_usec = us % Time::kMicrosecondsPerSecond;
  return result;
}










}
