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
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <memory>

#include "framework/public/scoped_variable.h"
#include "framework/channel/channel.h"
#include "framework/channel/channel_pump_libevent.h"

namespace framework {

struct Messenger;

struct ChannelIPv4 : public Channel,
                     public ChannelPump::Observer {

  ChannelIPv4(const Channel::Option& option);
  ~ChannelIPv4();

  void OnRead(int fd) override;
  void OnWrite(int fd) override;
  void AddWatcher(Messenger* messenger) override;
  bool Poll();

private:
  bool Init();
  in_addr_t GetInAddr() const {
    return address_.empty() ? INADDR_ANY : inet_addr(address_.c_str());
  }

private:
  Channel::Option option_;
  std::string address_;
  scoped_fd sock_;
  scoped_fd listen_sock_;
  struct sockaddr_in serv_addr_;

  Messenger* messenger_;
  ChannelPumpLibevent pump_;

};

}//namespace framework
#endif
