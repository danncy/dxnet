#include "server.h"
#include "framework/channel/channel_cli.h"

namespace cli {

Server::Server()
  : msg_(std::make_shared<CliMessage>()),
    thread_(new framework::Thread("cli-thread")) {
    framework::Channel::Option opt(framework::Channel::Type::SOCK_INET);
    opt.sock_type = SOCK_STREAM;
    opt.sock_port = 9995;
    opt.delegate  = msg_;
    cli_.reset(new framework::ChannelCli(opt));
  }

Server::~Server() {}

void Server::Run() {
  framework::Thread::Options thread_opt;
  thread_opt.pump = cli_->pump();
  thread_->StartWithOptions(thread_opt);
}

} // namespace cli