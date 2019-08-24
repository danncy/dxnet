#include "channel_pump_libevent.h"

#include "third_party/libevent/include/event.h"

namespace framework {

ChannelPumpLibevent::ChannelPumpLibevent()
  : evt_base_(event_base_new())
{}

ChannelPumpLibevent::~ChannelPumpLibevent() {
  for (auto& it : evt_map_) {
    event_del(it.second.get());
    it.second.reset();
  }
  evt_map_.clear();

  event_base_loopbreak(evt_base_);
  event_base_free(evt_base_);
}

void ChannelPumpLibevent::Run() {
  event_base_loop(evt_base_, EVLOOP_NO_EXIT_ON_EMPTY);
}

void ChannelPumpLibevent::RunAsync() {
  for(;;) {
    event_base_loop(evt_base_, EVLOOP_NONBLOCK);
  }
}

//static
void ChannelPumpLibevent::OnNotify(int fd, short flags, void* observer) {
  ChannelPump::Observer* obs =
    static_cast<ChannelPump::Observer*>(observer);
  if (!obs)
    return;

  if ((flags & (EV_READ | EV_WRITE)) == (EV_READ | EV_WRITE)) {
    obs->OnWrite(fd);
    obs->OnRead(fd);
  } else if (flags & EV_WRITE) {
    obs->OnWrite(fd);
  } else if (flags & EV_READ) {
    obs->OnRead(fd);
  }
}

bool ChannelPumpLibevent::Watch(int fd,
                                bool persistent,
                                ChannelPump::Mode mode,
                                ChannelPump::Observer* observer) {
  int event_mask = persistent ? EV_PERSIST : 0;
  int event_mode = static_cast<int>(mode);
  if (event_mode & static_cast<int>(ChannelPump::Mode::WATCH_READ))
    event_mask |= EV_READ;
  if (event_mode & static_cast<int>(ChannelPump::Mode::WATCH_WRITE))
    event_mask |= EV_WRITE;

  std::unique_ptr<event> evt(new event);
  event_set(evt.get(), fd, event_mask, OnNotify, observer);

  if (event_base_set(evt_base_, evt.get())) {
    return false;
  }

  if (event_add(evt.get(), nullptr)) {
    return false;
  }

  evt_map_.insert(std::make_pair(fd, std::move(evt)));
  return true;
}

void ChannelPumpLibevent::UnWatch(int fd) {
  auto it = evt_map_.find(fd);
  if (it != evt_map_.end()) {
    event_del(it->second.get());
    it->second.reset();
  }
  evt_map_.erase(it);
}

}
