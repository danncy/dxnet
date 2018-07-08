#ifndef FRAMEWORK_FORMAT_H_
#define FRAMEWORK_FORMAT_H_
#include <string>
#include <sstream>
#include <unordered_map>

namespace framework {

/*
 * usage:
 *    Format(fmt) % obj1 % obj2 % obj3...
 *    fmt : "%1 %2 %3 ...."
 *
 */
struct Format {

  Format() = default;
  explicit Format(const std::string& fmt);

  template <typename T>
  Format& operator% (const T& obj) {
    index_ = buffer_.find('%', index_);
    if (!IsValidFlag(index_))
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

  template<typename T>
  void DoFmt(const T& v) {
    std::stringstream ss;
    ss << v;
    if (buffer_.empty()) {
      buffer_ = ss.str();
    } else {
      int i = buffer_.find('%');
      if (!IsValidFlag(i))
        return;

      std::string key = buffer_.substr(i, 2);
      if (cache_.find(key) == cache_.end()) {
        cache_[key] = ss.str();
      }
      replace_all(key, cache_[key].c_str());
    }
  }

  template<typename T>
  std::string fmt(const T& v) {
    DoFmt(v);
    return buffer_;
  }

  template<typename T, typename... Args>
  std::string fmt(const T& v, const Args& ...args) {
    DoFmt(v);
    return fmt(args ...);
  }

  void replace_all(const std::string& key, const std::string& val);

private:
  bool IsValidFlag(int index) {
    return index != std::string::npos &&
           index+1 < buffer_.size() &&
           std::isdigit(buffer_[index+1]);
  }

private:
  std::string buffer_;
  int index_;
  std::unordered_map<std::string, std::string> cache_;
};

} //namespace framework

namespace std {
std::ostream& operator << (std::ostream& os, const framework::Format& fmt);
}

#define _F(fmt) ::framework::Format(fmt)
#define _Fmt(str, ...) ::framework::Format().fmt(str, __VA_ARGS__)

#endif
