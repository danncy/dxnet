#ifndef CLI_COMMAND_H_
#define CLI_COMMAND_H_

#include <string>
#include <vector>
#include <functional>
#include <any>

namespace cli {

struct Argument {

};

const auto ParseInt = [](const std::string& value) { return std::stoi(value); };
const auto ParseFloat = [](const std::string& value) { return std::stof(value); };
const auto ParseDouble = [](const std::string& value) { return std::stod(value); };
//const auto ParseIntArray = [](const std::string& value) { return std::vector<int>();};
//const auto ParseStrArray = [](const std::string& value) { return std::vector<std::string>(); };

// Command cmd("program");
// cmd.Option("help")
//    .Help("print help information.")
//    .Action([](const std::string& value) { return std::stoi(value); });
struct Command {
  Command(const std::string& name /* = ""*/);
  ~Command();

  Command& Option(const std::string& name, const std::string& description, const std::string& value);
  Command& Help(const std::string& description);
  Command& Action(std::function< std::any(const std::string&) > action);

private:
  std::string name_;
  std::vector<Argument> arguments_;
};

struct Program {
  Program(const std::string& name);
  ~Program();

  static Program* instance();

  cli::Command& Command(const std::string& name, const std::string& description /* = "" */);
  Program& Option(const std::string& name, const std::string& description, const std::string& value /*= "" */);
  Program& Action();

  bool Parse(const std::string& args);

private:
  std::string name_;
  std::vector<cli::Command> commands_;
  std::vector<Argument> arguments_;

};

} // namespace cli

#endif