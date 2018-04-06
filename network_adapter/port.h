#ifndef DXNET_PORT_H_
#define DXNET_PORT_H_

#include <cstdint>
#include <string>

namespace dxnet {

struct Port {
  Port(uint8_t index, uint32_t addr);
  virtual ~Port();

  uint8_t Index() { return index_; }
  uint32_t Addr() { return addr_; }

private:
  uint8_t index_;
  uint32_t addr_;

};

} // namespace dxnet
#endif
