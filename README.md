# ACE-Template

A project structure for Arm Cortex Embedded programming

## Requirements

- CMake
- GNU Arm Embedded Toolchain

## Structure

```
.
|   cmake
|   |   gcc-arm-none-eabi.cmake (standard toolchain definition)
|   |   [MCU-famil].cmake       (MCU specific definitions)
|   |
|   drivers (drivers for target)
|   |   [MCU-family]-hal-driver
|   |   |   ...
|   |   |
|   |   ...
|   |
|   libs    (all kinds of vendor/own libs)
|   |   ...
|   |
|   src
|   |   app             (main app)
|   |   |   main.cpp
|   |   |   ...
|   |   |
|   |   test_apps       (tests for target-side)
|   |   |   ...
|   |   |
|   |   CMakeLists.txt  (manage executables)
|   |
|   tests   (unit testing on host-side)
|   |   CMakeLists.txt  (manage tests)
|   |   ...
|   |
|   CMakeLists.txt
|   LinkerScript.ld
|   Startup.s
|   ...
|
```