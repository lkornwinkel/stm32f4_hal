project(uartt1_test)

include(${CMAKE_SOURCE_DIR}/toolchain.cmake)

add_executable(uart1_test
    uartt1-test.c
#    ${CMAKE_SOURCE_DIR}/extern/cmsis/Source/Templates/system_stm32f4xx.c

)

target_link_libraries(uart1_test
    stm32f4_hal
)