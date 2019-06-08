#include <iostream>

#include "CircularBuffer.hpp"

using namespace microcpp::ds;

int main() {

  CircularBuffer<uint32_t, 4> buffer;
  for (uint32_t i = 0; i < 1000; ++i) {
    buffer.push_back(i);
  }

  for (uint32_t i = 0; i < 4; ++i) {
    uint32_t val = buffer.pop_front();
    std::cout << val << "\n";
  }

  return 0;
}
