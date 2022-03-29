include_directories(../deps/piot/breathe/src/include)

file(GLOB_RECURSE deps_src FOLLOW_SYMLINKS
        "${DEPS}*/src/*.c"
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


include_directories(${DEPS}burst/src/include)
include_directories(${DEPS}latch/src/include)
include_directories(${DEPS}scenery/src/include)
include_directories(${DEPS}swamp-runtime/src/include)
include_directories(${DEPS}swamp-script-scope/src/include)
include_directories(${DEPS}swamp-unpack-c/src/include)
include_directories(${DEPS}turmoil-resource/src/include)


        add_executable(sense_swamp_example
        ${deps_src}
        ${lib_src}
        examples/main.c
        platform/glfw/glfw_input_driver.c platform/glfw/glfw_input_manager.h platform/glfw/glfw_input_manager.c platform/glfw/glfw_keyboard.c platform/glfw/glfw_keyboard.h)


        find_package(glfw3 3.3 REQUIRED)

target_link_libraries(sense_swamp_example m)
target_link_libraries(sense_swamp_example glfw)
