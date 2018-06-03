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

}
