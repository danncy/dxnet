#include "format.h"

namespace framework {

Format::Format(const std::string& fmt)
  : buffer_(fmt),
    index_(0) {
}

}

namespace std {

std::ostream& operator << (std::ostream& os, const framework::Format& fmt) {
   return os << fmt.Buffer() << "\n";
}

}


