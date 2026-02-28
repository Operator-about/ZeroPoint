cmake_minimum_required(VERSION 3.15)

project(ZeroPoint ASM C)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/C-source/Interrput/Interrput.c
    ${CMAKE_CURRENT_SOURCE_DIR}/C-source/Interrput/Interrput-common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ASM/startup.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/ASM/table-vector.asm)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Headers/)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    "-T${LINKER_SCRIPT}"
    "-nostartfiles"
    "--specs=nosys.specs")
