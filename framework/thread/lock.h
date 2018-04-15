#ifndef FRAMEWORK_LOCK_H_
#define FRAMEWORK_LOCK_H_

#include <pthread.h>
#include <errno.h>

namespace framework {

struct Locker {

  Locker();
  ~Locker();

  void Lock();
  void UnLock();
  bool Try();

  pthread_mutex_t* Handle() {
    return &handle_;
  }

private:
  pthread_mutex_t handle_;

  Locker(const Locker&) = delete;
  Locker& operator=(const Locker&) = delete;
};

class AutoLock {
public:
  explicit AutoLock(Locker& lock) : lock_(lock) {
    lock_.Lock();
  }

   ~AutoLock() {
    lock_.UnLock();
  }

private:
  Locker& lock_;

  AutoLock(const AutoLock&) = delete;
  AutoLock& operator=(const AutoLock&) = delete;
};

} // namespace framework
#endif
