cmake_minimum_required(VERSION 3.28)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_STANDARD 11)

project(stm32f4_hal C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MCPU cortex-m4)

add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-O0 -g)

add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_definitions(-DCORE_CM4 -DDEBUG -DSTM32F446xx)

set(EXTERNAL_SOURCES_PATH ${CMAKE_SOURCE_DIR}/extern)
set(CMSIS_SOURCES_PATH ${EXTERNAL_SOURCES_PATH}/cmsis)
set(CORE_SOURCES_PATH ${EXTERNAL_SOURCES_PATH}/core)

set(STM32F4_HAL_SOURCES 
  src/buffer.c
  src/clock.c
  src/delay.c
  src/dma.c
  src/gpio.c
  src/init.c
  src/spi.c
  src/timer.c
  src/uart.c
  src/util.c
)
set(STM32F4_HAL_INCLUDES
  include/hal/buffer.h
  include/hal/clock.h
  include/hal/delay.h
  include/hal/dma.h
  include/hal/gpio.h
  include/hal/init.h
  include/hal/spi.h
  include/hal/timer.h
  include/hal/uart.h
  include/hal/util.h
)

set(STARTUP_FILE ${CMSIS_SOURCES_PATH}/Source/Templates/gcc/startup_stm32f446xx.s)
set(SYSTEM_FILE ${CMSIS_SOURCES_PATH}/Source/Templates/system_stm32f4xx.c)

set(CMSIS_SOURCES ${STARTUP_FILE} ${SYSTEM_FILE})

add_library(${PROJECT_NAME} STATIC ${STM32F4_HAL_SOURCES} ${STM32F4_HAL_INCLUDES})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/include
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
)


add_library(cmsis STATIC ${CMSIS_SOURCES})

target_include_directories(cmsis
        PUBLIC ${CMSIS_SOURCES_PATH}/Include
        PUBLIC ${CORE_SOURCES_PATH}/Core/Include
)

#if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/libcmsis.a")                                                                                                         
#    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/libcmsis.a" DESTINATION "${INSTALL_LIB_DIR}/")
#endif ()
#
#if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.a")
#    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.a" DESTINATION "${INSTALL_LIB_DIR}/")
#endif ()

