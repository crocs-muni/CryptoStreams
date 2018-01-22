add_library(others STATIC EXCLUDE_FROM_ALL
    others_stream
    others_interface
    others_factory
    hash_functions/sha1/sha1
    hash_functions/sha1/sha1_factory
    )
target_link_libraries(others eacirc-core)

