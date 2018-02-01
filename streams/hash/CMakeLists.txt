add_subdirectory(others)
add_subdirectory(sha3)

add_library(hash STATIC EXCLUDE_FROM_ALL
    hash_stream
    hash_interface
    hash_factory
    )

target_link_libraries(hash eacirc-core others sha3)
