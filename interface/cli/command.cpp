#include "command.h"

#include <iomanip>
#include <sstream>
#include <algorithm>

namespace cli {

std::string Argument::ToString() {
  std::stringstream s;
  s << std::left << std::setfill(' ') << "\t"
    << std::setw(12) << name_
    << std::setw(24) << description_
    << std::setw(24) << value_;

  return s.str();
}

Command::Command(const std::string& name)
  : name_(name) {}

Command::~Command() {}

Command& Command::AddOption(const std::string& name, const std::string& description) {
  arguments_.emplace_back(Argument(name, description, nullptr));
  return *this;
}

Command& Command::AddOption(const std::string& name, const std::string& description,
  std::function<std::any(const std::string&)> action) {
  arguments_.emplace_back(Argument(name, description, action));
  return *this;
}

Argument& Command::Option(const std::string& name) {
  return AddOption(name, "").arguments_.back();
}

Command& Command::Help(const std::string& description) {
  return *this;
}

std::vector<std::unique_ptr<Program>> Program::program_list;

// static
Program& Program::New(const std::string& name) {
  Program::program_list.emplace_back(std::make_unique<Program>(name));
  return *Program::program_list.back().get();
}

// static
std::vector<std::unique_ptr<Program>>& Program::All() {
  return Program::program_list;
}

// static
bool Program::Delete(const std::string& name) {
  auto iter = Program::FindIterator(name);
  if (iter != std::end(Program::program_list)) {
    iter->reset();
    Program::program_list.erase(iter);
    return true;
  }

  return false;
}

// static
std::vector<std::unique_ptr<Program>>::iterator Program::FindIterator(const std::string& name) {
  auto iter = std::find_if(std::begin(Program::program_list), std::end(Program::program_list),
    [&](const std::unique_ptr<Program>& elem) {
      return elem->name() == name;
    });
  return iter;
}

// static
void Program::Destroy() {
  for (auto& it : Program::program_list) {
    it.reset();
  }
  Program::program_list.clear();
}

Program::Program(const std::string& name)
  : name_(name) {}

Program::~Program() {}

Program& Program::AddCommand(Command cmd) {
  commands_.emplace_back(std::move(cmd));
  return *this;
}

Program& Program::Option(const std::string& name, const std::string& description,
  std::function<std::any(const std::string&)> action)
{
  arguments_.emplace_back(Argument(name, description, action));
  return *this;
}

void Program::Usage() {

}

std::string Program::Dump() {
  return "";
}

bool Program::Parse(const std::string& args) {

  return true;
}

} // namespace cli