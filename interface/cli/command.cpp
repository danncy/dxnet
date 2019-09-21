#include "command.h"

#include <iomanip>
#include <sstream>

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
Program& Program::instance(const std::string& name) {
  program_list.emplace_back(std::make_unique<Program>(name));
  return *program_list.back().get();
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

bool Program::Parse(const std::string& args) {

  return true;
}

} // namespace cli