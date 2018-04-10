#include "thread_local.h"

namespace framework {

template<typename T>
ThreadLocalPointer<T>::ThreadLocalPointer() {
  pthread_key_create(&key_, nullptr);
}

template<typename T>
ThreadLocalPointer<T>::~ThreadLocalPointer() {
  pthread_key_delete(key_);
}

template<typename T>
void ThreadLocalPointer<T>::Set(T* ptr) {
  pthread_setspecific(key_, ptr);
}

template<typename T>
T* ThreadLocalPointer<T>::Get() {
  return static_cast<T*>(pthread_getspecific(key_));
}

}
