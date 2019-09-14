#include "channel.h"
#include <cstring>
#include <cerrno>

namespace framework {

Channel::Channel(const Channel::Option& option) {}
Channel::~Channel() {}

int Channel::ErrorCode() const {
  return errno;
}

std::string Channel::Error() const {
  return std::strerror(errno);
}

in_addr_t Channel::GetInAddr(const Channel::Option& option) const {
  return option.sock_addr.empty() ? INADDR_ANY : inet_addr(option.sock_addr.c_str());
}

}
