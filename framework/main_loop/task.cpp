#include "task.h"

namespace framework {

Task::Task(std::function<void()> closure)
  : Closure(closure)
{}

Task::~Task() {}


}
