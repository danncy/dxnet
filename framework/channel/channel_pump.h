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
#ifndef FRAMEWORK_CHANNEL_PUMP_H_
#define FRAMEWORK_CHANNEL_PUMP_H_

namespace framework {

struct ChannelPump {
  ChannelPump();
  virtual ~ChannelPump();

  enum class Mode : int {
    WATCH_READ = 1 << 0,
    WATCH_WRITE = 1 << 1,
    WATCH_READ_WRITE = WATCH_READ | WATCH_WRITE
  };

  struct Observer {
    virtual void OnRead(int fd) = 0;
    virtual void OnWrite(int fd) = 0;
  };

  virtual void Run() = 0;
  virtual bool Watch(int fd, bool persistent, Mode mode, Observer* observer) = 0;
};

}//namespace framework
#endif
