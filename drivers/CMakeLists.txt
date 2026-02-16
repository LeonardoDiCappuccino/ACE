cmake_minimum_required(VERSION 3.20)


# CMSIS Core
add_library(cmsis_core INTERFACE)
target_include_directories(cmsis_core INTERFACE "cmsis-core/Include/")


# CMSIS Device files for STM32F042x6
add_library(cmsis_device_stm32f042x6 INTERFACE)

target_include_directories(cmsis_device_stm32f042x6 INTERFACE "cmsis-device-f0/Include/")

target_link_libraries(cmsis_device_stm32f042x6 INTERFACE cmsis_core)

target_compile_definitions(cmsis_device_stm32f042x6 INTERFACE STM32F042x6)   # Define specific MCU


# HAL Drivers for STM32F042x6
file(GLOB stm32f042x6_hal_driver_src "stm32f0xx-hal-driver/Src/*.c")

list(FILTER stm32f042x6_hal_driver_src EXCLUDE REGEX ".*_template\\.c$")   # Exclude template files

add_library(stm32f042x6_hal_driver STATIC "${stm32f042x6_hal_driver_src}")

target_include_directories(stm32f042x6_hal_driver PUBLIC 
    "stm32f0xx-hal-driver/Inc/"
    "${CMAKE_SOURCE_DIR}/src/setup/"    # Include hal config
)

target_link_libraries(stm32f042x6_hal_driver PUBLIC cmsis_device_stm32f042x6)

target_compile_definitions(stm32f042x6_hal_driver PUBLIC USE_HAL_DRIVER)