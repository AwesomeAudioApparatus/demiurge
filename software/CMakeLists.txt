set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.17)


if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# project settings
project(demiurge C ASM)
set(CMAKE_C_STANDARD 99)
set(PROJECT_BINARY_DIR CMakeFiles)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if(BOARD STREQUAL demi1-ch32)
    # specify cross compilers and tools
    set(CMAKE_C_COMPILER riscv-none-elf-gcc CACHE STRING "May be set by IDE")
    set(CMAKE_ASM_COMPILER riscv-none-elf-gcc CACHE STRING "May be set by IDE")
    set(CMAKE_AR riscv-none-elf-ar CACHE STRING "May be set by IDE")
    set(CMAKE_OBJCOPY riscv-none-elf-objcopy CACHE STRING "May be set by IDE")
    set(CMAKE_OBJDUMP riscv-none-elf-objdump CACHE STRING "May be set by IDE")
    set(SIZE riscv-none-elf-size CACHE STRING "May be set by IDE")

    include_directories(boards mcu/ch32v307/debug components/demiurge/inc components/littlefs mcu/ch32v307/cpu mcu/ch32v307/rtthread/libcpu/risc-v/common mcu/ch32v307/rtthread mcu/ch32v307/rtthread/components/drivers/include mcu/ch32v307/rtthread/components/finsh mcu/ch32v307/system/inc mcu/ch32v307/peripherals/inc mcu/ch32v307/rtdrivers mcu/ch32v307/rtthread/include mcu/ch32v307/rtthread/include/libc )
    add_compile_options(-DBOARD=${BOARD} -MMD -MP -msmall-data-limit=8 -mstrict-align -msave-restore -march=rv32imafc -mabi=ilp32f -O0 -fno-common -fno-inline-functions -Wunused -Wuninitialized -g -fmessage-length=0 -ffunction-sections -fdata-sections)
    add_link_options(-march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mstrict-align -msave-restore -O0)
    add_link_options(-fmessage-length=0 -ffunction-sections -fdata-sections -fno-common -fno-inline-functions -Wunused -Wuninitialized)
    add_link_options(-g -T ${CMAKE_SOURCE_DIR}/mcu/ch32v307/link.ld -nostartfiles -Xlinker --gc-sections )
    add_link_options(-Wl,-Map,${PROJECT_BINARY_DIR}/demiurge.map --specs=nano.specs --specs=nosys.specs)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-anhlmsd=$@.lst")
endif()

#if(STM32F405)
#    # specify cross compilers and tools
#    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
#    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
#    set(CMAKE_AR arm-none-eabi-ar)
#    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
#    set(CMAKE_OBJDUMP arm-none-eabi-objdump)
#    set(SIZE arm-none-eabi-size)
#    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#
#    add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#    add_compile_options(-mfpu=fpv4-sp-d16)
#    add_link_options(-mfpu=fpv4-sp-d16)
#
#    add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
#    add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
#
#    include_directories(Middlewares/ST/ARM/DSP/Inc Drivers/CMSIS/Include Drivers/STM32F4xx_HAL_Driver/Inc Core/Inc AwesomeAudioApparatus/Demiurge/Inc Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/STM32F4xx_HAL_Driver/Inc/Legacy)
#
#    add_definitions(-DSTM32F405xx -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG)
#
#    file(GLOB_RECURSE SOURCES "Library/*.*" "Core/*.*" "Middlewares/*.*" "AwesomeAudioApparatus/*.*" "Drivers/*.*")
#
#    set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F405RGTX_FLASH.ld)
#
#    add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/stm32f405/${PROJECT_NAME}.map)
#    add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
#    add_link_options(-T ${LINKER_SCRIPT})
#
#    add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
#    target_link_libraries(${PROJECT_NAME}.elf ${CMAKE_SOURCE_DIR}/Middlewares/ST/ARM/DSP/Lib/libarm_cortexM4l_math.a)
#endif()

file(GLOB_RECURSE SOURCES boards/*.c components/*.c mcu/*.c mcu/*.S examples/*.c user/*.c)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
#    add_compile_options(-Og -g)
    add_compile_options(-g)
endif()

link_libraries(m)
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "\nBuilt ${HEX_FILE}\nBuilt ${BIN_FILE}")
