#include <iostream>
#include "framework/channel/channel_ipv4.h"
#include "framework/main_loop/main_loop.h"
#include "framework/public/format.h"
#include "framework/public/logging.h"

int main(int argc, char** argv) {
  LOG(INFO) << _F("adapter init!");

  framework::Channel::Option opt(framework::Channel::Type::SOCK_INET);
  opt.sock_type = SOCK_STREAM;
  opt.sock_port = 8000;
  framework::ChannelIPv4 chn(opt);

  framework::MainLoop mainloop;
  mainloop.Run();

  return 0;
}
