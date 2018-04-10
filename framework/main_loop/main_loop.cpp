#include "main_loop.h"
#include <algorithm>

namespace framework {

MainLoop::MainLoop()
  : state_(START) {}

MainLoop::~MainLoop() {}

//static
MainLoop* MainLoop::current() {
  return nullptr;
}

void MainLoop::Run() {
  for (;;) {

    if (!pending_task_queue_.empty() && state_ == START) {
      pending_task_queue_.front().Closure();
      pending_task_queue_.pop();
    }

    if (state_ == STOP)
      break;
  }
}

void MainLoop::PostTask(Task task) {
  pending_task_queue_.push(std::move(task));
}

void MainLoop::Start() {
  state_ = START;
}

void MainLoop::Pause() {
  state_ = PAUSE;
}

void MainLoop::Stop() {
  state_ = STOP;
}

}
