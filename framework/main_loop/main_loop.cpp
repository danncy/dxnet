#include "main_loop.h"
#include <algorithm>
#include <time.h>
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

    if (pending_task_queue_.empty()) {
      Sleep(TimeSlice::FromMilliseconds(10));
    }
  }
}

void MainLoop::RunWith(std::unique_ptr<ChannelPump> pump = nullptr) {
  if (pump) {
    pump_.reset(pump.release());
  }
  
  if (pump_) {
    pump_->Run();
  }
}

void MainLoop::PostTask(Task task) {
  pending_task_queue_.push(std::move(task));
}

void MainLoop::PostTask(std::function<void()> closure) {
  pending_task_queue_.push(std::move(Task(closure)));
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

void MainLoop::Sleep(TimeSlice duration) {
  struct timespec sleep_time, rest_time;

  sleep_time.tv_sec = duration.InSeconds();
  duration -= TimeSlice::FromSeconds(sleep_time.tv_sec);
  sleep_time.tv_nsec = duration.InMicroseconds() * 1000;

  while (nanosleep(&sleep_time, &rest_time) == -1 && errno == EINTR)
    sleep_time = rest_time;
}

}
