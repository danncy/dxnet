#include "cli_message.h"
#include <assert.h>

namespace cli {

CliMessage::CliMessage() {}

CliMessage::~CliMessage() {}

bool CliMessage::OnRecv(const char* msg, int len) {
  assert(msg);

  return true;
}

bool CliMessage::OnSend(const char* msg, int len) {
  assert(msg);

  return true;
}

} // namespace cli