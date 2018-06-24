#include "socket.h"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include "framework/public/logging.h"
#include "framework/public/format.h"

namespace framework {

namespace {
  int BuildSocketFD(int family,
                    int type,
                    in_addr_t addr,
                    int port,
                    struct sockaddr_in* serv_addr) {
    if (!serv_addr)
      return -1;

    scoped_fd fd;
    fd.reset(socket(family, type, 0));
    if (!fd.IsValid())
      return -1;

    if (fcntl(fd.get(), F_SETFL, O_NONBLOCK) < 0)
      return -1;

    memset(serv_addr, 0, sizeof(*serv_addr));
    serv_addr->sin_family = family;
    serv_addr->sin_addr.s_addr = htonl(addr);
    serv_addr->sin_port = htons(port);

    return fd.release();
  }
}

SocketFileDescriptor::SocketFileDescriptor(in_addr_t addr, int port)
 : addr_(addr), port_(port) {}

bool SocketFileDescriptor::Init() {
  if (!DoInit()) {
    LOG(ERROR) << _F("socket error, errno = %1, info: %2") 
      % errno % std::strerror(errno);
    return false;
  }
  return true;
}

TcpServerFileDescriptor::TcpServerFileDescriptor(in_addr_t addr, int port)
 : SocketFileDescriptor(addr, port) {}

bool TcpServerFileDescriptor::DoInit() {
  fd_.reset(BuildSocketFD(AF_INET, SOCK_STREAM, addr_, port_, &serv_addr_));
  if (!fd_.IsValid())
    return false;

  if (bind(fd_.get(),
           (struct sockaddr*)&serv_addr_,
           sizeof(serv_addr_)) == -1)
    return false;
  
  if (listen(fd_.get(), kSocketMaxConn) == -1)
    return false;

  LOG(INFO) << _F("Tcp Server is initialized at %1:%2") % inet_ntoa(serv_addr_.sin_addr) % port_;
  return true;
}

TcpClientFileDescriptor::TcpClientFileDescriptor(in_addr_t addr, int port)
 : SocketFileDescriptor(addr, port) {}

bool TcpClientFileDescriptor::DoInit() {
  fd_.reset(BuildSocketFD(AF_INET, SOCK_STREAM, addr_, port_, &serv_addr_));
  if (!fd_.IsValid())
    return false;

  if (connect(fd_.get(),
              (struct sockaddr*)&serv_addr_,
              sizeof(serv_addr_)) == -1) {
    return false;
  }

  LOG(INFO) << _F("Tcp Client is connected to %1:%2") % inet_ntoa(serv_addr_.sin_addr) % port_;
  return true;
}

UdpServerFileDescriptor::UdpServerFileDescriptor(in_addr_t addr, int port)
  : SocketFileDescriptor(addr, port) {}

bool UdpServerFileDescriptor::DoInit() {
  fd_.reset(BuildSocketFD(AF_INET, SOCK_DGRAM, addr_, port_, &serv_addr_));
  if (!fd_.IsValid())
    return false;

  if (bind(fd_.get(),
           (struct sockaddr*)&serv_addr_,
           sizeof(serv_addr_)) == -1)
    return false;

  LOG(INFO) << _F("Udp Server is initialized at %1:%2") % inet_ntoa(serv_addr_.sin_addr) % port_;
  return true;
}

UdpClientFileDescriptor::UdpClientFileDescriptor(in_addr_t addr, int port)
  : SocketFileDescriptor(addr, port) {}

bool UdpClientFileDescriptor::DoInit() {
  fd_.reset(BuildSocketFD(AF_INET, SOCK_DGRAM, addr_, port_, &serv_addr_));
  if (!fd_.IsValid())
    return false;

  return true;
}

}