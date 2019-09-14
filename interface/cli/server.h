#ifndef CLI_SERVER_H_
#define CLI_SERVER_H_

#include "framework/channel/channel_cli.h"
#include "interface/cli/cli_message.h"

namespace cli {

// this class implementation of cli server.
// 1. should accept the connection in local or remote
// 2. recv fd via unixsock
struct Server {

  Server();
  ~Server();

  void Run();

private:
  std::shared_ptr<CliMessage> msg_;
  std::unique_ptr<framework::ChannelCli> cli_;
};

} // namespace cli

#endif