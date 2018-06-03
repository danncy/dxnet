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

#ifndef FRAMEWORK_CHANNEL_H_
#define FRAMEWORK_CHANNEL_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <string>

namespace framework {

struct Channel {
  enum class Type : int {
    FIFO,
    MESSAGE_QUEUE,
    SOCK_LOCAL,
    SOCK_INET,
    SOCK_INET6,
  };

  enum class Mode : int {
    SERVER,
    CLIENT,
  };

  struct Option {
    Channel::Type channel_type;
    Channel::Mode mode = Mode::SERVER;
    int sock_type  = SOCK_DGRAM;
    int sock_proto = 0;
    int sock_port  = 8888;
    std::string sock_addr;

    Option(Channel::Type type) : channel_type(type) {}
    Option(Channel::Type type, Channel::Mode cmode)
      : channel_type(type),
        mode(cmode)
    {}
    Option(Channel::Type type,
        Channel::Mode cmode,
        int stype, int sproto, int sport, const std::string& addr)
      : channel_type(type),
        mode(cmode),
        sock_type(stype),
        sock_proto(sproto),
        sock_port(sport),
        sock_addr(addr)
    {}
  };

  Channel(const Option& option);
  virtual ~Channel();

  int ErrorCode() const;
  std::string Error() const;
};

}//namespace framework
#endif
