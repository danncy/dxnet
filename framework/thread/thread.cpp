#include "thread.h"
#include "framework/public/logging.h"
#include "framework/public/format.h"

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
  LOG(INFO) << _F("%1 thread name: %2") % __func__ % Name();
  Options option;
  return StartWithOptions(option);
}

void* ThreadMain(void* arguments) {
  LOG(INFO) << __func__;
  if (!arguments)
    return nullptr;

  Thread *thread = static_cast<Thread*>(arguments);
  thread->Mainloop();

  return nullptr;
}

bool Thread::StartWithOptions(const Options& option) {
  LOG(INFO) << "StartWithOptions\n";
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  bool success = true;

  if (option.detached)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (option.stack_size > 0)
    pthread_attr_setstacksize(&attr, option.stack_size);

  int ret = pthread_create(&thread_id_, &attr, ThreadMain, this);
  LOG(INFO) << ret;
  if (ret) {
    //log pthread_create failed.
    LOG(ERROR) << ret;
    success = false;
  }
  pthread_attr_destroy(&attr);

  return success;
}

}
