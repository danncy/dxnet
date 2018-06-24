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
#ifndef FRAMEWORK_SOCKET_H_
#define FRAMEWORK_SOCKET_H_
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "framework/public/scoped_variable.h"

namespace framework {

static const size_t kMaxSocketNameLength = 104;
static const int kSocketMaxConn = 128;

// build socket.
struct SocketFileDescriptor {
  SocketFileDescriptor(in_addr_t addr, int port);
  virtual ~SocketFileDescriptor() = default;
  virtual bool Init();
  int GetFileDescriptor() const {
    return fd_.get();
  }

  struct sockaddr_in* GetSockAddr() {
    return &serv_addr_;
  }

protected:
  virtual bool DoInit() = 0;

protected:
  scoped_fd fd_;
  in_addr_t addr_;
  int port_;
  struct sockaddr_in serv_addr_;
};

struct TcpServerFileDescriptor : public SocketFileDescriptor {
  TcpServerFileDescriptor(in_addr_t addr, int port);
  ~TcpServerFileDescriptor() = default;

protected:
  bool DoInit() override;

};

struct TcpClientFileDescriptor : public SocketFileDescriptor {
  TcpClientFileDescriptor(in_addr_t addr, int port);
  ~TcpClientFileDescriptor() = default;

protected:
  bool DoInit() override;
};

struct UdpServerFileDescriptor : public SocketFileDescriptor {
  UdpServerFileDescriptor(in_addr_t addr, int port);
  ~UdpServerFileDescriptor() = default;

protected:
  bool DoInit() override;
};

struct UdpClientFileDescriptor : public SocketFileDescriptor {
  UdpClientFileDescriptor(in_addr_t addr, int port);
  ~UdpClientFileDescriptor() = default;

protected:
  bool DoInit() override;
};

}

#endif
