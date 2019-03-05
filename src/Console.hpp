#pragma once

#include "Register.hpp"
#include "SString.hpp"
#include "String.hpp"

extern "C" {
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <util/setbaud.h>
}

namespace microcpp {

// TODO The implementation for Console needs to be
// board specific. Also for AVR the general setting
// and calculation for these registers can be abstracted.
// Right now this only applies to the ATMEGA328P.

// TODO Add 2X USART support.

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

    // Setup UBRR{n}H/L registers.
    // The H register has the first four bits reserved.
    // TODO They need to be set to 0 for future compatibility.
    // The rest of the register is the combination of the
    // 12-bit calculated value of the baud rate.

    // Setup No 2x and 8-bit data.
    // TODO This shouldn't be hard-coded.
    reg::Register ucsr0a(UCSR0A);
    ucsr0a.clear_bits(U2X0);
    reg::Register ucsr0c(UCSR0C);
    ucsr0c.set_bits(UCSZ01, UCSZ00);
    reg::Register ucsr0b(UCSR0B);
    ucsr0b.set_bits(RXEN0, TXEN0);

    // Calculate baud rate.
    // Formula is (CLOCK_SPD / (16 * BAUD_RATE)) - 1.
    const uint16_t baud_rate_calc = (F_CPU / (16 * baud_rate)) - 1;

    // // Upper four bits should be set to the lower 4 bits of UBRR{n}H.
    reg::Register uart_high(UBRR0H,
                            static_cast<uint8_t>(baud_rate_calc & 0xf00));

    // // Lower eight bits should be set to UBRR{n}L.
    reg::Register uart_low(UBRR0L, static_cast<uint8_t>(baud_rate_calc & 0xff));
  }

  /// Write a specific char to the console.
  void write(const char &input) {
    reg::Register udr0(UDR0);
    while (!(UCSR0A & _BV(UDRE0))) {
    }
    udr0 = input;
  }

  // Read a specifc char from the console.
  void read(char &output) {
    while (!(UCSR0A & _BV(RXC0))) {
    }
    output = UDR0;
  }

  // Loop through a c-string and set the
  // UART register to write to the console.
  // TODO write should be overloaded as
  // needed to handle multiple types.
  void write(const char *input) {
    size_t counter = 0;
    char value = input[counter];
    // NOTE This isn't using string.h.
    // The idea is that we should reduce
    // looping as much as possible.
    reg::Register udr0(UDR0);
    while (value != '\0') {
      // Wait until the transmit buffer is empty.
      while (!(UCSR0A & _BV(UDRE0))) {
      }
      udr0 = value;
      value = input[++counter];
    }
  }

  // Read a SString's worth of data via UART.
  // Return how many bytes were actually read.
  // TODO Re-evaluate this method later.
  // template <size_t Size> size_t read(SString<Size> &buffer) {
  //   size_t bytes_read = 0;
  //   for (size_t i = 0; i < Size; ++i) {
  //     // TODO Handle error.
  //     read(buffer[i]);
  //     // TODO Actually figure out if character was read on some timeout.
  //     bytes_read++;
  //   }
  //   return bytes_read;
  // }

  // Read until a character and then return.
  // If the buffer isn't big enough or the character wasn't found,
  // return Size + 1. If success return bytes_read.
  template <size_t Size>
  size_t read_until(SString<Size> &buffer, const char &input,
                    bool strip = true) {
    size_t bytes_read = 0;
    for (size_t i = 0; i < Size; ++i) {
      // TODO Handle error.
      read(buffer[i]);
      bytes_read++;
      if (buffer[i] == input) {
        // If the found character is stripped, then remove it from bytes
        // read as well.
        if (strip) {
          buffer[i] = '\0';
          bytes_read--;
        }
        return bytes_read;
      }
    }
    return bytes_read;
  }

  template <typename IntType> void write(const IntType &input) {
    // TODO This should be deprecated and string formatting
    // should be used instead.
    // TODO Assert that this is an integer type.
    constexpr uint8_t base = 10;
    // String can't be bigger than 3 times the size of
    // the integer type.
    constexpr uint8_t scale_size = 3;
    SString<sizeof(IntType) * scale_size> buffer;
    micro_itoa(input, buffer.mut_c_str(), base);
    write(buffer.c_str());
  }

  /// Find the first '{' in the fmt string and substitute between
  /// that and the next char of '}'.
  /// If the next char is '{', that means it's an escape.
  /// If the next char is a  number, that means it's a
  /// specific variable argument.
  /// TODO If the number of arguments is incorrect, a compile-time
  /// error should be thrown.
  template <typename T, typename... Ts>
  void fmt_stream(const char *fmt, uint8_t &pos, T val, Ts... vals) {

    char value = fmt[pos];
    while (value != '\0') {
      if (value == '{') {
        // Now write the next val.
        write(val);
        value = fmt[++pos];
        value = fmt[++pos];
        fmt_stream(fmt, pos, vals...);
        break;
      }
      else {
        if (value != '}') {
          write(value);
        }
      }
      value = fmt[++pos];
    }
  }

  template <typename T> void fmt_stream(const char *fmt, uint8_t &pos, T val) {

    char value = fmt[pos];

    while (value != '\0') {
      if (value == '{') {
        write(val);
      }
      else {
        if (value != '}') {
          write(value);
        }
      }
      value = fmt[++pos];
    }
  }

  template <typename... Ts> void fmt(const char *fmt, Ts... vals) {
    uint8_t pos = 0;
    fmt_stream(fmt, pos, vals...);
  }

private:
  uint32_t baud_rate;
}; // namespace microcpp

} // namespace microcpp
