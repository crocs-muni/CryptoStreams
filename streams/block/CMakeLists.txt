add_library(block STATIC EXCLUDE_FROM_ALL
    block_stream
    block_cipher
    block_factory
    ciphers/common_fun.h
    # === eSTREAM cipher files ===
    ciphers/tea/tea
    ciphers/aes/aes
    ciphers/aria/aria
    ciphers/aria/aria_block
    ciphers/camellia/camellia
    ciphers/camellia/camellia_block
    ciphers/gost/gost89
    ciphers/gost/gost_block
    ciphers/seed/seed
    ciphers/seed/seed_block
    ciphers/simon/simon
    ciphers/speck/speck
    ciphers/rc4/rc4
    ciphers/des/des
    ciphers/des/single_des
    ciphers/des/triple_des    
    ciphers/blowfish/blowfish
    ciphers/blowfish/blowfish_factory
    ciphers/idea/idea
    ciphers/idea/idea_block
    ciphers/cast/cast
    )

target_link_libraries(block eacirc-core)
