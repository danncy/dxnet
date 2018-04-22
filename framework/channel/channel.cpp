#include "channel.h"

namespace framework {

//static
Channel* Channel::Create(const Channel::Option& option) {
  return new Channel(option);
}

Channel::Channel(const Channel::Option& option) {}


}
