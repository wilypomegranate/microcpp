#pragma once
namespace microcpp {
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
    return reg;
  }

  template <typename... Ts> T bitwise_or(T reg, Ts... regs) {
    return (1 << reg) | (1 << bitwise_or(regs...));
  }

  /// Set the provided bits to 1. All other bits will stay
  /// the same value.
  template <typename... Ts> void set_bits(T reg, Ts... regs) {
    (*value) |= bitwise_or(reg, regs...);
  }

  /// Set the register to a specific value.
  Register &operator=(const T &new_value) {
    (*value) = new_value;
    return *this;
  }

private:
  volatile T *value;
};
} // namespace microcpp
