#ifndef CLI_SERVER_H_
#define CLI_SERVER_H_

#include "framework/channel/channel_cli.h"
#include "interface/cli/cli_message.h"
#include "framework/thread/thread.h"

namespace cli {

// this class implementation of cli server.
// should accept the connection in local or remote
struct Server {

  Server();
  ~Server();

  void Run();

private:
  std::shared_ptr<CliMessage> msg_;
  std::unique_ptr<framework::Thread> thread_;
  std::unique_ptr<framework::ChannelCli> cli_;

};

} // namespace cli

#endif