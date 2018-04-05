#include "port.h"

namespace dxnet {

Port::Port(uint8_t index, uint32_t addr)
  : index_(index),
    addr_(addr) {
}

Port::~Port() {}

}
