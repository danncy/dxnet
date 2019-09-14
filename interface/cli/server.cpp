#include "server.h"
#include "framework/thread/thread.h"
#include "framework/channel/channel_cli.h"

namespace cli {

Server::Server()
  : msg_(std::make_shared<CliMessage>()) {}

Server::~Server() {}

void Server::Run() {
  framework::Channel::Option opt(framework::Channel::Type::SOCK_INET);
  opt.sock_type = SOCK_STREAM;
  opt.sock_port = 9995;
  opt.delegate  = msg_;
  framework::ChannelCli cli(opt);

  framework::Thread thread("cli-thread");
  framework::Thread::Options thread_opt;
  thread_opt.pump = cli.pump();

  thread.StartWithOptions(thread_opt);
}

} // namespace cli