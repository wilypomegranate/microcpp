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
/// beyond 255 in capacity.
/// This class supports constexpr since a lot of the time
/// strings are well-known at compile time.
template <size_t Capacity = 24> class SString {
public:
  constexpr SString() : buffer{'\0'} {}

  /// Take in a c-string.
  /// This assumes that the c-string is < Capacity.
  /// TODO I really want it to be possible for the input size to
  /// be determined at compile time  from the const char* input.
  constexpr SString(const char *input) : str_size(0), buffer() {
    assign_cstr(input);
  }

  /// Copy-assignment overload for c-strings.
  /// SStrings should be assignable to a c-string.
  SString &operator=(const char *rhs) {
    assign_cstr(rhs);
    return *this;
  }

  /// Copy constructor for SStrings of a smaller capacity.
  template <size_t SmallerCapacity>
  SString(const SString<SmallerCapacity> &rhs)
      : str_size(rhs.length()), buffer() {
    static_assert(Capacity >= SmallerCapacity,
                  "New SString must be at least the same size.");
    for (size_t i = 0; i < SmallerCapacity; ++i) {
      buffer[i] = rhs[i];
    }
  }

  constexpr size_t capacity() const {
    return Capacity;
  }

  constexpr size_t size() const {
    return str_size;
  }

  constexpr size_t length() const {
    return str_size;
  }

  const char *c_str() const {
    return buffer;
  }

  // NOTE A dangerous method.
  /// Gives raw acces to the buffer.
  /// Only for debugging purposes.
  char *mut_c_str() {
    return buffer;
  }

  // NOTE A dangerous method.
  /// Update the str_size in the case
  /// where the buffer is directly modified.
  /// Only for debugging purposes.
  void set_str_size(size_t new_str_size) {
    str_size = new_str_size;
  }

  // TODO Bounds checking.
  char operator[](size_t idx) const {
    return buffer[idx];
  }

  // TODO Bounds checking.
  char &operator[](size_t idx) {
    // When an index is accessed beyond the current size, it increases
    // the total size to that index.
    // TODO Should this fill unused indices with zeros?
    if (idx <= str_size) {
      str_size = idx + 1;
    }
    return buffer[idx];
  }

  /// Check to see if two SStrings are equivalent.
  /// str_size must be equivalent before checking
  /// that the bytes match.
  template <size_t OtherCapacity>
  bool operator==(const SString<OtherCapacity> &rhs) const {
    if (length() != rhs.length()) {
      return false;
    }

    for (size_t i = 0; i < str_size; ++i) {
      if (buffer[i] != rhs[i]) {
        return false;
      }
    }

    return true;
  }

private:
  // This is the actual string length.
  size_t str_size;
  char buffer[Capacity];

  /// Shared method for setting the buffer via a c-string.
  /// TODO Some sort of error should be thrown
  /// at least at compile time if this can't
  /// be handled by Capacity.
  constexpr void assign_cstr(const char *input) {
    // Set to bogus value that's not \0.
    char value = 'a';
    do {
      value = input[str_size];
      buffer[str_size] = value;
      str_size++;
    } while (value != '\0');
  }
};
} // namespace microcpp
