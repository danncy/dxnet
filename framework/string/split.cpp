#include "split.h"

#include <sstream>

namespace framework {

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> result;
  std::istringstream input(s);

  for (std::string buf; std::getline(input, buf, delimiter); )
    result.emplace_back(buf);

  return result;
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
  std::vector<std::string> result;
  int delim_len = delimiter.length();
  int r = 0, pos = 0;

  while (true) {
    r = s.find(delimiter, pos);
    if (r == std::string::npos) {
      result.emplace_back(s.substr(pos, s.length() - pos));
      break;
    } else {
      if (r > pos) {
        result.emplace_back(s.substr(pos, r - pos));
      }

      pos = r;
      pos += delim_len;
    }
  }

  return result;
}


} // namespace framework