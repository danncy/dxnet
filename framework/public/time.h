#ifndef FRAMEWORK_TIME_SLICE_H_
#define FRAMEWORK_TIME_SLICE_H_

#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <sys/time.h>

namespace framework {

struct TimeSlice;
int64_t SaturatedAdd(TimeSlice delta, int64_t value);
int64_t SaturatedSub(TimeSlice delta, int64_t value);

struct TimeSlice {

  constexpr TimeSlice() : timeslice_(0) {}
  constexpr explicit TimeSlice(int64_t delta_us) : timeslice_(delta_us) {}

  static constexpr TimeSlice FromDays(int days);
  static constexpr TimeSlice FromHours(int hours);
  static constexpr TimeSlice FromMinutes(int minutes);
  static constexpr TimeSlice FromSeconds(int64_t secs);
  static constexpr TimeSlice FromMilliseconds(int64_t ms);
  static constexpr TimeSlice FromMicroseconds(int64_t us);
  static constexpr TimeSlice FromNanoseconds(int64_t ns);
  static constexpr TimeSlice FromSecondsD(double secs);
  static constexpr TimeSlice FromMillisecondsD(double ms);
  static constexpr TimeSlice FromMicrosecondsD(double us);
  static constexpr TimeSlice FromNanosecondsD(double ns);
  static constexpr TimeSlice FromDouble(double value);
  static constexpr TimeSlice FromProduct(int64_t value, int64_t positive_value);
  static constexpr TimeSlice Max();
  static constexpr TimeSlice Min();

  constexpr bool is_zero() const { return timeslice_ == 0; }

  constexpr bool is_max() const {
    return timeslice_ == std::numeric_limits<int64_t>::max();
  }

  constexpr bool is_min() const {
    return timeslice_ == std::numeric_limits<int64_t>::min();
  }
  
  int InDays() const;
  int InHours() const;
  int InMinutes() const;
  double InSecondsF() const;
  int64_t InSeconds() const;
  double InMillisecondsF() const;
  int64_t InMilliseconds() const;
  int64_t InMillisecondsRoundedUp() const;
  int64_t InMicroseconds() const;
  int64_t InNanoseconds() const;
  
  constexpr TimeSlice& operator=(TimeSlice other) {
    timeslice_ = other.timeslice_;
    return *this;
  }

  TimeSlice operator+(TimeSlice other) const {
    return TimeSlice(SaturatedAdd(*this, other.timeslice_));
  }
  TimeSlice operator-(TimeSlice other) const {
    return TimeSlice(SaturatedSub(*this, other.timeslice_));
  }

  TimeSlice& operator+=(TimeSlice other) {
    return *this = (*this + other);
  }
  TimeSlice& operator-=(TimeSlice other) {
    return *this = (*this - other);
  }
  constexpr TimeSlice operator-() const { return TimeSlice(-timeslice_); }

  template <typename T>
  TimeSlice operator*(T a) const {
    int64_t result = timeslice_ * a;
    if (std::abs(result) < std::numeric_limits<int64_t>::max())
      return result;

    // Matched sign overflows. Mismatched sign underflows.
    if ((timeslice_ < 0) ^ (a < 0))
      return TimeSlice(std::numeric_limits<int64_t>::min());
    return TimeSlice(std::numeric_limits<int64_t>::max());
  }
  template <typename T>
  constexpr TimeSlice operator/(T a) const {
    if (a == 0)
      return TimeSlice(std::numeric_limits<int64_t>::min());

    int64_t result = timeslice_ / a;
    if (std::abs(result) < std::numeric_limits<int64_t>::max())
      return result;

    // Matched sign overflows. Mismatched sign underflows.
    // Special case to catch divide by zero.
    if ((timeslice_ < 0) ^ (a <= 0))
      return TimeSlice(std::numeric_limits<int64_t>::min());
    return TimeSlice(std::numeric_limits<int64_t>::max());
  }
  template <typename T>
  TimeSlice& operator*=(T a) {
    return *this = (*this * a);
  }
  template <typename T>
  constexpr TimeSlice& operator/=(T a) {
    return *this = (*this / a);
  }

