#include "Console.hpp"
#include "SString.hpp"

namespace microcpp {
namespace wifi {
class Esp8266 {
public:
  enum class AP_MODE {};
  const static uint32_t DEFAULT_BAUD_RATE = 111520;

  Esp8266(uint32_t baud_rate) : console(baud_rate) {}
  bool join_ap() {
    // Requires macros for SSID and PW to be set.
    // TODO Should be constexpr defined outside of here.
    console.write("AT+CWJAP=");
    console.write(MICROCPP_WIFI_SSID);
    console.write(",");
    console.write(MICROCPP_WIFI_PW);
    console.write("\r\n");
    return true;
  }

  bool connected_ap() {
    SString<32> ap_name;
    console.write("AT+CWJAP?\r\n");
    if (!handle_response(ap_name)) {
      return false;
    }
    if (ap_name == MICROCPP_WIFI_SSID) {
      return true;
    }
    return false;
  }

  bool quit_ap() {
    console.write("AT+CWQAP\r\n");
    return true;
  }

  // bool set_ap_mode() {}

  bool udp_connect(const char *addr, const uint16_t &port) {
    // 'AT+CIPSTART', 'UDP', '255.255.255.255', 24000
    console.write("AT+CIPSTART, UDP,\"");
    console.write(addr);
    console.write("\",");
    console.write(port);
    console.write("\r\n");
    return true;
  }

private:
  template <size_t Size> bool handle_response(SString<Size> &buffer) {
    console.read_until(buffer, '\n');
    // If ERROR or FAIL is read, then this is a failure.
    if (buffer == "ERROR\r" || buffer == "FAIL\r") {
      return false;
    }
    return true;
  }

  Console console;
};
} // namespace wifi
} // namespace microcpp
