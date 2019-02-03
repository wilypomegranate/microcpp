#include "SString.hpp"
#include "String.hpp"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

using namespace microcpp;

int sum(int a, int b) {
  return a + b;
}

int main() {
  constexpr uint32_t convert = 19083250;
  constexpr uint8_t base = 10;
  constexpr uint16_t num_elements = log_base_int(convert, base);
  // char buffer[num_elements + 1];
  int res = 0;
  for (int i = 0; i < 10; ++i) {
    res += sum(i, i + 1);
  }
  std::cout << res << std::endl;
  char buffer[sum(res, res + 1)];
  for (uint32_t i = 0; i < 100000000; ++i) {
    micro_itoa(convert, buffer, base);
    // itoa(convert, buffer, base);
    // snprintf(buffer, base, "%d", convert);
    // std::to_string(convert);
  }
  std::cout << buffer << "\n";

  constexpr SString<24> ss("foo");
  SString<ss.size()> st = ss;
  st = "foo2";
  std::cout << st.c_str() << "\n";

  return 0;
}
