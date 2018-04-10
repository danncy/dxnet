#include "main_loop.h"
#include <algorithm>
#include "framework/thread/thread_local.h"

namespace framework {

namespace {
  ThreadLocalPointer<MainLoop> g_main_loop_tls;
}

MainLoop::MainLoop()
  : state_(START) {
  g_main_loop_tls.Set(this);  
}

MainLoop::~MainLoop() {
  g_main_loop_tls.Set(nullptr);
}

//static
MainLoop* MainLoop::current() {
  return g_main_loop_tls.Get();
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