  constexpr int64_t operator/(TimeSlice a) const { return timeslice_ / a.timeslice_; }
  constexpr TimeSlice operator%(TimeSlice a) const {
    return TimeSlice(timeslice_ % a.timeslice_);
  }

  // Comparison operators.
  constexpr bool operator==(TimeSlice other) const {
    return timeslice_ == other.timeslice_;
  }
  constexpr bool operator!=(TimeSlice other) const {
    return timeslice_ != other.timeslice_;
  }
  constexpr bool operator<(TimeSlice other) const {
    return timeslice_ < other.timeslice_;
  }
  constexpr bool operator<=(TimeSlice other) const {
    return timeslice_ <= other.timeslice_;
  }
  constexpr bool operator>(TimeSlice other) const {
    return timeslice_ > other.timeslice_;
  }
  constexpr bool operator>=(TimeSlice other) const {
    return timeslice_ >= other.timeslice_;
  }

  int64_t timeslice_;

};

struct Time {
  constexpr explicit Time(int64_t us) : us_(us) {}
  constexpr Time() : us_(0) {}

  static const int64_t kHoursPerDay = 24;
  static const int64_t kMillisecondsPerSecond = 1000;
  static const int64_t kMillisecondsPerDay =
      kMillisecondsPerSecond * 60 * 60 * kHoursPerDay;
  static const int64_t kMicrosecondsPerMillisecond = 1000;
  static const int64_t kMicrosecondsPerSecond =
      kMicrosecondsPerMillisecond * kMillisecondsPerSecond;
  static const int64_t kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
  static const int64_t kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
  static const int64_t kMicrosecondsPerDay =
      kMicrosecondsPerHour * kHoursPerDay;
  static const int64_t kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
  static const int64_t kNanosecondsPerMicrosecond = 1000;
  static const int64_t kNanosecondsPerSecond =
      kNanosecondsPerMicrosecond * kMicrosecondsPerSecond;
  static constexpr int64_t kTimeTToMicrosecondsOffset =
      INT64_C(11644473600000000);

  
  bool is_null() const {
    return us_ == 0;
  }

  bool is_max() const { return us_ == std::numeric_limits<int64_t>::max(); }
  bool is_min() const { return us_ == std::numeric_limits<int64_t>::min(); }

  static Time Max() {
    return Time(std::numeric_limits<int64_t>::max());
  }

  static Time Min() {
    return Time(std::numeric_limits<int64_t>::min());
  }

  Time& operator=(Time other) {
    us_ = other.us_;
    return *(static_cast<Time*>(this));
  }

  TimeSlice operator-(Time other) const {
    return TimeSlice::FromMicroseconds(us_ - other.us_);
  }

  Time operator+(TimeSlice delta) const {
    return Time(SaturatedAdd(delta, us_));
  }
  Time operator-(TimeSlice delta) const {
    return Time(-SaturatedSub(delta, us_));
  }

  Time& operator+=(TimeSlice delta) {
    return static_cast<Time&>(*this = (*this + delta));
  }
  Time& operator-=(TimeSlice delta) {
    return static_cast<Time&>(*this = (*this - delta));
  }

  bool operator==(Time other) const {
    return us_ == other.us_;
  }
  bool operator!=(Time other) const {
    return us_ != other.us_;
  }
  bool operator<(Time other) const {
    return us_ < other.us_;
  }
  bool operator<=(Time other) const {
    return us_ <= other.us_;
  }
  bool operator>(Time other) const {
    return us_ > other.us_;
  }
  bool operator>=(Time other) const {
    return us_ >= other.us_;
  }

  static Time Now();
  static Time FromTimeT(time_t tt);
  time_t ToTimeT() const;

  static Time FromTimeVal(struct timeval t);
  struct timeval ToTimeVal() const;

private:
  // Time value in a microsecond timebase.
  int64_t us_;
};

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

}//namespace framework
#endif
