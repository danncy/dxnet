#include "messenger.h"
#include "framework/channel/channel.h"

namespace framework {

Messenger::Messenger()
  : worker_thread_("worker") {
    worker_thread_.Start();
  }

bool Messenger::Watch(Channel* channel, std::shared_ptr<Delegate> delegate) {
  if (channel && delegate.get()) {
    channel->AddWatcher(this);
    channel->StartWatching();

    // delegate_list_ is for multi customers are intreseting with the channel.
    // TODO
    delegate_list_.push_back(delegate);
    return true;
  }
  return false;
}

} //namespace framework