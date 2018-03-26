cmake_minimum_required(VERSION 3.4)
project(eacirc-streams)

find_package(Git)

if (NOT EXISTS eacirc-core/CMakeLists.txt)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/eacirc-core/cmake)
include(build_stream)

# === Set CXX flags ===
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++14> -Wall -Wextra)
    add_compile_options(-fdiagnostics-color)
elseif (MSVC AND (MSVC_VERSION GREATER 1700))
    # nothing special yet
else()
    message(FATAL_ERROR "unsuported compiler id:${CMAKE_CXX_COMPILER_ID}, path: ${CMAKE_CXX_COMPILER}")
endif()


# === Provide sources as library
set(eacirc-streams-sources
        stream.h
        streams.h
        streams.cc
        )

add_library(eacirc-streams-lib STATIC
        ${eacirc-streams-sources}
        )

set_target_properties(eacirc-streams-lib PROPERTIES
        LINKER_LANGUAGE CXX
        )

target_link_libraries(eacirc-streams-lib eacirc-core)

add_subdirectory(eacirc-core)

add_subdirectory(streams/estream)
add_subdirectory(streams/sha3)
add_subdirectory(streams/block)

## Executables

option(BUILD_testsuite "Build all tests." OFF)

# === eacirc generator executable
add_executable(eacirc-streams main.cc generator)

set_target_properties(eacirc-streams PROPERTIES
        LINKER_LANGUAGE CXX
        )

target_link_libraries(eacirc-streams eacirc-core eacirc-streams-lib)

build_stream(eacirc-streams estream)
build_stream(eacirc-streams sha3)
build_stream(eacirc-streams block)

##############
# Building of testsuite
##############

if (BUILD_testsuite)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    # === testsuite executable
    add_executable(testsuite
            ${eacirc-streams-sources}
            testsuite/test_main.cc
            testsuite/stream_tests.cc
            testsuite/sha3_streams_tests.cc
            testsuite/estream_streams_tests.cc
            testsuite/block_streams_tests.cc
            testsuite/test-utils/test_streams
            testsuite/test-utils/sha3_test_case
            testsuite/test-utils/estream_test_case
            testsuite/test-utils/block_test_case
            testsuite/test-utils/common_functions
            testsuite/test-utils/test_case.h)

    target_compile_definitions(testsuite PUBLIC "TEST_STREAM=1")

    file(COPY testsuite/test-resources DESTINATION resources)

    # Standard linking to gtest stuff.
    target_link_libraries(testsuite gtest gtest_main)

    # Extra linking for the project.
    target_link_libraries(testsuite eacirc-core)

    build_stream(testsuite estream)
    build_stream(testsuite sha3)
    build_stream(testsuite block)
    add_definitions(-DBUILD_testsuite)

endif()
