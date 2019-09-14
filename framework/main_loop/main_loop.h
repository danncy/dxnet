#ifndef FRAMEWORK_MAIN_LOOP_H_
#define FRAMEWORK_MAIN_LOOP_H_

#include "task.h"
#include "framework/public/time.h"
#include "framework/channel/channel_pump.h"

#include <functional>
#include <memory>

namespace framework {

/*
 * MainLoop use to execute Task in the current thread(as pthread.)
 * task is running in the mainloop,
 * with their current process and thread access rights.
 */
struct MainLoop {
  MainLoop();
  ~MainLoop();

  enum State {
    START,PAUSE,STOP
  };

  static MainLoop* current();
  void Run();
  void RunWithPump();

  inline void SetChannelPump(std::shared_ptr<ChannelPump> pump) {
    pump_ = pump;
  }

  inline bool HasPump() const { return pump_.get() != nullptr; }

  void PostTask(Task task);
  void PostTask(std::function<void()> closure);

  void Start();
  void Pause();
  void Stop();
  void Sleep(TimeSlice duration);

private:
  TaskQueue pending_task_queue_;
  State state_;
  std::shared_ptr<ChannelPump> pump_;
};

} // namespace framework
#endif
