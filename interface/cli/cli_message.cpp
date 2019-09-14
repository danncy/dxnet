#include "cli_message.h"
#include <string.h>
#include <assert.h>

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

  LOG(INFO) << _F("%1") % msg;

  return true;
}

bool CliMessage::OnSend(const char* msg, int len) {
  assert(msg);

  return true;
}

} // namespace cli