#ifndef FRAMEWORK_CHANNEL_CLI_H_
#define FRAMEWORK_CHANNEL_CLI_H_

#include "framework/public/scoped_variable.h"
#include "framework/channel/channel.h"
#include "framework/channel/channel_pump_libevent.h"
#include "framework/networking/socket.h"

namespace framework {

struct ChannelCli : public Channel,
                    public ChannelPump::Observer {

  ChannelCli(const Channel::Option& option, std::shared_ptr<Channel::Delegate> delegate);
  ~ChannelCli();

  void OnRead(int fd) override;
  void OnWrite(int fd) override;

  ChannelPump* pump() {
    return static_cast<ChannelPump*>(&pump_);
  }

  Channel::Delegate* delegate() {
    return delegate_.get();
  }

  inline bool IsValid() const override { return init_status_; }

private:
  bool Init();
  void Accept();

private:
  Channel::Option option_;
  ChannelPumpLibevent pump_;

  std::unique_ptr<SocketFileDescriptor> serv_fd_;
  std::shared_ptr<Channel::Delegate> delegate_;

  bool init_status_;

};

}//namespace framework

#endif