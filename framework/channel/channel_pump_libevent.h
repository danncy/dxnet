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
#ifndef FRAMEWORK_CHANNEL_PUMP_LIBEVENT_H_
#define FRAMEWORK_CHANNEL_PUMP_LIBEVENT_H_

#include "framework/channel/channel_pump.h"

namespace framework {

struct ChannelPumpLibevent : public ChannelPump {
  ChannelPumpLibevent();
  ~ChannelPumpLibevent();

  void Run() override;
};
}//namespace framework

#endif
