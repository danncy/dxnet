#ifndef FRAMEWORK_SCOPED_VARIABLE_H_
#define FRAMEWORK_SCOPED_VARIABLE_H_

namespace framework {

template<typename T, typename RAII>
struct ScopedVariable {
  typedef T value_type;
  typedef RAII raii_type;

  ScopedVariable() : data_(raii_type::DefaultValue()) {}
  explicit ScopedVariable(value_type& value)
    : data_(value) {}
  ScopedVariable(ScopedVariable<T, RAII>&& rv)
    : data_(rv.release()) {}

  bool IsValid() { return data_ != raii_type::DefaultValue(); }
  void reset(const value_type& value = raii_type::DefaultValue()) {
    if (IsValid() && data_ == value)
      return;

    if (IsValid())
      raii_type::Free(data_);
    
    data_ = value;
  }

  const value_type& get() const { return data_; }
  value_type release() {
    value_type old_data_ = data_;
    data_ = raii_type::DefaultValue();
    return old_data_;
  }

  ~ScopedVariable() {
    reset();
  }

  ScopedVariable& operator=(ScopedVariable<T, RAII>&& rv) {
    reset(rv.release());
    return *this;
  }

  bool operator==(const value_type& value) {
    return data_ == value;
  }
  bool operator!=(const value_type& value) {
    return data_ != value;
  }

private:
  T data_;
};

struct FileDescriptorRAII {
  static int DefaultValue();
  static void Free(int fd);
};

typedef ScopedVariable<int, FileDescriptorRAII> ScopedFD;

}//namespace framework

using scoped_fd = ::framework::ScopedFD;

#endif
