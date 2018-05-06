#include "channel_pump_libevent.h"
#include <memory>

#include "third_party/libevent/include/event.h"

namespace framework {

ChannelPumpLibevent::ChannelPumpLibevent() {}

ChannelPumpLibevent::~ChannelPumpLibevent() {}

void ChannelPumpLibevent::Run() {}

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
                                int mode,
                                ChannelPump::Observer* observer) {
  int event_mask = persistent ? EV_PERSIST : 0;
  if (mode & static_cast<int>(ChannelPump::Mode::WATCH_READ))
    event_mask |= EV_READ;
  if (mode & static_cast<int>(ChannelPump::Mode::WATCH_WRITE))
    event_mask |= EV_WRITE;

  std::unique_ptr<event> evt(new event);
  event_set(evt.get(), fd, event_mask, OnNotify, observer);

  if (event_base_set(evt_base_, evt.get())) {
    return false;
  }

  if (event_add(evt.get(), nullptr)) {
    return false;
  }
  return true;
}
}
