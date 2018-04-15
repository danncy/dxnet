#ifndef FRAMEWORK_MAIN_LOOP_H_
#define FRAMEWORK_MAIN_LOOP_H_

#include "task.h"
#include "framework/public/time.h"

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
  void PostTask(Task task);

  void Start();
  void Pause();
  void Stop();
  void Sleep(TimeSlice duration);

private:
  TaskQueue pending_task_queue_;
  State state_;
};

} // namespace framework
#endif
