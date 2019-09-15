#include "message.h"
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "framework/public/logging.h"
#include "framework/public/format.h"

namespace cli {

Message::Message() {}

Message::~Message() {}

bool Message::OnRecv(const char* msg, int len) {
  assert(msg);

  if (strncmp(msg, "quit", strlen("quit")) == 0) {
    return false;
  }
  std::string m = msg;
  m.pop_back();
  LOG(INFO) << _F("%1") % m;
  queue_.push_back(msg);

  return true;
}

bool Message::OnSend(int fd) {
  if (queue_.empty())
    return false;

  std::string msg = queue_.front();
  int length  = msg.length();
  int times   = 0;
  int written = 0;

  while(written != length) {
    if (times > 5) {
      LOG(WARN) << _F("try times > %1, don't continue.") % times;
      return false;
    }

    int r = write(fd, msg.c_str() + written, length - written);
    times++;

    if (r < 0 && (errno == EINTR || errno == EAGAIN)) {
      continue;
    }
    written += r;
  };

  queue_.pop_front();

  return true;
}

} // namespace cli