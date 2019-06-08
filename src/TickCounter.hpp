#pragma once

#include "Register.hpp"

extern "C" {
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
}

namespace microcpp {

/// A class for keeping track of the total uptime of the board.
/// It allows for dispatching of callbacks when a registered has triggered the
/// delay period.
/// The specific resolution cannot be lower than the device's miniumum
/// resolution.
/// This class is a Singleton because there should only be one counter
/// that globally keeps the uptime count.
/// This is even the case when multiple timers may exist on the board.
/// If the user need a separate timer they should engage it outside of
/// this abstraction.

// TODO Evaluate whether a separate timer class abstracting the
// registers should be used as the timing source of this class.
// TODO Figure out a good way to handle different granularities.

template <typename CounterSize = uint32_t> class TickCounter {
public:
  static TickCounter &instance() {
    static TickCounter counter;
    return counter;
  }

  CounterSize uptime() const {
    return uptime_;
  }

  inline void increment() {
    uptime_ += (64 * 3);
    reg::set_register(TCNT0, 0x00);
    reg::set_register(OCR0A, 2);
  }

private:
  TickCounter() : uptime_(0) {
    reg::set_register(TCNT0, 0x00);

    // CPU cycles / 1024.
    reg::set_bits(TCCR0B, CS00, CS02);

    // 1/1024 of a tick.
    reg::set_register(OCR0A, 2);

    reg::set_bits(TIMSK0, OCIE0A);

    // Enable global interrupts.
    sei();
  }
  CounterSize uptime_;
};

TickCounter counter = TickCounter<>::instance();
ISR(TIMER0_COMPA_vect) // Timer1 ISR
{
  counter.increment();
}

} // namespace microcpp
