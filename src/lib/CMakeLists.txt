cmake_minimum_required(VERSION 3.17.3)
project(sense C)

set(CMAKE_C_STANDARD 11)
add_compile_definitions(_POSIX_C_SOURCE=200112L TORNADO_OS_LINUX CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)

include_directories(include)

set (DEPS ../../deps/)


file(GLOB_RECURSE lib_src FOLLOW_SYMLINKS
        "*.c"
        )

add_library(sense ${lib_src})

target_include_directories(sense PUBLIC ../include)
target_include_directories(sense PUBLIC ${DEPS}piot/basal-c/src/include)
target_include_directories(sense PUBLIC ${DEPS}piot/clog/src/include)
target_include_directories(sense PUBLIC ${DEPS}piot/tiny-libc/src/include)

target_link_libraries(sense -fsanitize=address)
