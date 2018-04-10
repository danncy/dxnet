#ifndef DXNET_BOARD_H_
#define DXNET_BOARD_H_

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <ostream>

namespace dxnet {

struct Port;

struct Board {
  Board(const std::string& name, const std::string& addr);
  virtual ~Board();

  void appendPort(std::unique_ptr<Port> port);
  void removePort(uint8_t index);
  Port* getPort(uint8_t index);
  int portNum();

  std::string Name() { return name_; }
  std::string Addr() { return addr_; }

  friend std::ostream& operator << (std::ostream& os, const Board& board);
  friend bool operator == (const Board& bda, const Board& bdb);

	private:
		std::string name_;
		std::string addr_;

    std::vector<std::unique_ptr<Port>> ports_;
};

} // namespace dxnet
#endif
