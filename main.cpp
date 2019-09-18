#include "framework/public/logging.h"
#include "framework/public/format.h"
#include "framework/thread/thread.h"
#include "framework/main_loop/main_loop.h"
#include "framework/channel/channel_pump_libevent.h"
#include "interface/cli/server.h"
#include "interface/cli/command.h"
#include <functional>

void LogTest() {
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
  LOG(INFO) << _F("%1") % cli::Argument("--test", "a test option", "just").ToString();

  for (auto& s : framework::split("%1,%2,%3,%1,%4", "%"))
    LOG(DEBUG) << s;

  for (auto& s : framework::split("%1,%2,%3,%1,%4", ','))
    LOG(DEBUG) << s;

  framework::Thread thread("main_thread");

  if(!thread.Start()) {
    LOG(DEBUG) << "thread start failed!\n";
  } else {
    LOG(DEBUG) << "thread start success!\n";
  }
  thread.main_loop()->PostTask(framework::Task(std::bind(LogTest)));
  thread.main_loop()->PostTask(framework::Task(std::bind([](){
    LOG(DEBUG) << "thread testing.\n";
        })));

  cli::Server serv;
  serv.Run();

  framework::MainLoop mainloop;
  mainloop.SetChannelPump(std::make_unique<framework::ChannelPumpLibevent>());
  mainloop.RunWithPump();

  return 0;
}

