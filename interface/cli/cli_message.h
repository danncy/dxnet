#ifndef CLI_MSG_H_
#define CLI_MSG_H_

#include "framework/channel/channel.h"
#include <list>
#include <string>

namespace cli {

struct CliMessage : public framework::Channel::Delegate {

  CliMessage();
  ~CliMessage();

  bool OnRecv(const char* msg, int len) override;
  bool OnSend(int fd) override;

private:
  std::list<std::string> queue_;
};

} // namespace cli

#endif