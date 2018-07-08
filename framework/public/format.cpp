#include "format.h"

namespace framework {

Format::Format(const std::string& fmt)
  : buffer_(fmt),
    index_(0) {
}

void Format::replace_all(const std::string& key, const std::string& val) {
  while (true) {
    int i = buffer_.find(key);
    if (i == std::string::npos)
      break;
    buffer_.replace(i, 2, val.c_str());
  }
}

}

namespace std {

std::ostream& operator << (std::ostream& os, const framework::Format& fmt) {
   return os << fmt.Buffer() << "\n";
}

}


