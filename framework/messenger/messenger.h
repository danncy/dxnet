#ifndef FRAMEWORK_MESSENGER_H_
#define FRAMEWORK_MESSENGER_H_

#include "message.h"

namespace framework {

struct Messenger {

  bool OnRecv(const Message& msg);
  bool OnSend(Message* msg);

};

} // namespace framework
#endif
