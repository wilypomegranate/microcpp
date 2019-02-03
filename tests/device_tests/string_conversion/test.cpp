extern "C" {
#include <avr/io.h>
}
#include "String.hpp"

using namespace microcpp;

int main() {
  log_base_int(10000, 10);

  return 0;
}
