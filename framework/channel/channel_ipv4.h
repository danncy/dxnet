#ifndef FRAMEWORK_CHANNEL_IPV4_H_
#define FRAMEWORK_CHANNEL_IPV4_H_
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include "framework/public/scoped_variable.h"
#include "framework/channel/channel.h"

namespace framework {

struct ChannelIPv4 : public Channel {

  ChannelIPv4(const Channel::Option& option);
  ~ChannelIPv4();

private:
  bool Init();
  in_addr_t GetInAddr() const {
    return address_.empty() ? INADDR_ANY : inet_addr(address_.c_str());
  }

private:
  Channel::Option option_;
  std::string address_;
  scoped_fd sock_;
  scoped_fd listen_sock_;

};

}//namespace framework
#endif
