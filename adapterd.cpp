#include <iostream>
#include <memory>

#include "framework/channel/channel_ipv4.h"
#include "framework/main_loop/main_loop.h"
#include "framework/public/format.h"
#include "framework/public/logging.h"
#include "framework/messenger/messenger.h"

int main(int argc, char** argv) {
  LOG(INFO) << _F("adapter init!");

  framework::Channel::Option opt(framework::Channel::Type::SOCK_INET);
  opt.sock_type = SOCK_STREAM;
  opt.sock_port = 8000;
  framework::ChannelIPv4 chn(opt);

  framework::Messenger messenger;
  using MessengerDelegate = framework::Messenger::Delegate;
  std::shared_ptr<MessengerDelegate> delegate =
    std::make_shared<MessengerDelegate>();

  messenger.Watch(&chn, delegate);
  chn.Poll();

  framework::MainLoop mainloop;
  mainloop.Run();

  return 0;
}
