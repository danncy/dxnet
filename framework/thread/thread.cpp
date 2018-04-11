#include "thread.h"

namespace framework {

Thread::Thread(const std::string& name)
  : name_(name),
    main_loop_(new MainLoop()) {}

Thread::~Thread() {
  main_loop_.reset();
}

void Thread::MainRoutine() {
  if (main_loop_)
    main_loop_->Run();
}

bool Thread::Start() {
  Options option;
  return StartWithOptions(option);
}

bool Thread::StartWithOptions(const Options& option) {

  return false;
}

}
