#include "channel_ipv4.h"
#include <cerrno>
#include <cstring>
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

  if (!Init()) {
    LOG(ERROR) << _F("Channel failed, errno is %1, info: %2") % errno % Error();
  }
}

ChannelIPv4::~ChannelIPv4() {}

bool ChannelIPv4::Init() {
  LOG(INFO) << _F("ipv4 channel init.");
  int fd = socket(AF_INET, option_.sock_type, 0);
  if (-1 == fd)
    return false;
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    return false;

  memset(&serv_addr_, 0, sizeof(serv_addr_));
 
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_addr.s_addr = htonl(GetInAddr());
  serv_addr_.sin_port = htons(option_.sock_port);
 
  if (option_.mode == Channel::Mode::SERVER) {
    listen_sock_.reset(fd);
    
    if (bind(listen_sock_.get(),
             (struct sockaddr*)&serv_addr_,
             sizeof(serv_addr_)) == -1) {
      return false;
    }

    if (listen(listen_sock_.get(), 10) == -1) {
      return false;
    }

    pump_.Watch(listen_sock_.get(),
                false,
                static_cast<int>(ChannelPump::Mode::WATCH_READ),
                static_cast<ChannelPump::Observer*>(this));
    pump_.Run();
  } else if (option_.mode == Channel::Mode::CLIENT) {
    sock_.reset(fd);

    if (connect(sock_.get(),
                (struct sockaddr*)&serv_addr_,
                sizeof(serv_addr_)) == -1) {
      return false;
    }
  }
  return true;
}

void ChannelIPv4::OnRead(int fd) {
  if (fd == listen_sock_.get()) {
    int len = sizeof(serv_addr_);
    int tmp_fd = accept(listen_sock_.get(), (struct sockaddr*)&serv_addr_,
                (socklen_t*)&len);
    if (tmp_fd < 0) {
      LOG(ERROR) << _F("accept new connection failed.");
    } else {
      sock_.reset(tmp_fd);
      LOG(INFO) << _F("accept a connection");
    }
  }
}

void ChannelIPv4::OnWrite(int fd) {

}

}
