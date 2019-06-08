extern "C" {
#include <avr/io.h>
}
#include "Console.hpp"
#include "Register.hpp"

using namespace microcpp;

int main() {

  Console console(9600);

  // Read some data from the console.
  SString<256> buffer;

  console.write("Initialized\n");
  console.read_until(buffer, '\n');
  console.fmt("Data {}\n", buffer.c_str());

  return 0;
}
