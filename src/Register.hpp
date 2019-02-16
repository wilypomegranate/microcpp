#pragma once
namespace microcpp {
namespace reg {
/// Class to represent a register
template <class T> class Register {
public:
  explicit Register(volatile T &value) : value(&value) {}

  /// Alternate constructor to just directly set the value.
  /// Overwrites unspecified bits to 0.
  Register(volatile T &location, T new_value) : value(&location) {
    (*value) = new_value;
  }

  T bitwise_or(T reg) {
    return (1 << reg);
  }

  template <typename... Ts> T bitwise_or(T reg, Ts... regs) {
    return (1 << reg) | bitwise_or(regs...);
  }

  /// Set the provided bits to 1. All other bits will stay
  /// the same value.
  template <typename... Ts> void set_bits(T reg, Ts... regs) {
    (*value) |= bitwise_or(reg, regs...);
  }

  /// Clear specific bits.
  template <typename... Ts> void clear_bits(T reg, Ts... regs) {
    (*value) &= (~(bitwise_or(reg, regs...)));
  }

  /// Flip specific bits.
  template <typename... Ts> void flip_bits(T reg, Ts... regs) {
    (*value) ^= bitwise_or(reg, regs...);
  }

  /// Set the register to a specific value.
  Register &operator=(const T &new_value) {
    (*value) = new_value;
    return *this;
  }

private:
  volatile T *value;
};

/// In addition to the stateful interface, a simpler static interface
/// also is provided.

/// Set the value of a particular register.
template <typename T, typename U>
inline void set_register(volatile T &location, U new_value) {
  location = new_value;
}

template <typename T> inline T bitwise_or(T reg) {
  return (1 << reg);
}

template <typename T, typename... Ts> inline T bitwise_or(T reg, Ts... regs) {
  return (1 << reg) | bitwise_or(regs...);
}

/// Set specific bits of a register.
template <typename T, typename U, typename... Ts>
inline void set_bits(volatile T &location, U reg, Ts... regs) {
  location |= bitwise_or(reg, regs...);
}

/// Clear specific bits of a register.
template <typename T, typename U, typename... Ts>
inline void clear_bits(volatile T &location, U reg, Ts... regs) {
  location &= (~(bitwise_or(reg, regs...)));
}

/// Flip specific bits of a register.
template <typename T, typename U, typename... Ts>
inline void flip_bits(volatile T &location, U reg, Ts... regs) {
  location ^= bitwise_or(reg, regs...);
}
} // namespace reg
} // namespace microcpp
