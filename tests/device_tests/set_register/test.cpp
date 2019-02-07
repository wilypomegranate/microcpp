extern "C" {
#include <avr/io.h>
}
#include "Console.hpp"
#include "Register.hpp"

using namespace microcpp;

int main() {

  Register test(TCCR0A);
  test.set(WGM00, WGM01);

  Console console(9600);
  console.write(4000294967295);
  console.write("\n");
  return 0;
}
