#include "framework/public/logging.h"
#include "framework/thread/thread.h"
#include "framework/main_loop/main_loop.h"
#include <functional>

void log() {
  LOG(DEBUG) << "thread testing log.";
}

int main(int argc, char** argv) {
  LOG(INFO) << "test logging\n";
  framework::Thread thread("main_thread");

  if(!thread.Start()) {
    LOG(DEBUG) << "thread start failed!\n";
  } else {
    LOG(DEBUG) << "thread start success!\n";
  }
  thread.main_loop()->PostTask(framework::Task(std::bind(log)));
  thread.main_loop()->PostTask(framework::Task(std::bind([](){
    LOG(DEBUG) << "thread testing.\n";
        })));

  framework::MainLoop mainloop;
  mainloop.Run();

  return 0;
}

