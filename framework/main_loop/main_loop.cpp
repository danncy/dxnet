#include "main_loop.h"

namespace framework {

MainLoop::MainLoop() {
}

MainLoop::~MainLoop() {}

//static
MainLoop* MainLoop::current() {
  return nullptr;
}

void MainLoop::Run() {
  while(!pending_task_queue_.empty()) {
    pending_task_queue_.front().Closure();
    pending_task_queue_.pop();
  }
}

bool MainLoop::Start() {
  return true;
}

bool MainLoop::Pause() {
  return true;
}

bool MainLoop::Stop() {
  return true;
}

}
