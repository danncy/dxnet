#include "rand_util.h"
#include "framework/public/format.h"
#include "unistd.h"
#include <limits>

namespace SeqNum {
  std::atomic_int uuid{0};
  int GetNext() {
    return uuid.fetch_add(1, std::memory_order_relaxed);
  }
} // namespace SeqNum

namespace framework {

std::string GenerateUniqueRandomProcId() {
  Format fmt("%1.%2.%3");
  fmt % getpid() % SeqNum::GetNext() % GenerateRandomNumber();

  return fmt.Buffer();
}

int GenerateRandomNumber() {
  std::random_device rdev;
  std::default_random_engine e(rdev());
  std::uniform_int_distribution<int> uniform_dist(1,
    std::numeric_limits<int>::max());

  return uniform_dist(e);
}

}