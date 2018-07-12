#include "framework/public/logging.h"
#include "framework/public/format.h"
#include "framework/thread/thread.h"
#include "framework/main_loop/main_loop.h"
#include <functional>

void log_test() {
  LOG(DEBUG) << "thread testing log.";
  LOG(DEBUG) << _F("thread testing %1 log %2.") % "%2" % "good!";
  LOG(DEBUG) << _Fmt("%1,%2,%3,%1,%4", 1, "test", 9.1, "中国");
  Log::info("%1,%2,%3,%1,%4", 1, "test", 9.1, "中国");
  Log::debug("%1,%2,%3,%1,%4", 1, "test", 9.1, "中国");
  Log::error("%1,%2,%3,%1,%4", 1, "test", 9.1, "中国");
  Log::warn("%1,%2,%3,%1,%4", 1, "test", 9.1, "中国");
  Log::fatal("%1,%2,%3,%1,%4", 1, "test", 9.1, "中国");

}

int main(int argc, char** argv) {
  LOG(INFO) << "test logging\n";
  framework::Thread thread("main_thread");

  if(!thread.Start()) {
    LOG(DEBUG) << "thread start failed!\n";
  } else {
    LOG(DEBUG) << "thread start success!\n";
  }
  thread.main_loop()->PostTask(framework::Task(std::bind(log_test)));
  thread.main_loop()->PostTask(framework::Task(std::bind([](){
    LOG(DEBUG) << "thread testing.\n";
        })));

  framework::MainLoop mainloop;
  mainloop.Run();

  return 0;
}

