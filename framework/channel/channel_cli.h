#ifndef FRAMEWORK_CHANNEL_CLI_H_
#define FRAMEWORK_CHANNEL_CLI_H_

#include "framework/public/scoped_variable.h"
#include "framework/channel/channel.h"
#include "framework/channel/channel_pump.h"
#include "framework/networking/socket.h"

namespace framework {

struct ChannelCli : public Channel,
                    public ChannelPump::Observer {

  explicit ChannelCli(const Channel::Option& option);
  ~ChannelCli();

  void OnRead(int fd) override;
  void OnWrite(int fd) override;

  void AddWatcher(Messenger* messenger) override {}
  bool StartWatching() override { return false; };

  std::shared_ptr<ChannelPump> pump() override {
    return pump_;
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
  std::shared_ptr<ChannelPump> pump_;

  std::unique_ptr<SocketFileDescriptor> serv_fd_;
  std::shared_ptr<Channel::Delegate> delegate_;

  bool init_status_;

};

}//namespace framework

#endif