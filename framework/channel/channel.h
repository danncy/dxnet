#ifndef FRAMEWORK_CHANNEL_H_
#define FRAMEWORK_CHANNEL_H_

#include <sys/types.h>
#include <sys/socket.h>

namespace framework {

struct Channel {
  enum class Type : int {
    FIFO,
    MESSAGE_QUEUE,
    SOCK_LOCAL,
    SOCK_INET,
    SOCK_INET6,
  };

  struct Option {
    Channel::Type type_;
    int sock_type_  = SOCK_DGRAM;
    int sock_proto_ = 0;

    Option(Channel::Type type) : type_(type) {}
    Option(Channel::Type type, int stype, int sproto)
      : type_(type),
        sock_type_(stype),
        sock_proto_(sproto) {}
  };

  static Channel* Create(const Option& option);

private:
  Channel(const Option& option);

};

}//namespace framework
#endif
