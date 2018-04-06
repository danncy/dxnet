#ifndef FRAMEWORK_TASK_H_
#define FRAMEWORK_TASK_H_

#include <functional>
#include <queue>

namespace framework {

struct Task {
  Task(std::function<void()> closure);
  ~Task();

  std::function<void()> Closure;
  //TODO:
  //add timetick to delay task 
};

using TaskQueue = std::queue<Task>;
using DelayedTaskQueue = std::priority_queue<Task>;

}// namespace framework
#endif
