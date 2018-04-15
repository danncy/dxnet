#ifndef FRAMEWORK_CONDITION_VARIABLE_H_
#define FRAMEWORK_CONDITION_VARIABLE_H_

#include <pthread.h>
#include "framework/public/time.h"
#include "framework/thread/lock.h"

namespace framework {

struct ConditionVariable {

  explicit ConditionVariable(Locker* lock);
  ~ConditionVariable();

  void Wait();
  void TimeWait(const TimeSlice& max_time);
  void Broadcast();
  void Signal();

private:
  pthread_cond_t cond_;
  pthread_mutex_t* mutex_;

};

} //namespace framework
#endif
