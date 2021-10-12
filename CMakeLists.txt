cmake_minimum_required(VERSION 3.21)

# Specify the cross compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

# Set paths
set(SCATTER_PATH ${CMAKE_SOURCE_DIR}/MDK-ARM/Objects/release.sct)
set(HEX_PATH ${CMAKE_SOURCE_DIR}/MDK-ARM/Objects/release.hex)
set(TOOLCHAIN_PATH C:/Tools/Keil_v5/ARM/ARMCLANG/)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}bin/armclang.exe)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}bin/armclang.exe)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_C_LINK_EXECUTABLE ${TOOLCHAIN_PATH}bin/armlink.exe)
set(CMAKE_CXX_LINK_EXECUTABLE ${TOOLCHAIN_PATH}bin/armlink.exe)
set(CMAKE_ASM_LINK_EXECUTABLE ${TOOLCHAIN_PATH}bin/armlink.exe)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(DBG_FLAGS "-O1")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(DBG_FLAGS "-O3")
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    set(DBG_FLAGS "-Oz")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(DBG_FLAGS "-Ofast")
endif()

# Set flags
set(FPU_FLAGS "-mfpu=fpv5-d16 -mfloat-abi=hard")
set(COMMON_FLAGS "--target=arm-arm-none-eabi -mcpu=${CMAKE_SYSTEM_PROCESSOR} ${FPU_FLAGS}")
set(CMAKE_C_FLAGS " ${COMMON_FLAGS} ${DBG_FLAGS} -std=c99 -Wall -fno-rtti -c -ffunction-sections -gdwarf-4")
set(CMAKE_CXX_FLAGS " ${COMMON_FLAGS} ${DBG_FLAGS} -std=c++11 -Wall -fno-rtti -c -ffunction-sections -gdwarf-4")
set(CMAKE_ASM_FLAGS " ${COMMON_FLAGS} -masm=auto -c -gdwarf-3")
set(CMAKE_EXE_LINKER_FLAGS "
    --cpu Cortex-M7.fp.dp \
    --strict --scatter ${SCATTER_PATH} \
    --summary_stderr --info summarysizes \
    --map --load_addr_map_info --xref --callgraph --symbols \
    --info sizes --info totals --info unused --info veneers \
    "
)

enable_language(ASM)

# Configure the project
project(STM32V7 C ASM)

add_definitions(
    -DSTM32H743xx
)

include_directories(
    ${TOOLCHAIN_PATH}include
    ${TOOLCHAIN_PATH}include/arm_linux
    ${TOOLCHAIN_PATH}include/arm_linux_compat
    ${TOOLCHAIN_PATH}include/libcxx
    Core/Inc
    Drivers/STM32H7xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32H7xx/Include
    Drivers/CMSIS/Include
    Components/uCOS-III/uC-CONFIG
    Components/uCOS-III/uC-CPU/ARM-Cortex-M/ARMv7-M/ARM
    Components/uCOS-III/uC-CPU
    Components/uCOS-III/uC-LIB
    Components/uCOS-III/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/ARM
    Components/uCOS-III/uC-OS3/Source
    Components/BSP
    Components/Devices/TFTLCD
    Components/Devices/Stepmotor
)

file(GLOB_RECURSE SOURCES
    MDK-ARM/startup_*.s
    Core/Src/*.c
    Drivers/STM32*_Driver/Src/*.c
    Components/uCOS-III/uC-CPU/ARM-Cortex-M/ARMv7-M/ARM/cpu_a.asm
    Components/uCOS-III/uC-CPU/ARM-Cortex-M/ARMv7-M/cpu_c.c
    Components/uCOS-III/uC-CPU/cpu_core.c
    Components/uCOS-III/uC-LIB/*.c
    Components/uCOS-III/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/ARM/os_cpu_a.asm
    Components/uCOS-III/uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c
    Components/uCOS-III/uC-OS3/Source/*.c
    Components/BSP/*.c
    Components/Devices/TFTLCD/tftlcd.c
    Components/Devices/Stepmotor/stepmotor.c
)

add_executable(release ${SOURCES})

# Generate binary file
add_custom_command(TARGET release POST_BUILD
    COMMAND ${TOOLCHAIN_PATH}bin/fromelf.exe --i32 -o ${HEX_PATH} release.elf
)
