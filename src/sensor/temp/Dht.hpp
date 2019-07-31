#include "Console.hpp"
#include "Register.hpp"

extern "C" {
#include <string.h>
#include <util/delay.h>
}

namespace microcpp {
namespace sensor {
namespace temp {

class DhtData {

public:
  DhtData() : rh_(0), temp_(0) {}
  DhtData(uint16_t rh, uint16_t temp) : rh_(rh), temp_(temp) {}

  double rh() {
    return rh_;
  }

  double temp() {
    return temp_;
  }

  void set_data(uint16_t rh, uint16_t temp) {
    rh_ = rh;
    temp_ = temp;
  }

  // private:
  uint16_t rh_;
  uint16_t temp_;
};

class Dht {
public:
  // TODO Validate sensor.
  explicit Dht(reg::Register<unsigned char> &output,
               reg::Register<unsigned char> &input,
               reg::Register<unsigned char> &io, int pin)
      : output(output), input(input), io(io), pin(pin) {

    // Configure port & pin as input.
    set_input();

    // TODO Check if port is high.
    Console console(9600);

    if (!is_high()) {
      console.write("Error Initializing DHT Sensor\r\n");
    }
  }

  bool poll(DhtData &data) {
    // First set low to start the polling process.
    uint8_t buffer[40];
    memset(&buffer, 0, 40);

    set_output();
    set_low();
    _delay_ms(10);
    set_high();

    // Wait for response.
    set_input();
    _delay_us(40);
    _delay_us(160);

    // Recv data here.
    for (uint8_t count = 0; count < 40; ++count) {
      _delay_us(50);

      _delay_us(29);

      if (is_high()) {
        // Means this was a 1.
        buffer[count] = 1;
        _delay_us(44);
      }
      // Means this was a 0.
      // Don't do anything here because it's set at the start.
    }

    uint16_t rh = read_uint16_t(buffer, 0);
    uint16_t temp = read_uint16_t(buffer, 16);
    uint8_t checksum = read_byte(buffer, 32);

    if (validate_checksum(rh, temp, checksum)) {
      data.set_data(rh, temp);
      return true;
    }

    return false;
  }

private:
  reg::Register<unsigned char> output;
  reg::Register<unsigned char> input;
  reg::Register<unsigned char> io;
  int pin;

  uint8_t read_byte(uint8_t *buffer, uint8_t start) {
    uint8_t value = 0;
    uint8_t end = start + 8;
    uint8_t idx_end = end - 1;

    for (uint8_t i = start; i < end; i++) {
      if (buffer[i] == 1) {
        value |= (1 << (idx_end - i));
      }
      else {
        value &= ~(1 << (idx_end - i));
      }
    }
    return value;
  }

  uint16_t read_uint16_t(uint8_t *buffer, uint8_t start) {
    uint16_t value = 0;
    uint16_t val1 = (uint16_t)(read_byte(buffer, start));
    uint16_t val2 = (uint16_t)(read_byte(buffer, start + 8));
    value = (val1 << 8) | (val2 & 0xFF);
    return value;
  }

  bool validate_checksum(uint16_t rh, uint16_t temp, uint8_t checksum) {
    uint8_t check = (rh >> 8) + (rh & 0xFF) + (temp >> 8) + (temp & 0xFF);
    return checksum == check;
  }

  bool recv_bit() {
    _delay_us(26);
    if (!is_high()) {
      // Means this was a 0.
      return false;
    }
    else {
      // Means this was a 1.
      _delay_us(44);
      return true;
    }
  }

  void set_output() {
    io.set_bits(pin);
  }

  void set_input() {
    io.clear_bits(pin);
  }

  void set_high() {
    output.set_bits(pin);
  }

  void set_low() {
    output.clear_bits(pin);
  }

  inline bool is_high() {
    return input & (1 << pin);
  }
}; // namespace temp
} // namespace temp
} // namespace sensor
} // namespace microcpp
