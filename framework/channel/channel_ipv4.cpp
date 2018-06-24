#include "channel_ipv4.h"
#include <functional>
#include "framework/public/logging.h"
#include "framework/public/format.h"
#include "framework/messenger/messenger.h"

namespace framework {

ChannelIPv4::ChannelIPv4(const Channel::Option& option)
  : Channel(option),
    option_(option),
    address_(option.sock_addr),
    init_status_(false),
    server_fd_(GetInAddr(), option.sock_port),
    client_fd_(GetInAddr(), option.sock_port),
    messenger_(nullptr) {
  if (option_.mode == Channel::Mode::CLIENT &&
      address_.empty())
    address_ = "127.0.0.1";

  if (!Init()) {
    LOG(ERROR) << _F("Channel failed, errno is %1, info: %2") % errno % Error();
  }
}

ChannelIPv4::~ChannelIPv4() {}

bool ChannelIPv4::Init() {
  if (option_.mode == Channel::Mode::SERVER) {
    init_status_ = server_fd_.Init();
  } else if (option_.mode == Channel::Mode::CLIENT) {
    init_status_ = client_fd_.Init();
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

    messenger_->WatchFileDescriptor(server_fd_.GetFileDescriptor(),
                false,
                ChannelPump::Mode::WATCH_READ,
                static_cast<ChannelPump::Observer*>(this));
  } else if (option_.mode == Channel::Mode::CLIENT) {
    if (!init_status_) {
      LOG(ERROR) << _F("channel init socket error in server mode.");
      return false;
    }

    messenger_->WatchFileDescriptor(client_fd_.GetFileDescriptor(),
                true,
                ChannelPump::Mode::WATCH_READ_WRITE,
                static_cast<ChannelPump::Observer*>(this));
  }
  return true;
}

void ChannelIPv4::OnRead(int fd) {
  if (fd == server_fd_.GetFileDescriptor()) {
    int len = sizeof(struct sockaddr_in);
    int tmp_fd = accept(server_fd_.GetFileDescriptor(),
                        (struct sockaddr*)server_fd_.GetSockAddr(),
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
  } else if (fd == sock_.get()) {
    // TODO:
    // Delegate will handle the event.
  } else if (fd == client_fd_.GetFileDescriptor()) {
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
  } else if (fd == client_fd_.GetFileDescriptor()) {
    // TODO
    // Delegate will handle the event.
  }
}

void ChannelIPv4::AddWatcher(Messenger* messenger) {
  if (messenger) {
    messenger_ = messenger;
  }
}

}
