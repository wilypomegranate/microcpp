cmake_minimum_required(VERSION 3.12)

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

  add_executable(device_tests.set_register.elf tests/device_tests/set_register/test.cpp)
  set_target_properties(device_tests.set_register.elf PROPERTIES LINKER_LANGUAGE CXX)
  set_target_properties(device_tests.set_register.elf PROPERTIES COMPILE_FLAGS "-fverbose-asm -Wa,-adhln=device_tests.set_register.s")
  target_include_directories(device_tests.set_register.elf PRIVATE src)

  add_executable(device_tests.string_conversion.elf tests/device_tests/string_conversion/test.cpp)
  set_target_properties(device_tests.string_conversion.elf PROPERTIES LINKER_LANGUAGE CXX)
  set_target_properties(device_tests.string_conversion.elf PROPERTIES COMPILE_FLAGS "-fverbose-asm -Wa,-adhln=device_tests.string_conversion.s")
  target_include_directories(device_tests.string_conversion.elf PRIVATE src)
else()
  project(microcpp VERSION 0.1.0 LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  add_executable(unit_tests.string tests/unit_tests/string/test.cpp)
  target_include_directories(unit_tests.string PRIVATE src)
endif()
