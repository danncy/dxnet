#ifndef CLI_MSG_H_
#define CLI_MSG_H_

#include "framework/channel/channel.h"
#include <list>
#include <string>

namespace cli {

struct Message : public framework::Channel::Delegate {

  Message();
  ~Message();

  bool OnRecv(const char* msg, int len) override;
  bool OnSend(int fd) override;

private:
  std::list<std::string> queue_;
};

} // namespace cli

#endif