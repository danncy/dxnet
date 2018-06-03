#include <iostream>
#include "framework/channel/channel_ipv4.h"
#include "framework/main_loop/main_loop.h"

int main(int argc, char** argv) {
  std::cout << "adapter init!" << std::endl;

  framework::Channel::Option opt(framework::Channel::Type::SOCK_INET);
  opt.sock_type = SOCK_STREAM;
  opt.sock_port = 8000;
  framework::ChannelIPv4 chn(opt);

  framework::MainLoop mainloop;
  mainloop.Run();

  return 0;
}
