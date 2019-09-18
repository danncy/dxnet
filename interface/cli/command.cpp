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

} // namespace cli