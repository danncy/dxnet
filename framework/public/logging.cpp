#include "logging.h"

#include <ctime>
#include <iomanip>
#include <sys/time.h>

namespace framework {

Logger::Logger(Level::Severity level) 
  : level_(level),
    file_(__FILE__),
    line_(__LINE__) {
  Init();
}

void Logger::Init() {
  stream_ <<  '[';
  timeval tv;
  gettimeofday(&tv, nullptr);
  time_t t = tv.tv_sec;
  struct tm local_time;
  localtime_r(&t, &local_time);
  struct tm* tm_time = &local_time;
  stream_ << std::setfill('0')
          << std::setw(2) << 1 + tm_time->tm_mon
          << std::setw(2) << tm_time->tm_mday
          << '/'
          << std::setw(2) << tm_time->tm_hour
          << std::setw(2) << tm_time->tm_min
          << std::setw(2) << tm_time->tm_sec
          << '.'
          << std::setw(6) << tv.tv_usec
          << ":" << file_ << "(" << line_ << ")] ";
}

}
