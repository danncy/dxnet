#ifndef FRAMEWORK_THREAD_LOCAL_H_
#define FRAMEWORK_THREAD_LOCAL_H_

#include <pthread.h>

namespace framework {

template<typename T>
struct ThreadLocalPointer {

  ThreadLocalPointer() {
    pthread_key_create(&key_, nullptr);
  }
  ~ThreadLocalPointer() {
    pthread_key_delete(key_);
  }

  void Set(T* ptr) {
    pthread_setspecific(key_, ptr);
  }
  T* Get() {
    return static_cast<T*>(pthread_getspecific(key_));
  }

private:
  pthread_key_t key_;

  ThreadLocalPointer(const ThreadLocalPointer<T>&) = delete;
  ThreadLocalPointer<T>& operator=(const ThreadLocalPointer<T>&) = delete;

};

} //namespace framework
#endif
