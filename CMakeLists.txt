cmake_minimum_required(VERSION 3.13)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

if(NOT DEFINED TOOLCHAIN_PREFIX)
    foreach(tc riscv64-unknown-elf riscv-none-elf riscv64-elf)
        find_program(_TC_${tc} ${tc}-gcc)
        if(_TC_${tc})
            set(TOOLCHAIN_PREFIX "${tc}-" CACHE STRING "RISC-V GCC toolchain prefix")
            break()
        endif()
    endforeach()
endif()
if(NOT TOOLCHAIN_PREFIX)
    message(FATAL_ERROR "No RISC-V toolchain found (riscv64-unknown-elf / riscv-none-elf / riscv64-elf). Set -DTOOLCHAIN_PREFIX=xxx-")
endif()

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)

project(U2HTS_V003J4 C ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "" FORCE)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Os -march=rv32ec -mabi=ilp32e -DCH32V003 -ffunction-sections -fdata-sections -fmessage-length=0 -msmall-data-limit=8 -fno-tree-loop-distribute-patterns -Wall")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -march=rv32ec -mabi=ilp32e")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# u2hts_core cache overrides must come BEFORE add_subdirectory
set(U2HTS_MAX_TPS 5 CACHE STRING "" FORCE)
set(U2HTS_LOG_LEVEL -1 CACHE STRING "" FORCE)
set(U2HTS_ENABLE_COMPACT_REPORT ON CACHE BOOL "" FORCE)
add_subdirectory(u2hts_core)

# Touch controller (default gt9xx; override with -DTOUCH_CONTROLLER=...)
set(TOUCH_CONTROLLER ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/gt9xx.c
    CACHE FILEPATH "touch controller source file")

set_source_files_properties(${TOUCH_CONTROLLER} PROPERTIES COMPILE_FLAGS -fno-lto)

add_executable(U2HTS_V003J4
    ${CMAKE_CURRENT_LIST_DIR}/ch32fun/ch32fun/ch32fun.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_v003j4.c
    ${CMAKE_CURRENT_LIST_DIR}/rv003usb/rv003usb/rv003usb.S
    ${CMAKE_CURRENT_LIST_DIR}/rv003usb/rv003usb/rv003usb.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_v003j4_impl.c
    ${CMAKE_CURRENT_LIST_DIR}/CH32V00x_lib_i2c/lib_i2c.c
    ${TOUCH_CONTROLLER}
)

target_include_directories(U2HTS_V003J4 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/ch32fun/ch32fun
    ${CMAKE_CURRENT_LIST_DIR}/ch32fun/extralibs
    ${CMAKE_CURRENT_LIST_DIR}/rv003usb/rv003usb
    ${CMAKE_CURRENT_LIST_DIR}/CH32V00x_lib_i2c
)

target_link_libraries(U2HTS_V003J4 PRIVATE u2hts_core -lgcc)

target_link_options(U2HTS_V003J4 PRIVATE
    -march=rv32ec 
    -mabi=ilp32e
    -nostdlib
    -static-libgcc
    -L${CMAKE_CURRENT_LIST_DIR}/ch32fun/misc
    -T ${CMAKE_CURRENT_LIST_DIR}/ch32v003_flash.ld
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map
)

set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin)
set(LST_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.lst)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${CMAKE_PROJECT_NAME}> > ${LST_FILE}
        BYPRODUCTS ${HEX_FILE} ${BIN_FILE} ${LST_FILE}
)