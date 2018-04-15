#ifndef FRAMEWORK_FORMAT_H_
#define FRAMEWORK_FORMAT_H_
#include <string>
#include <sstream>

namespace framework {

/*
 * usage:
 *    Format(fmt) % obj1 % obj2 % obj3...
 *    fmt : "%1 %2 %3 ...."
 *
 */
struct Format {

  Format(const std::string& fmt);

  template <typename T>
  Format& operator% (const T& obj) {
    index_ = buffer_.find('%', index_);
    if (index_ == std::string::npos)
      return *this;
    if (index_+1 >= buffer_.size())
      return *this;
    if (!std::isdigit(buffer_[index_+1]))
      return *this;
    
    std::stringstream format;
    format << obj;
    buffer_.replace(index_, 2, format.str().c_str());
    index_ += format.str().size();

    return *this;
  }

  std::string Buffer() const {
    return buffer_;
  }

private:
  std::string buffer_;
  int index_;
};

} //namespace framework

namespace std {
std::ostream& operator << (std::ostream& os, const framework::Format& fmt);
}

#define _F(fmt) ::framework::Format(fmt)

#endif
