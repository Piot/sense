cmake_minimum_required(VERSION 3.17.3)
project(sense_swamp_example C)

set(CMAKE_C_STANDARD 11)
add_compile_definitions(_POSIX_C_SOURCE=200112L TORNADO_OS_LINUX CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)

include_directories(include)

include_directories(../deps/basal-c/src/include)
include_directories(../deps/clog/src/include)
include_directories(../deps/tiny-libc/src/include)
include_directories(../deps/imprint/src/include)
include_directories(../deps/sense/src/include)
include_directories(../deps/breathe/src/include)
include_directories(../deps/burst/src/include)
include_directories(../deps/latch/src/include)
include_directories(../deps/scenery/src/include)
include_directories(../deps/swamp-runtime/src/include)
include_directories(../deps/swamp-script-scope/src/include)
include_directories(../deps/swamp-unpack-c/src/include)
include_directories(../deps/turmoil-resource/src/include)

file(GLOB_RECURSE deps_src FOLLOW_SYMLINKS
        "../deps/*/src/*.c"
        )

# Remove iOS and SDL (including Webassembly)
list(FILTER deps_src EXCLUDE REGEX ".*sdl_main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*webassembly/main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/examples/main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/example/main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/ios/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/posix/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/webassembly/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/sdl/.*" )


file(GLOB_RECURSE lib_src FOLLOW_SYMLINKS
        "lib/*.c"
        )


add_executable(sense_swamp_example
        ${deps_src}
        ${lib_src}
        examples/main.c
        platform/glfw/glfw_input_driver.c platform/glfw/glfw_input_manager.h platform/glfw/glfw_input_manager.c platform/glfw/glfw_keyboard.c platform/glfw/glfw_keyboard.h)

#find_package (Vulkan)
#if (Vulkan_FOUND)
    #include_directories(${Vulkan_INCLUDE_DIRS})
    #target_link_libraries (sense_swamp_example ${Vulkan_LIBRARIES})
#endif (Vulkan_FOUND)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(sense_swamp_example m)
target_link_libraries(sense_swamp_example glfw)
target_link_libraries(sense_swamp_example -fsanitize=address)

