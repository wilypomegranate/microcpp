#pragma once

extern "C" {
#include <stddef.h>
#include <stdint.h>
}

namespace microcpp {

/// A class for handling static strings where the size
/// is known at compile time.
/// Should replace usage of char[] buffers.
/// Memory is stack allocated and the size isn't
/// changeable after creation.
/// The size of the SString is setup to be a size_t.
/// So the size is platform dependent.
/// TODO Add some compile time checking to make sure
/// the size is reasonable for the platform.
/// Practially for the AVR platform this should never go
/// beyond 255 in size.
/// This class supports constexpr since a lot of the time
/// strings are well-known at compile time.
template <size_t Size = 24> class SString {
public:
  constexpr SString() : buffer{'\0'} {}

  /// Take in a c-string.
  /// This assumes that the c-string is < Size.
  constexpr SString(const char *input) : buffer() {
    assign_cstr(input);
  }

  /// Copy-assignment overload for c-strings.
  /// SStrings should be assignable to a c-string.
  SString &operator=(const char *rhs) {
    assign_cstr(rhs);
    return *this;
  }

  constexpr size_t size() const {
    return Size;
  }

  const char *c_str() const {
    return buffer;
  }

private:
  char buffer[Size];

  /// Shared method for setting the buffer via a c-string.
  /// TODO Some sort of error should be thrown
  /// at least at compile time if this can't
  /// be handled by Size.
  constexpr void assign_cstr(const char *input) {
    size_t counter = 0;
    // Set to bogus value that's not \0.
    char value = 'a';
    do {
      value = input[counter];
      buffer[counter] = value;
      counter++;
    } while (value != '\0');
  }
};
} // namespace microcpp
