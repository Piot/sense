cmake_minimum_required(VERSION 3.17.3)
project(sense C)

if (DEFINED PLATFORM)
    message("platform ${PLATFORM}")
    if (PLATFORM STREQUAL "OS64" )
        add_compile_definitions(TORNADO_OS_IOS)
        set(OS_IOS 1)
        set(OS_NAME "ios")
    endif()
elseif (APPLE)
    add_compile_definitions(TORNADO_OS_MACOS)
    set(OS_MACOS 1)
    set(OS_NAME "macos")
elseif (UNIX)
    add_compile_definitions(TORNADO_OS_LINUX)
    set(OS_LINUX 1)
    set(OS_NAME "linux")

elseif (WIN32)
    add_compile_definitions(TORNADO_OS_WINDOWS)
    set(OS_WINDOWS 1)
    set(OS_NAME "windows")
endif()


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
