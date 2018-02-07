add_library(others STATIC EXCLUDE_FROM_ALL
    others_factory.h
    others_factory.cc
    hash_functions/hash_functions.h
    hash_functions/sha1/sha1
    hash_functions/sha1/sha1_factory
    hash_functions/sha2/sha256
    hash_functions/sha2/sha256_factory
    hash_functions/md5/md5
    hash_functions/md5/md5_factory
    )
target_link_libraries(others eacirc-core)

