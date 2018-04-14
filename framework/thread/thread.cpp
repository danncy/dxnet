#include "thread.h"

namespace framework {

Thread::Thread(const std::string& name)
  : name_(name),
    main_loop_(new MainLoop()),
    thread_id_(0) {}

Thread::~Thread() {
  main_loop_.reset();
}

void Thread::Mainloop() {
  if (main_loop_)
    main_loop_->Run();
}

bool Thread::Start() {
  Options option;
  return StartWithOptions(option);
}

void* ThreadMain(void* arguments) {
  if (!arguments)
    return nullptr;

  Thread *thread = static_cast<Thread*>(arguments);
  thread->Mainloop();

  return nullptr;
}

bool Thread::StartWithOptions(const Options& option) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  bool success = true;

  if (option.detached)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (option.stack_size > 0)
    pthread_attr_setstacksize(&attr, option.stack_size);

  int ret = pthread_create(&thread_id_, &attr, ThreadMain, this);
  if (ret) {
    //log pthread_create failed.
    success = false;
  }
  pthread_attr_destroy(&attr);

  return success;
}

}
