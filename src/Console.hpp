#pragma once
#include "Register.hpp"

namespace microcpp {
// TODO The implementation for Console needs to be
// board specific. Also for AVR the general setting
// and calculation for these registers can be abstracted.
// Right now this only applies to the ATMEGA328P.

// TODO Add 2X USART supportl.

// TODO I guess the other thing here is that
// this is really a UART implementation with some
// serial console sugar on top. The UART code
// should probably be abstracted away so
// that it can be used for other purposes.

// TODO Formatting and working with string like
// types should be moved out into a separate
// abstraction.

class Console {
public:
  Console(uint32_t br) : baud_rate(br) {
    // Initialize UART with baud rate provided.
    // uint8_t baud_rate_clock = 0;
  }

private:
  uint32_t baud_rate;
};

} // namespace microcpp
