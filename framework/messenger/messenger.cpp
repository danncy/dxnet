#include "messenger.h"
#include "framework/channel/channel.h"
#include "framework/channel/channel_pump_libevent.h"
#include "framework/public/rand_util.h"
#include "framework/public/logging.h"

namespace framework {

Messenger::Messenger()
  : pump_(std::make_shared<ChannelPumpLibevent>()),
    worker_thread_(std::string("th.") + GenerateUniqueRandomProcId()) {
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

bool Messenger::WatchFileDescriptor(int fd,
    bool persistent, ChannelPump::Mode mode, ChannelPump::Observer* observer) {
  return pump_->Watch(fd, persistent, mode, observer);
}

void Messenger::PostTask(const Location& location, std::function<void()> func) {
  LOG(INFO) << location.ToString();
  worker_thread_.main_loop()->PostTask(func);
}

} //namespace framework
