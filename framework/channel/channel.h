#ifndef FRAMEWORK_CHANNEL_H_
#define FRAMEWORK_CHANNEL_H_

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
    Channel::Type type;
    int sock_type;
    int sock_proto;
  };

  static Channel* Create(const Option& option);

private:
  Channel(const Option& option);

};

}//namespace framework
#endif
