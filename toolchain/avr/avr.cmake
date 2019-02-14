set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_CXX_COMPILER avr-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# TODO Make this platform independent.
include_directories(SYSTEM /usr/avr/include)

set(CMAKE_C_FLAGS "-Os -Werror -Wall -pedantic -Wextra -fno-threadsafe-statics")
set(CMAKE_CXX_FLAGS "-Os -Werror -Wall -pedantic -Wextra -fno-threadsafe-statics")
