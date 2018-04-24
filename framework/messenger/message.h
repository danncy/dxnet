#ifndef FRAMEWORK_MESSAGE_H_ 
#define FRAMEWORK_MESSAGE_H_

#include <cstdint>

namespace framework {

struct Message {

  uint8_t route_id;
  uint8_t type;
  char* data;
  uint32 length;

};
}// namespace framework
#endif
