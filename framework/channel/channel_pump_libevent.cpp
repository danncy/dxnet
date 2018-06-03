#include "channel_pump_libevent.h"

#include "third_party/libevent/include/event.h"

namespace framework {

ChannelPumpLibevent::ChannelPumpLibevent()
  : evt_base_(event_base_new()),
    evt_(nullptr)
{}

ChannelPumpLibevent::~ChannelPumpLibevent() {
  if (evt_) {
    event_del(evt_.get());
    evt_.reset();
  }
  event_base_loopbreak(evt_base_);
  event_base_free(evt_base_);
}

void ChannelPumpLibevent::Run() {
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
                                int mode,
                                ChannelPump::Observer* observer) {
  int event_mask = persistent ? EV_PERSIST : 0;
  if (mode & static_cast<int>(ChannelPump::Mode::WATCH_READ))
    event_mask |= EV_READ;
  if (mode & static_cast<int>(ChannelPump::Mode::WATCH_WRITE))
    event_mask |= EV_WRITE;

  evt_.reset(new event);
  event_set(evt_.get(), fd, event_mask, OnNotify, observer);

  if (event_base_set(evt_base_, evt_.get())) {
    return false;
  }

  if (event_add(evt_.get(), nullptr)) {
    return false;
  }
  return true;
}
}
