include(cmake/build_gen_stream.cmake)
include(cmake/detect_version.cmake)

add_executable(generator main.cc
    generator
    ../eacirc/seed
    ../eacirc/streams
    )

target_link_libraries(generator core)

add_subdirectory(streams/estream)
add_subdirectory(streams/sha3)
add_subdirectory(streams/block)

build_gen_stream(estream)
build_gen_stream(sha3)
build_gen_stream(block)
