#pragma once
namespace microcpp {
template <class T> class Register {
public:
  explicit Register(volatile T &value) : value(&value) {}

  T bitwise_or(T reg) { return reg; }

  template <typename... Ts> T bitwise_or(T reg, Ts... regs) {
    return (1 << reg) | (1 << bitwise_or(regs...));
  }

  template <typename... Ts> void set(T reg, Ts... regs) {
    (*value) |= bitwise_or(reg, regs...);
  }

private:
  volatile T *value;
};
} // namespace microcpp
