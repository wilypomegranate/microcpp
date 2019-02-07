#pragma once

// Right now a collection of useful string functions.
// This is a pre-cursor to the static and dynamic string
// implementations that I want to write.
extern "C" {
#include <stdint.h>
}

namespace microcpp {
/// Calculate the log of the base rounded up to an integer.
/// This is used for sizing conversion buffers.
/// This can be used at compile time with constants.
/// TODO This should validate an integer type for T.
template <typename T>
constexpr uint16_t log_base_int(const T &value, uint8_t base) {
  uint16_t size = 1;
  for (T i = base; i <= value; i *= base) {
    ++size;
  }
  return size;
}

/// Take an integer type and place it in the provided char buffer.
/// TODO Should validate the incoming type is an integer type.
/// TODO Should support signed numbers.
template <typename T>
constexpr void micro_itoa(const T &value, char *buffer, uint8_t base) {
  uint16_t counter = 0;
  /// Use a similar algorithm to K&R.
  /// The difference is we know what the size is going to be.
  const uint16_t num_elements = log_base_int(value, base);
  T n = value;
  do {
    buffer[num_elements - ++counter] = n % base + '0';
  } while ((n /= base) > 0);
  buffer[counter] = '\0';
}
} // namespace microcpp
