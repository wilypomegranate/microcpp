cmake_minimum_required(VERSION 3.12)

macro(flash_device)
endmacro()

macro(add_device_test test_name test_src)
  add_executable(${test_name}.elf ${test_src})
  set_target_properties(${test_name}.elf PROPERTIES LINKER_LANGUAGE CXX)
  set_target_properties(
    ${test_name}.elf
    PROPERTIES COMPILE_FLAGS
    "-fverbose-asm -Wa,-adhln=${test_name}.s"
    )
  target_include_directories(${test_name}.elf PRIVATE src)

  add_custom_command(
    OUTPUT ${test_name}.hex
    COMMAND avr-objcopy -O ihex -R .eeprom ${test_name}.elf ${test_name}.hex
    DEPENDS ${test_name}.elf
    )

  add_custom_target(
    ${test_name}.flash
    COMMAND avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:${test_name}.hex
    DEPENDS ${test_name}.hex
    )

endmacro()

if(NOT MICROCPP_TOOLCHAIN_FILE)
  message(FATAL_ERROR "MICROCPP_TOOLCHAIN_FILE must be set")
endif()

if(NOT UNIT_TESTS)
  set(
    CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_LIST_DIR}/toolchain/${MICROCPP_TOOLCHAIN_FILE}.cmake
    )

  project(microcpp VERSION 0.1.0 LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)

  add_device_test(device_tests.set_register tests/device_tests/set_register/test.cpp)
  add_device_test(device_tests.string_conversion tests/device_tests/string_conversion/test.cpp)
  add_device_test(device_tests.uptime tests/device_tests/uptime/test.cpp)
  add_device_test(device_tests.audio_player tests/device_tests/audio_player/test.cpp)
  add_device_test(device_tests.uart tests/device_tests/uart/test.cpp)
else()
  project(microcpp VERSION 0.1.0 LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  add_executable(unit_tests.string tests/unit_tests/string/test.cpp)
  target_include_directories(unit_tests.string PRIVATE src)
  add_executable(unit_tests.circular_buffer tests/unit_tests/circular_buffer/test.cpp)
  target_include_directories(unit_tests.circular_buffer PRIVATE src)
endif()
