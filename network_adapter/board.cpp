#include "board.h"

namespace dxnet {

Board::Board(const std::string& name, const std::string& addr)
  : name_(name),
    addr_(addr) {
     
}

Board::~Board() {
  ports_.clear();
}

void Board::appendPort(Port* port) {
  ports_.push_back(port);
}

void Board::removePort(Port* port) {
  for(auto it = ports_.begin(); it != ports_.end();) {
    if (*it == port) {
      it = ports_.erase(it);
      break;
    } else {
      ++it;
    }
  }
}

int Board::portNum() {
  return ports_.size();
}

}
