#include "lock.h"

namespace framework {

Locker::Locker() {
  pthread_mutexattr_t mta;
  pthread_mutexattr_init(&mta);
  
  pthread_mutex_init(&handle_, &mta);
  pthread_mutexattr_destroy(&mta);
}

Locker::~Locker() {
  pthread_mutex_destroy(&handle_);
}

void Locker::Lock() {
  pthread_mutex_lock(&handle_);
}

void Locker::UnLock() {
  pthread_mutex_unlock(&handle_);
}

bool Locker::Try() {
  int rv = pthread_mutex_trylock(&handle_);
  return rv == 0;
}

}
