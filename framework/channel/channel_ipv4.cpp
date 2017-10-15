#include "channel_ipv4.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "framework/public/logging.h"
#include "framework/public/format.h"

namespace framework {

ChannelIPv4::ChannelIPv4(const Channel::Option& option)
  : Channel(option),
    option_(option),
    address_(option.sock_addr) {
  if (option_.mode == Channel::Mode::CLIENT &&
      address_.empty())
    address_ = "127.0.0.1";

  if (Init()) {
    LOG(ERROR) << _F("Channel created fail, error is %1") % errno;
  }
}

ChannelIPv4::~ChannelIPv4() {}

bool ChannelIPv4::Init() {
  int fd = socket(AF_INET, option_.sock_type, 0);
  if (-1 == fd)
    return false;
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    return false;

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(GetInAddr());
  serv_addr.sin_port = htons(option_.sock_port);
 
  if (option_.mode == Channel::Mode::SERVER) {
    listen_sock_.reset(fd);
    
    if (bind(listen_sock_.get(),
             (struct sockaddr*)&serv_addr,
             sizeof(serv_addr)) == -1) {
      return false;
    }

    if (listen(listen_sock_.get(), 10) == -1) {
      return false;
    }
  } else if (option_.mode == Channel::Mode::CLIENT) {
    sock_.reset(fd);

    if (connect(sock_.get(),
                (struct sockaddr*)&serv_addr,
                sizeof(serv_addr)) == -1) {
      return false;
    }
  }
  return true;
}

}
