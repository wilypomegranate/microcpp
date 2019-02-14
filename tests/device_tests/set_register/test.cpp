extern "C" {
#include <avr/io.h>
}
#include "Console.hpp"
#include "Register.hpp"

using namespace microcpp;

int main() {

  Console console(9600);
  Register test(TCCR0A);
  test.set_bits(WGM00, WGM01);

  console.fmt("WGM00: {}\tWGM01: {}\tTCCR0A: {}\n", WGM00, WGM01, TCCR0A);

  return 0;
}
