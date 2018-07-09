/* Copyright (C)
 * 2018 - dancy, danncy@21cn.com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

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

    virtual bool OnRecv(const Message& msg) { return true; };
    virtual bool OnSend(Message* msg) { return true; };
  };

  bool Watch(Channel* channel, std::shared_ptr<Delegate> delegate);
  bool WatchFileDescriptor(int fd,
    bool persistent, ChannelPump::Mode mode, ChannelPump::Observer* observer);
  void PostTask(const Location& location, std::function<void()> func);

  ChannelPump* pump() {
    return static_cast<ChannelPump*>(&pump_);
  }

private:
  ChannelPumpLibevent pump_;
  Thread worker_thread_;

  std::list<std::shared_ptr<Delegate>> delegate_list_;
};

} // namespace framework
#endif
