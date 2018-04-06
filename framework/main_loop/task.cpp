#include "task.h"

namespace framework {

Task::Task(std::function<void()> closure)
  : closure_(closure)
{}

Task::~Task() {}


}
