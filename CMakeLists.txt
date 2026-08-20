cmake_minimum_required(VERSION 3.15)

project(ZeroPoint ASM C)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/MMU.c
${CMAKE_CURRENT_SOURCE_DIR}/src/Kernel.c
${CMAKE_CURRENT_SOURCE_DIR}/common/std.c
${CMAKE_CURRENT_SOURCE_DIR}/common/Stringz.c
${CMAKE_CURRENT_SOURCE_DIR}/common/Mathz.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/table-handel.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/ZeroShell.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/SD/SD.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/SD/SD-DW.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/SD/SD-Standart.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/Kernel-modules.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/exFAT/exFAT-PhysicalLayer.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/UART/PL011.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/UART/165050.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/exFAT/exFAT-VirtualLayer.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/c-source/exFAT/MBR.c
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/boot/table-vector.asm
${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/boot/startup.asm)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/UART/)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/SD/)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/exFAT/)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/common/)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    "-T${LINKER_SCRIPT}"
    "-mcmodel=large"
    "-nostartfiles"
    "--specs=nosys.specs")
