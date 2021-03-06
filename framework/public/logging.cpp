#include "logging.h"

#include <ctime>
#include <iomanip>
#include <libgen.h>

namespace framework {

Logger::Targets Logger::target = Logger::Targets::CONSOLE;

Logger::Logger(Level::Severity level,
               const char* file,
               int line)
  : level_(level),
    file_(file),
    line_(line) {
  Init();
}

void Logger::Init() {
  std::time_t t = std::time(nullptr);
  stream_ << "["
          << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
          << "]"
          << LevelToString(level_)
          <<"[" << basename(const_cast<char*>(file_.c_str()))
          << "(" << line_ << ")]:";
}

std::string Logger::LevelToString(Level::Severity level) {
  switch (level) {
    case Level::Severity::DEBUG:
      return "[DBG]";
    case Level::Severity::INFO:
      return "[INF]";
    case Level::Severity::WARN:
      return "[WRN]";
    case Level::Severity::ERROR:
      return "[ERR]";
    case Level::Severity::FATAL:
      return "[FTL]";
    default:
      break;
  }

  return " ";
}

}
