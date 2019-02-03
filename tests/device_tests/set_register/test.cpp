extern "C" {
#include <avr/io.h>
}
#include "Console.hpp"
#include "Register.hpp"

using namespace microcpp;

int main() {

  Register test(TCCR0A);
  test.set(WGM00, WGM01);

  int x = 0;

  // Console console(9600);
  return 0;
}
