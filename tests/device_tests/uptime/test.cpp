#include "Console.hpp"
#include "TickCounter.hpp"

extern "C" {
#include <util/delay.h>
}

using namespace microcpp;

int main() {
  //Console console(9600);
  // TickCounter counter = TickCounter<>::instance();
  while (true) {
  //  console.fmt("uptime is: {}\n", counter.uptime());
    _delay_ms(1000);
  }
  return 0;
}
