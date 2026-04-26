cmake_minimum_required(VERSION 3.15)

project(ZeroPoint ASM C)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/GICv2.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/GICv3.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/MMU.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/Interrupts.c
${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/UART.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/kernel-modules.asm
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/table-vector.asm
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/boot/startup.asm
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/boot/info.asm
${CMAKE_CURRENT_SOURCE_DIR}/src/Kernel.c
${CMAKE_CURRENT_SOURCE_DIR}/common/Ella.c
${CMAKE_CURRENT_SOURCE_DIR}/common/Kernel-modules.c)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    "-T${LINKER_SCRIPT}"
    "-nostartfiles"
    "--specs=nosys.specs")
