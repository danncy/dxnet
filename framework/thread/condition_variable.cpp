#include "condition_variable.h"

namespace framework {

ConditionVariable::ConditionVariable(Locker* lock)
  : mutex_(lock->Handle()) {
  pthread_condattr_t attrs;
  pthread_condattr_init(&attrs);
  pthread_condattr_setclock(&attrs, CLOCK_MONOTONIC);
  pthread_cond_init(&cond_, &attrs);
  pthread_condattr_destroy(&attrs);
}

ConditionVariable::~ConditionVariable() {
  pthread_cond_destroy(&cond_);
}

void ConditionVariable::Wait() {
  pthread_cond_wait(&cond_, mutex_);
}

void ConditionVariable::TimeWait(const TimeSlice& max_time) {
  int64_t usecs = max_time.InMicroseconds();
  struct timespec relative_time;
  relative_time.tv_sec = usecs / Time::kMicrosecondsPerSecond;
  relative_time.tv_nsec =
      (usecs % Time::kMicrosecondsPerSecond) * Time::kNanosecondsPerMicrosecond;

  struct timespec absolute_time;
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  absolute_time.tv_sec = now.tv_sec;
  absolute_time.tv_nsec = now.tv_nsec;

  absolute_time.tv_sec += relative_time.tv_sec;
  absolute_time.tv_nsec += relative_time.tv_nsec;
  absolute_time.tv_sec += absolute_time.tv_nsec / Time::kNanosecondsPerSecond;
  absolute_time.tv_nsec %= Time::kNanosecondsPerSecond;

  pthread_cond_timedwait(&cond_, mutex_, &absolute_time);
}

void ConditionVariable::Broadcast() {
  pthread_cond_broadcast(&cond_);
}

void ConditionVariable::Signal() {
  pthread_cond_signal(&cond_);
}


}
