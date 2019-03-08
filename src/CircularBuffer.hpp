extern "C" {
#include <stdlib.h>
}

namespace microcpp {
namespace ds {
template <typename T, size_t Capacity> class CircularBuffer {
public:
  CircularBuffer() : start_idx(0), buffer_size(0) {}

  size_t size() const {
    return buffer_size;
  }

  size_t capacity() const {
    return Capacity;
  }

  void push_back(const T &val) {
    size_t end_loc = 0;
    end_loc = (start_idx + buffer_size) % Capacity;
    // NOTE It probably makes sense to not set start_idx
    // until the set operation is done in case an interrupt happens
    // here that accesses buffer. I'm not sure of all the implications
    // of that.
    buffer[end_loc] = val;
    // Once the buffer size is equivalent to the capacity,
    // the buffer is at its maximum size.
    if (buffer_size < Capacity) {
      buffer_size++;
    }
    else {
      start_idx = (start_idx + 1) % Capacity;
    }
  }

  T &pop_front() {
    // There is an assumption here that size is checked
    // before anything is returned.
    // NOTE It seems better to decrement start_idx before
    // buffer_size.
    T &val = buffer[start_idx];
    start_idx = (start_idx + 1) % Capacity;
    buffer_size--;
    return val;
  }

private:
  T buffer[Capacity];
  size_t start_idx;
  size_t buffer_size;
};
} // namespace ds
} // namespace microcpp
