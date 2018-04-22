#include "scoped_variable.h"
#include <unistd.h>

namespace framework {

//static
int FileDescriptorRAII::DefaultValue() {
  return -1;
}

//static
void FileDescriptorRAII::Free(int fd) {
  close(fd);
}

}
