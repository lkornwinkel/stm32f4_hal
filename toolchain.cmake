
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(TOOLCHAIN_PATH "/home/lars/opt/STM32CubeCLT/GNU-tools-for-STM32/bin")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_AR ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objdump)
set(SIZE ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}size)

set(MCPU cortex-m4)

set(COMMON_FLAGS "-mcpu=${MCPU} -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -DCORE_CM4 -DDEBUG -DSTM32F446xx -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING")

set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "-mcpu=${MCPU} -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g")

set(CMAKE_EXE_LINKER_FLAGS_INIT "-mcpu=${MCPU} -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs")

#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_compile_options(-O0 -g)
#
#add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
#add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_definitions(-DCORE_CM4 -DDEBUG -DSTM32F446xx)


