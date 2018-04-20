#ifndef DXNET_BOARD_H_
#define DXNET_BOARD_H_

#include <string>
#include <vector>
#include <memory>
#include <ostream>

namespace dxnet {

struct Port;

struct Board {
  Board(const std::string& name, const std::string& addr);
  ~Board();

  void AppendPort(std::unique_ptr<Port> port);
  void RemovePort(int index);
  Port* GetPort(int index);
  int PortNum();

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
