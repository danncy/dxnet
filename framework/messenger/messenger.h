#ifndef FRAMEWORK_MESSENGER_H_
#define FRAMEWORK_MESSENGER_H_

#include "message.h"
#include "framework/channel/channel_pump_libevent.h"
#include "framework/thread/thread.h"
#include "framework/public/location.h"

#include <list>
#include <memory>
#include <functional>

namespace framework {

struct Channel;

struct Messenger {
  Messenger();
  ~Messenger() = default;

  struct Delegate {
    Delegate() {}
    virtual ~Delegate() = default;

    virtual bool OnRecv(const Message& msg) {};
    virtual bool OnSend(Message* msg) {};
  };

  bool Watch(Channel* channel, std::shared_ptr<Delegate> delegate);
  bool WatchFileDescriptor(int fd,
    bool persistent, ChannelPump::Mode mode, ChannelPump::Observer* observer);
  void PostTask(const Location& location, std::function<void()> func);

  ChannelPump* pump() {
    return static_cast<ChannelPump*>(&pump_);
  }

  MainLoop* mainloop() const {
    return worker_thread_.main_loop();
  }

private:
  ChannelPumpLibevent pump_;
  Thread worker_thread_;

  std::list<std::shared_ptr<Delegate>> delegate_list_;
};

} // namespace framework
#endif
