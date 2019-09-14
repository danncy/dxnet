#ifndef CLI_MSG_H_
#define CLI_MSG_H_

#include "framework/channel/channel.h"

namespace cli {

struct CliMessage : public framework::Channel::Delegate {

  CliMessage();
  ~CliMessage();

  bool OnRecv(const char* msg, int len) override;
  bool OnSend(const char* msg, int len) override;

};

} // namespace cli

#endif