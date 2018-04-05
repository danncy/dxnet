#ifndef DXNET_BOARD_H_
#define DXNET_BOARD_H_

#include <string>
#include <vector>

namespace dxnet {

struct Port;

struct Board {
  Board(const std::string& name, const std::string& addr);
  virtual ~Board();

	void appendPort(Port* port);
  void removePort(Port* port);
  int portNum();

  std::string Name() { return name_; }
  std::string Addr() { return addr_; }

	private:
		std::string name_;
		std::string addr_;

    std::vector<Port*> ports_;

};

} // namespace dxnet
#endif
