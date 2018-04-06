#ifndef FRAMEWORK_LOCATION_H_
#define FRAMEWORK_LOCATION_H_

#include <stddef.h>
#include <cassert>
#include <string>

namespace framework {

struct Location {
  Location();
  Location(const Location& other);
  Location(const char* function_name,
           const char* file_name,
           int line_number);

  bool has_source_info() const { return function_name_ && file_name_; }
  const char* function_name() const { return function_name_; }
  const char* file_name() const { return file_name_; }
  int line_number() const { return line_number_; }

  std::string ToString() const;
                                  
  static Location CreateFromHere(const char* function_name,
                                 const char* file_name,
                                 int line_number);

private:
  const char* function_name_ = nullptr;
  const char* file_name_ = nullptr;
  int line_number_ = -1;
};

#define FROM_HERE FROM_HERE_WITH_EXPLICIT_FUNCTION(__func__)
#define FROM_HERE_WITH_EXPLICIT_FUNCTION(function_name) \
          ::framework::Location::CreateFromHere(function_name, __FILE__, __LINE__)

}  // namespace framework

#endif
