cmake_minimum_required(VERSION 3.7)

if(NOT MICROCPP_TOOLCHAIN_FILE)
  message(FATAL_ERROR "MICROCPP_TOOLCHAIN_FILE must be set")
endif()

set(
  CMAKE_TOOLCHAIN_FILE
  ${CMAKE_CURRENT_LIST_DIR}/toolchain/${MICROCPP_TOOLCHAIN_FILE}.cmake
  )

project(microcpp VERSION 0.1.0 LANGUAGES CXX)
