#ifndef FRAMEWORK_THREAD_LOCAL_H_
#define FRAMEWORK_THREAD_LOCAL_H_

#include <pthread.h>

namespace framework {

template<typename T>
struct ThreadLocalPointer {

  explicit ThreadLocalPointer();
  ~ThreadLocalPointer();

  void Set(T* ptr);
  T* Get();

private:
  pthread_key_t key_;

  ThreadLocalPointer(const ThreadLocalPointer<T>&) = delete;
  ThreadLocalPointer<T>& operator=(const ThreadLocalPointer<T>&) = delete;

};

} //namespace framework
#endif
