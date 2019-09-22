#ifndef CLI_COMMAND_H_
#define CLI_COMMAND_H_

#include <string>
#include <vector>
#include <functional>
#include <any>
#include <memory>

#include "framework/string/split.h"

namespace cli {

struct Argument {
  explicit Argument(const std::string& name)
    : name_(name),
      description_(""),
      action_(nullptr) {}

  Argument(const std::string& name, const std::string& description,
    std::function<std::any(const std::string&)> action = nullptr)
    : name_(name),
      description_(description),
      action_(action) {}

  Argument(const Argument& other) {
    name_ = other.name_;
    description_ = other.description_;
    value_ = other.value_;
    action_ = other.action_;
  }

  Argument(Argument&& other)
    : name_(other.name_),
      description_(other.description_),
      value_(other.value_),
      action_(other.action_) {}

  ~Argument() = default;

  const std::string& Name() const { return name_; }

  Argument& Usage(const std::string& description) {
    description_ = description;
    return *this;
  }

  Argument& Action(std::function<std::any(const std::string&)> action) {
    parsed_value_ = action(this->value_);
    return *this;
  }

  const std::string& Value() const { return value_; }
  void Value(const std::string& value) { value_ = value; }

  std::string ToString();

private:
  std::string name_;
  std::string description_;
  std::string value_;
  std::any parsed_value_;
  std::function<std::any(const std::string&)> action_;
};

const auto ParseInt      = [](const std::string& value) { return std::stoi(value); };
const auto ParseFloat    = [](const std::string& value) { return std::stof(value); };
const auto ParseDouble   = [](const std::string& value) { return std::stod(value); };
const auto ParseIntArray = [](const std::string& value) {
  std::vector<int> v;
  for (auto& s : framework::split(value, ','))
    v.push_back(std::stoi(s));
  return v;
};
const auto ParseStrArray = [](const std::string& value) { return framework::split(value, ','); };

/**
 * Command cmd("program");
 * cmd.Option("help")
 *  .Usage("print help information.")
 *  .Action([](const std::string& value) { return std::stoi(value); });
 */
struct Command {
  Command(const std::string& name);
  ~Command();

  Command& AddOption(const std::string& name, const std::string& description);
  Command& AddOption(const std::string& name, const std::string& description,
    std::function<std::any(const std::string&)> action);

  Command& Help(const std::string& description);
  Argument& Option(const std::string& name);

private:
  std::string name_;
  std::string help_;
  std::vector<Argument> arguments_;
};

/**
 * @brief 定义命令行子程序的命令选项
 *
 * Program prog("ping");
 * prog.AddOption("--count", "ping times", ParseInt)
 *  .AddOption("<address>", "ping target ipv4 address", [](const std::string& value){ return inet_addr(value); });
 *
 * Program::instance("git")
 *  .AddCommand(Command("clone").AddOption("--username", "username").AddOption("--password", "password").Option("<repo>"))
 *  .AddCommand(Command("checkout").Option("<repo>"))
 *  .AddCommand(Command("reset").AddOption("--commit-id", "the commit id", ParseInt));
 */
struct Program {
  Program(const std::string& name);
  ~Program();

  static Program& instance(const std::string& name);
  static std::vector<std::unique_ptr<Program>> program_list;

  Program& AddCommand(Command cmd);
  Program& operator()(Command cmd) {
    commands_.emplace_back(std::move(cmd));
    return *this;
  }

  Program& Option(const std::string& name, const std::string& description,
    std::function< std::any(const std::string&) > action);
  void Usage();

  bool Parse(const std::string& args);

private:
  std::string name_;
  std::vector<cli::Command> commands_;
  std::vector<Argument> arguments_;

};

} // namespace cli

#endif