cmake_minimum_required(VERSION 3.20)

# Define toolchain before project()
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake" CACHE FILEPATH "ARM GCC toolchain file")
endif()

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Define language standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core project
project(ACE LANGUAGES C CXX ASM)

# MCU specifics
include("cmake/stm32f0.cmake")

# Add drivers
add_subdirectory("drivers/")

# Add libraries
add_subdirectory("libs/")

# Add applications
add_subdirectory("src/")