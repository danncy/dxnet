/* Copyright (C)
 * 2018 - dancy, danncy@21cn.com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef FRAMEWORK_CHANNEL_IPV4_H_
#define FRAMEWORK_CHANNEL_IPV4_H_

#include <string>
#include <memory>

#include "framework/public/scoped_variable.h"
#include "framework/channel/channel.h"
#include "framework/channel/channel_pump_libevent.h"
#include "framework/networking/socket.h"

namespace framework {

struct Messenger;

struct ChannelIPv4 : public Channel,
                     public ChannelPump::Observer {

  ChannelIPv4(const Channel::Option& option);
  ~ChannelIPv4() override;

  void OnRead(int fd) override;
  void OnWrite(int fd) override;
  void AddWatcher(Messenger* messenger) override;
  bool StartWatching();
  bool IsValid() const override { return init_status_; }

private:
  bool Init();

  void TcpAcceptConnection();
  void UdpServerRead();

private:
  Channel::Option option_;
  bool init_status_;

  std::unique_ptr<SocketFileDescriptor> serv_fd_;
  std::unique_ptr<SocketFileDescriptor> clnt_fd_;
  scoped_fd sock_;

  Messenger* messenger_;
};

}//namespace framework
#endif
