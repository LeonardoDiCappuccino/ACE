cmake_minimum_required(VERSION 3.20)

# Main application
add_executable(${CMAKE_PROJECT_NAME} 
    "app/main.cpp"

    "setup/system_stm32f0xx.c"
    "setup/startup_stm32f042x6.s"
    "setup/syscalls.c"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE stm32f042x6_hal_driver)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/setup/stm32f042x6_FLASH.ld"
    -Wl,-Map=$<TARGET_FILE_BASE_NAME:${CMAKE_PROJECT_NAME}>.map
)


# Serial Print test application
add_executable(serial_print 
    "test_apps/serial_print.cpp"

    "setup/system_stm32f0xx.c"
    "setup/startup_stm32f042x6.s"
    "setup/syscalls.c"
)

target_link_libraries(serial_print PRIVATE stm32f042x6_hal_driver)

target_link_options(serial_print PRIVATE
    "-T${CMAKE_CURRENT_SOURCE_DIR}/setup/stm32f042x6_FLASH.ld"
    -Wl,-Map=$<TARGET_FILE_BASE_NAME:serial_print>.map
)