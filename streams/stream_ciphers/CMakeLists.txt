add_library(stream_ciphers STATIC EXCLUDE_FROM_ALL
    stream_cipher
    stream_interface
    stream_stream
    # === eSTREAM cipher files ===
    estream/ecrypt-config.h
    estream/ecrypt-machine.h
    estream/ecrypt-portable.h
    estream/zk-crypt/ecrypt-sync.h
    estream/zk-crypt/ZKdef.h
    estream/zk-crypt/ZKengine.h
    estream/yamb/ecrypt-sync.h
    estream/wg/ecrypt-sync.h
    estream/tsc-4/ecrypt-sync.h
    estream/trivium/ecrypt-sync.h
    estream/sosemanuk/ecrypt-sync.h
    estream/sosemanuk/sosemanuk.h
    estream/sfinks/ecrypt-sync.h
    estream/salsa20/ecrypt-sync.h
    estream/rabbit/ecrypt-sync.h
    estream/py/ecrypt-sync.h
    estream/pomaranch/ecrypt-sync.h
    estream/pomaranch/pomaranch.h
    estream/mir-1/ecrypt-sync.h
    estream/mir-1/mir1.h
    estream/mickey/ecrypt-sync.h
    estream/mag/ecrypt-sync.h
    estream/mag/unrolliv.h
    estream/mag/unrollmain.h
    estream/lex/ecrypt-sync.h
    estream/hermes/ecrypt-sync.h
    estream/hc-128/ecrypt-sync.h
    estream/grain/ecrypt-sync.h
    estream/grain/grain-128.h
    estream/fubuki/ecrypt-sync.h
    estream/ffcsr/ecrypt-sync.h
    estream/ffcsr/ffcsrh-sync.h
    estream/edon80/ecrypt-sync.h
    estream/dragon/ecrypt-sync.h
    estream/dicing/ecrypt-sync.h
    estream/decim/decimv2.h
    estream/decim/ecrypt-sync.h
    estream/cryptmt/ecrypt-sync.h
    estream/cryptmt/params.h
    estream/achterbahn/achterbahn.h
    estream/achterbahn/ecrypt-sync.h
    estream/abc/abc.h
    estream/abc/abc-tables.h
    estream/abc/ecrypt-sync.h
    #    estream/polarbear/aes.h           # not implemented in EncryptorDecryptor
    #    estream/polarbear/aesopt.h        # not implemented in EncryptorDecryptor
    #    estream/polarbear/ecrypt-sync.h   # not implemented in EncryptorDecryptor
    #    estream/nls/ecrypt-sync.h         # not implemented in EncryptorDecryptor
    #    estream/nls/ecrypt-sync-ae.h      # not implemented in EncryptorDecryptor
    #    estream/nls/nls.h                 # not implemented in EncryptorDecryptor
    #    estream/nls/nlsmultab.h           # not implemented in EncryptorDecryptor
    #    estream/nls/nlssbox.h             # not implemented in EncryptorDecryptor
    #    estream/moustique/ecrypt-ssyn.h   # not implemented in EncryptorDecryptor
    #    estream/decim/decim-128.h           # do not include!
    # not used (but included):
    #    estream/zk-crypt/ecrypt-sync.h    # not used
    #    estream/zk-crypt/ZKdef.h          # not used
    #    estream/zk-crypt/ZKengine.h       # not used
    #    estream/cryptmt/ecrypt-sync.h     # not used
    #    estream/cryptmt/params.h          # not used
    #    estream/abc/abc.h                 # not used
    #    estream/abc/abc-tables.h          # not used
    #    estream/abc/ecrypt-sync.h         # not used
    estream/zk-crypt/zk-crypt-v3.cpp
    estream/wg/wg.cpp
    estream/tsc-4/tsc-4.cpp
    estream/sosemanuk/sosemanuk.cpp
    estream/sfinks/sfinks.cpp
    estream/salsa20/salsa20.cpp
    estream/rabbit/rabbit.cpp
    estream/py/py6.cpp
    estream/pomaranch/pomaranch.cpp
    estream/mir-1/mir-1.cpp
    estream/mickey/mickey-128-v2.cpp
    estream/mag/mag.cpp
    estream/lex/lex.cpp
    estream/hermes/hermes.cpp
    estream/hc-128/hc-128.cpp
    estream/grain/grain-128.cpp
    estream/fubuki/fubuki.cpp
    estream/ffcsr/f-fcsr-h.cpp
    estream/edon80/edon80.cpp
    estream/dragon/dragon.cpp
    estream/dragon/dragon-sboxes.cpp
    estream/dicing/dicing-v2.cpp
    estream/decim/decim-v2.cpp
    estream/cryptmt/cryptmt-v3.cpp
    estream/achterbahn/achterbahn-128-80.cpp
    estream/abc/abc-v3.cpp
    estream/trivium/trivium.cpp
    #    estream/yamb/yamb.cpp             # stopped working after IDE update
    #    estream/polarbear/polar-bear.cpp  # not implemented in EncryptorDecryptor
    #    estream/polarbear/aescrypt.cpp    # do not include!
    #    estream/polarbear/aestab.cpp      # do not include!
    #    estream/polarbear/whirltab.cpp    # do not include!
    #    estream/nls/nlsref.cpp            # not implemented in EncryptorDecryptor
    #    estream/moustique/moustique.cpp   # not implemented in EncryptorDecryptor
    #    estream/cryptmt/altivec.cpp       # do not include!
    #    estream/cryptmt/sse2.cpp          # do not include!
    #    estream/decim/decim-128.c         # do not include!

    other/chacha/chacha
    other/rc4/rc4
    )

target_link_libraries(stream_ciphers eacirc-core)
