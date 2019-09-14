#include "channel_cli.h"

#include "framework/public/logging.h"
#include "framework/public/format.h"
#include "framework/channel/channel_pump_libevent.h"

namespace framework {

ChannelCli::ChannelCli(const Channel::Option& option)
  : Channel(option),
    option_(option),
    pump_(std::make_shared<ChannelPumpLibevent>()),
    delegate_(option.delegate),
    init_status_(false) {
  if (!Init()) {
    LOG(ERROR) << _F("ChannelCli failed, errno is %1, info: %2") % errno % Error();
  }
}

ChannelCli::~ChannelCli() {}

bool ChannelCli::Init() {
  if (option_.mode == Channel::Mode::SERVER) {
    if (option_.sock_type == SOCK_STREAM) {
      serv_fd_.reset(new TcpServerFileDescriptor(GetInAddr(option_),
          option_.sock_port));
    }

    init_status_ = serv_fd_->Init();

    // add serv_fd_ to channel pump then record the fd;
    pump_->Watch(serv_fd_->GetFileDescriptor(),
                true,
                ChannelPump::Mode::WATCH_READ_WRITE,
                static_cast<ChannelPump::Observer*>(this));
    LOG(INFO) << _F("watch the server fd, fd = %1.") % serv_fd_->GetFileDescriptor();
  }

  return init_status_;
}

void ChannelCli::OnRead(int fd) {
  if (fd == serv_fd_->GetFileDescriptor()) {
    Accept();
  } else {
    char buf[2048] = {0};
    ssize_t n = read(fd, buf, sizeof(buf));
    if (n < 0) {
      LOG(ERROR) << _F("read failed, sockt fd: %1") % fd;
    } else if (n == 0) {
      pump_->UnWatch(fd);
      close(fd);
    } else {
      if (delegate_) {
        if (!delegate_->OnRecv(buf, n)) {
          pump_->UnWatch(fd);
          close(fd);
        }
      }
    }
  }
}

void ChannelCli::OnWrite(int fd) {
  if (delegate_)
    delegate_->OnSend(fd);
}

void ChannelCli::Accept() {
  int len = sizeof(struct sockaddr_in);
  int fd = accept(serv_fd_->GetFileDescriptor(),
                      (struct sockaddr*)serv_fd_->GetSockAddr(),
                      (socklen_t*)&len);

  if (fd < 0) {
    LOG(ERROR) << _F("accept new connection failed.");
  } else {
    // add fd to channel pump then record the fd;
    pump_->Watch(fd,
                true,
                ChannelPump::Mode::WATCH_READ_WRITE,
                static_cast<ChannelPump::Observer*>(this));
    LOG(INFO) << _F("a event arrived, fd = %1.") % fd;
  }
}

} // namespace framework