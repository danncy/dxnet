#ifndef FRAMEWORK_LOGGING_H_
#define FRAMEWORK_LOGGING_H_

#include <string>
#include <sstream>
#include <iostream>

namespace framework {

struct Logger {
  struct Level {
    enum class Severity : int {
      DEBUG,INFO,WARN,ERROR,FATAL,
    };
  };

  Logger(Level::Severity level, const char* file, int line);

  template<typename T>
  Logger& operator << (const T& info) {
    stream_ << info;
    std::cout << stream_.str();
    return *this;
  }

  std::string str() {
    return stream_.str();
  }
private:
  void Init();

private:
  Level::Severity level_ = Level::Severity::DEBUG;
  std::string file_;
  int line_;
  std::ostringstream stream_;
};
} // namespace framework

#define LOG(level) \
  ::framework::Logger(level, __FILE__, __LINE__)
using LEVEL = ::framework::Logger::Level::Severity;

const LEVEL DEBUG = ::framework::Logger::Level::Severity::DEBUG;
const LEVEL INFO  = ::framework::Logger::Level::Severity::INFO;
const LEVEL WARN  = ::framework::Logger::Level::Severity::WARN;
const LEVEL ERROR = ::framework::Logger::Level::Severity::ERROR;
const LEVEL FATAL = ::framework::Logger::Level::Severity::FATAL;

#endif
