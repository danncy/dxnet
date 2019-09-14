#include "channel_ipv4.h"
#include <functional>
#include <cstring>
#include <unistd.h>

#include "framework/public/logging.h"
#include "framework/public/format.h"
#include "framework/messenger/messenger.h"
#include "framework/channel/channel_pump_libevent.h"

namespace framework {

ChannelIPv4::ChannelIPv4(const Channel::Option& option)
  : Channel(option),
    option_(option),
    init_status_(false),
    messenger_(nullptr) {
  if (option_.mode == Channel::Mode::CLIENT && option_.sock_addr.empty())
    option_.sock_addr = "127.0.0.1";

  if (!Init()) {
    LOG(ERROR) << _F("Channel failed, errno is %1, info: %2") % errno % Error();
  }
}

ChannelIPv4::~ChannelIPv4() {}

bool ChannelIPv4::Init() {
  if (option_.mode == Channel::Mode::SERVER) {
    if (option_.sock_type == SOCK_STREAM)
      serv_fd_.reset(new TcpServerFileDescriptor(GetInAddr(option_),
          option_.sock_port));
    else
      serv_fd_.reset(new UdpServerFileDescriptor(GetInAddr(option_),
          option_.sock_port));
    init_status_ = serv_fd_->Init();
  } else if (option_.mode == Channel::Mode::CLIENT) {
    if (option_.sock_type == SOCK_STREAM)
      clnt_fd_.reset(new TcpServerFileDescriptor(GetInAddr(option_),
          option_.sock_port));
    else
      clnt_fd_.reset(new UdpServerFileDescriptor(GetInAddr(option_),
          option_.sock_port));
    init_status_ = clnt_fd_->Init();
  }
  return init_status_;
}

bool ChannelIPv4::StartWatching() {
  if (messenger_) {
    messenger_->PostTask(FROM_HERE,
        std::bind(&ChannelPump::Run, messenger_->pump()));
  } else {
    LOG(ERROR) << _F("Messenger is not initialized for channel.");
    return false;
  }

  if (option_.mode == Channel::Mode::SERVER) {
    if (!init_status_) {
      LOG(ERROR) << _F("channel init socket error in server mode.");
      return false;
    }

    messenger_->WatchFileDescriptor(serv_fd_->GetFileDescriptor(),
                true,
                ChannelPump::Mode::WATCH_READ,
                static_cast<ChannelPump::Observer*>(this));
  } else if (option_.mode == Channel::Mode::CLIENT) {
    if (!init_status_) {
      LOG(ERROR) << _F("channel init socket error in server mode.");
      return false;
    }

    messenger_->WatchFileDescriptor(clnt_fd_->GetFileDescriptor(),
                true,
                ChannelPump::Mode::WATCH_READ_WRITE,
                static_cast<ChannelPump::Observer*>(this));
  }
  return true;
}

void ChannelIPv4::TcpAcceptConnection() {
  LOG(INFO) << _F("enter TcpAcceptConnection");
  int len = sizeof(struct sockaddr_in);
  int tmp_fd = accept(serv_fd_->GetFileDescriptor(),
                      (struct sockaddr*)serv_fd_->GetSockAddr(),
                      (socklen_t*)&len);
  if (tmp_fd < 0) {
    LOG(ERROR) << _F("accept new connection failed.");
  } else {
    if (sock_.IsValid()) {
      LOG(ERROR) << _F("don't support multi client, will todo in future.");
      close(tmp_fd);
      return;
    }
    sock_.reset(tmp_fd);
    LOG(INFO) << _F("accept a connection");
    messenger_->WatchFileDescriptor(sock_.get(),
                true,
                ChannelPump::Mode::WATCH_READ,
                static_cast<ChannelPump::Observer*>(this));
    // TODO:
    // Recv and handle the new socket's data.
  }
}

void ChannelIPv4::UdpServerRead() {
  //TODO
  // recvfrom will using.
}

void ChannelIPv4::OnRead(int fd) {
  if (fd == serv_fd_->GetFileDescriptor()) {
    if (option_.sock_type == SOCK_STREAM)
      TcpAcceptConnection();
    else if (option_.sock_type == SOCK_DGRAM)
      UdpServerRead();
  } else if (fd == sock_.get()) {
    // TODO:
    // Delegate will handle the event.
    char buf[kMaxRecvBufLen] = {0};
    ssize_t n = read(fd, buf, sizeof(buf));
    if (n < 0) {
      LOG(ERROR) << _F("read failed, sockt fd: %1") % fd;
    }

    if (strncmp("quit", buf, strlen("quit")) == 0) {
      sock_.reset();
      messenger_->pump()->UnWatch(fd);
    } else {
      write(fd, buf, strlen(buf));
    }
  } else if (fd == clnt_fd_->GetFileDescriptor()) {
    // TODO:
    // Delegate will handle the event.
  } else {
    LOG(ERROR) << _F("Unknown socket: %1") % fd;
  }
}

void ChannelIPv4::OnWrite(int fd) {
  if (fd == sock_.get()) {
    // TODO
    // Delegate will handle the event.
  } else if (fd == clnt_fd_->GetFileDescriptor()) {
    // TODO
    // Delegate will handle the event.
  }
}

void ChannelIPv4::AddWatcher(Messenger* messenger) {
  if (messenger) {
    messenger_ = messenger;
  }
}

std::shared_ptr<ChannelPump> ChannelIPv4::pump() {
  return messenger_ ? messenger_->pump() : nullptr;
}

}
