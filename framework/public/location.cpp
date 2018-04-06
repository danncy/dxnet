#include "location.h"

namespace framework {

Location::Location() = default;
Location::Location(const Location& other) = default;
Location::Location(const char* function_name,
                   const char* file_name,
                   int line_number)
     : function_name_(function_name),
       file_name_(file_name),
       line_number_(line_number) {}

std::string Location::ToString() const {
  return std::string(function_name_) + "@" + file_name_ + ":" +
         std::to_string(line_number_);
}

// static
Location Location::CreateFromHere(const char* function_name,
                                  const char* file_name,
                                  int line_number) {
  return Location(function_name, file_name, line_number);
}

}
