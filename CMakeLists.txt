cmake_minimum_required(VERSION 3.4)
project(crypto-streams)

find_package(Git)

if (NOT EXISTS eacirc-core/CMakeLists.txt)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
    else()
        message(STATUS "Git not found, please make sure eacirc-core submodule is updated")
    endif()
endif()

if(BUILD_TAG)
    message(STATUS "Building build tag ${BUILD_TAG}")
    add_definitions("-DBUILD_TAG=${BUILD_TAG}")
else()
    message(STATUS "Building without build tag")
endif()

if(APPLE)
    include_directories(SYSTEM /usr/include/malloc)
    if(POLICY CMP0042)
        cmake_policy(SET CMP0042 NEW)
    endif()
endif()


# option(STATIC "Link libraries statically" ON)
option(BUILD_testsuite "Build all tests." OFF)
set(STATIC ON)

if (BUILD_testsuite)
    add_definitions(-DBUILD_testsuite)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/eacirc-core/cmake)
include(build_stream)
include(detect_version)
include(static)
include(portable_randomness)
detect_version(version.h)

# === Set CXX flags ===
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++14> -Wall -Wextra)
    add_compile_options(-fdiagnostics-color)
elseif (MSVC AND (MSVC_VERSION GREATER 1700))
    # nothing special yet
else()
    message(FATAL_ERROR "unsupported compiler id:${CMAKE_CXX_COMPILER_ID}, path: ${CMAKE_CXX_COMPILER}")
endif()

add_subdirectory(eacirc-core)

# === Provide sources as library
set(crypto-streams-sources
        stream.h
        streams.h
        streams.cc
        )

add_library(crypto-streams-lib STATIC
        ${crypto-streams-sources}
        )

set_target_properties(crypto-streams-lib PROPERTIES
        LINKER_LANGUAGE CXX
        )

target_link_libraries(crypto-streams-lib
        eacirc-core
        ${EXTRA_LIBRARIES})

add_subdirectory(streams/stream_ciphers)
add_subdirectory(streams/hash)
add_subdirectory(streams/block)
add_subdirectory(streams/prngs)

## Executables

# === eacirc generator executable
add_executable(crypto-streams main.cc generator)

set_target_properties(crypto-streams PROPERTIES
        LINKER_LANGUAGE CXX
        )

target_link_libraries(crypto-streams
        eacirc-core
        crypto-streams-lib
        ${EXTRA_LIBRARIES})

build_stream(crypto-streams stream_ciphers)
build_stream(crypto-streams hash)
build_stream(crypto-streams block)
build_stream(crypto-streams prngs)

##############
# Building of testsuite
##############

if (BUILD_testsuite)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    # === testsuite executable
    add_executable(testsuite
            ${crypto-streams-sources}
            testsuite/test_main.cc
            testsuite/stream_tests.cc
            testsuite/hash_streams_tests.cc
            testsuite/stream_ciphers_streams_tests.cc
            testsuite/block_streams_tests.cc
            testsuite/testu01_prng_tests.cc
            testsuite/std_prng_tests.cc
            testsuite/test_utils/test_streams
            testsuite/test_utils/hash_test_case
            testsuite/test_utils/stream_ciphers_test_case
            testsuite/test_utils/block_test_case
            testsuite/test_utils/common_functions
            testsuite/test_utils/test_case.h)

    target_compile_definitions(testsuite PUBLIC "TEST_STREAM=1")

    file(COPY testsuite/test_resources DESTINATION resources)

    # Standard linking to gtest stuff.
    target_link_libraries(testsuite
            gtest
            gtest_main
            ${EXTRA_LIBRARIES})

    # Extra linking for the project.
    target_link_libraries(testsuite
            eacirc-core
            ${EXTRA_LIBRARIES})

    build_stream(testsuite stream_ciphers)
    build_stream(testsuite hash)
    build_stream(testsuite block)
    build_stream(testsuite prngs)

endif()
