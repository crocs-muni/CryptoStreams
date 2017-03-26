cmake_minimum_required(VERSION 3.4)
project(eacirc-streams)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/eacirc-core/cmake)
include(build_stream)

# === Set CXX flags ===
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++11> -Wall -Wextra)
    add_compile_options(-fdiagnostics-color)
elseif (MSVC AND (MSVC_VERSION GREATER 1700))
    # nothing special yet
else()
    message(FATAL_ERROR "unsuported compiler id:${CMAKE_CXX_COMPILER_ID}, path: ${CMAKE_CXX_COMPILER}")
endif()

add_executable(eacirc-streams main.cc
        generator
        stream.h
        streams.cc
        streams.h
        )

set_target_properties(eacirc-streams PROPERTIES
        LINKER_LANGUAGE CXX
        )

add_subdirectory(eacirc-core)

add_subdirectory(streams/estream)
add_subdirectory(streams/sha3)
add_subdirectory(streams/block)

target_link_libraries(eacirc-streams eacirc-core)

build_stream(eacirc-streams estream)
build_stream(eacirc-streams sha3)
build_stream(eacirc-streams block)

# === Provide sources as Library ===
add_library(eacirc-streams-lib STATIC
        stream
        streams.h
        streams.cc
        )

set_target_properties(eacirc-streams-lib PROPERTIES
        LINKER_LANGUAGE CXX
        )

target_link_libraries(eacirc-streams-lib eacirc-core)