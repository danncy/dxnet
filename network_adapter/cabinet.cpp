#include "cabinet.h"
#include "board.h"
#include <algorithm>

namespace dxnet {

Cabinet::Cabinet(const std::string& name) 
  : name_(name) {}

Cabinet::~Cabinet() {}

void Cabinet::AppendBoard(std::unique_ptr<Board> board) {
  boards_.push_back(std::move(board));
}

void Cabinet::RemoveBoard(int index) {
  if (index <0 || index > BoardNum())
    return;

  auto it = boards_.erase(boards_.begin() + index);
  it->reset();
}

namespace {
  struct BoardCompare {
    BoardCompare(const std::string& name) : name_(name) {}

    bool operator()(const std::unique_ptr<Board>& board) {
      return name_ == board->Name();
    }
  private:
    std::string name_;
  };
}

void Cabinet::RemoveBoard(const std::string& name) {
  auto it = std::find_if(boards_.begin(),
                         boards_.end(),
                         BoardCompare(name));
  if (it != boards_.end()) {
    boards_.erase(it);
    it->reset();
  }
}

}
