add_library(prngs STATIC EXCLUDE_FROM_ALL
        prng_factory
        prng_stream.cc
        prng_stream.h
        prng_interface.h)

add_subdirectory(std-prngs)
add_subdirectory(testu01-prngs)

build_stream(prngs std-prngs)
build_stream(prngs testu01-prngs)

target_link_libraries(prngs eacirc-core)
