#include "cli_message.h"
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "framework/public/logging.h"
#include "framework/public/format.h"

namespace cli {

CliMessage::CliMessage() {}

CliMessage::~CliMessage() {}

bool CliMessage::OnRecv(const char* msg, int len) {
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

bool CliMessage::OnSend(int fd) {
  if (queue_.empty())
    return false;

  std::string msg = queue_.front();
  do {
    int r = write(fd, msg.c_str(), msg.length());
    if (r < 0 && (errno == EINTR || errno == EAGAIN)) {
      continue;
    } else {
      break;
    }
  } while(true);

  queue_.pop_front();

  return true;
}

} // namespace cli