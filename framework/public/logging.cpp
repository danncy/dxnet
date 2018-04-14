#include "logging.h"

#include <ctime>
#include <iomanip>

namespace framework {

Logger::Logger(Level::Severity level,
               const char* file,
               int line) 
  : level_(level),
    file_(file),
    line_(line) {
  Init();
}

void Logger::Init() {
  stream_ <<  '[';
  std::time_t t = std::time(nullptr);
  stream_ << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
          << "][" << file_ << "(" << line_ << ")]"
          << LevelToString(level_);
}

std::string Logger::LevelToString(Level::Severity level) {
  switch (level) {
    case Level::Severity::DEBUG:
      return "[DEBUG] ";
    case Level::Severity::INFO:
      return "[INFO] ";
    case Level::Severity::WARN:
      return "[WARN] ";
    case Level::Severity::ERROR:
      return "[ERROR] ";
    case Level::Severity::FATAL:
      return "[FATAL] ";
    default:
      break;
  }

  return " ";
}

}
