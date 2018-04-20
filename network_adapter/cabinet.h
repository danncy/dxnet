#ifndef DXNET_CABINET_H_
#define DXNET_CABINET_H_

#include <string>
#include <vector>
#include <memory>

namespace dxnet {

struct Board;

struct Cabinet {
  Cabinet(const std::string& name);
  ~Cabinet();

  void AppendBoard(std::unique_ptr<Board> board);
  void RemoveBoard(int index);
  void RemoveBoard(const std::string& name);
  
  int BoardNum() { return boards_.size(); }
  std::string Name() { return name_; }

private:
  std::string name_;
  std::vector<std::unique_ptr<Board>> boards_;
};

} //namespace dxnet
#endif
