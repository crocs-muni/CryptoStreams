add_library(estream STATIC EXCLUDE_FROM_ALL
    estream_cipher
    estream_interface
    estream_stream
    # === eSTREAM cipher files ===
    ciphers/ecrypt-config.h
    ciphers/ecrypt-machine.h
    ciphers/ecrypt-portable.h
    ciphers/zk-crypt/ecrypt-sync.h
    ciphers/zk-crypt/ZKdef.h
    ciphers/zk-crypt/ZKengine.h
    ciphers/yamb/ecrypt-sync.h
    ciphers/wg/ecrypt-sync.h
    ciphers/tsc-4/ecrypt-sync.h
    ciphers/trivium/ecrypt-sync.h
    ciphers/tea/ecrypt-sync.h
    ciphers/sosemanuk/ecrypt-sync.h
    ciphers/sosemanuk/sosemanuk.h
    ciphers/sfinks/ecrypt-sync.h
    ciphers/salsa20/ecrypt-sync.h
    ciphers/rabbit/ecrypt-sync.h
    ciphers/py/ecrypt-sync.h
    ciphers/pomaranch/ecrypt-sync.h
    ciphers/pomaranch/pomaranch.h
    ciphers/mir-1/ecrypt-sync.h
    ciphers/mir-1/mir1.h
    ciphers/mickey/ecrypt-sync.h
    ciphers/mag/ecrypt-sync.h
    ciphers/mag/unrolliv.h
    ciphers/mag/unrollmain.h
    ciphers/lex/ecrypt-sync.h
    ciphers/hermes/ecrypt-sync.h
    ciphers/hc-128/ecrypt-sync.h
    ciphers/grain/ecrypt-sync.h
    ciphers/grain/grain-v1.h
    ciphers/fubuki/ecrypt-sync.h
    ciphers/ffcsr/ecrypt-sync.h
    ciphers/ffcsr/ffcsrh-sync.h
    ciphers/edon80/ecrypt-sync.h
    ciphers/dragon/ecrypt-sync.h
    ciphers/dicing/ecrypt-sync.h
    ciphers/decim/decimv2.h
    ciphers/decim/ecrypt-sync.h
    ciphers/cryptmt/ecrypt-sync.h
    ciphers/cryptmt/params.h
    ciphers/achterbahn/achterbahn.h
    ciphers/achterbahn/ecrypt-sync.h
    ciphers/abc/abc.h
    ciphers/abc/abc-tables.h
    ciphers/abc/ecrypt-sync.h
    #    ciphers/polarbear/aes.h           # not implemented in EncryptorDecryptor
    #    ciphers/polarbear/aesopt.h        # not implemented in EncryptorDecryptor
    #    ciphers/polarbear/ecrypt-sync.h   # not implemented in EncryptorDecryptor
    #    ciphers/nls/ecrypt-sync.h         # not implemented in EncryptorDecryptor
    #    ciphers/nls/ecrypt-sync-ae.h      # not implemented in EncryptorDecryptor
    #    ciphers/nls/nls.h                 # not implemented in EncryptorDecryptor
    #    ciphers/nls/nlsmultab.h           # not implemented in EncryptorDecryptor
    #    ciphers/nls/nlssbox.h             # not implemented in EncryptorDecryptor
    #    ciphers/moustique/ecrypt-ssyn.h   # not implemented in EncryptorDecryptor
    #    ciphers/decim/decim-128.h           # do not include!
    # not used (but included):
    #    ciphers/zk-crypt/ecrypt-sync.h    # not used
    #    ciphers/zk-crypt/ZKdef.h          # not used
    #    ciphers/zk-crypt/ZKengine.h       # not used
    #    ciphers/cryptmt/ecrypt-sync.h     # not used
    #    ciphers/cryptmt/params.h          # not used
    #    ciphers/abc/abc.h                 # not used
    #    ciphers/abc/abc-tables.h          # not used
    #    ciphers/abc/ecrypt-sync.h         # not used
    ciphers/zk-crypt/zk-crypt-v3.cpp
    ciphers/wg/wg.cpp
    ciphers/tsc-4/tsc-4.cpp
    ciphers/tea/tea.cpp
    ciphers/sosemanuk/sosemanuk.cpp
    ciphers/sfinks/sfinks.cpp
    ciphers/salsa20/salsa20.cpp
    ciphers/rabbit/rabbit.cpp
    ciphers/py/py6.cpp
    ciphers/pomaranch/pomaranch.cpp
    ciphers/mir-1/mir-1.cpp
    ciphers/mickey/mickey-128-v2.cpp
    ciphers/mag/mag.cpp
    ciphers/lex/lex.cpp
    ciphers/hermes/hermes.cpp
    ciphers/hc-128/hc-128.cpp
    ciphers/grain/grain-v1.cpp
    ciphers/fubuki/fubuki.cpp
    ciphers/ffcsr/f-fcsr-h.cpp
    ciphers/edon80/edon80.cpp
    ciphers/dragon/dragon.cpp
    ciphers/dragon/dragon-sboxes.cpp
    ciphers/dicing/dicing-v2.cpp
    ciphers/decim/decim-v2.cpp
    ciphers/cryptmt/cryptmt-v3.cpp
    ciphers/achterbahn/achterbahn-128-80.cpp
    ciphers/abc/abc-v3.cpp
    #    ciphers/trivium/trivium.cpp       # stopped working after IDE update
    #    ciphers/yamb/yamb.cpp             # stopped working after IDE update
    #    ciphers/polarbear/polar-bear.cpp  # not implemented in EncryptorDecryptor
    #    ciphers/polarbear/aescrypt.cpp    # do not include!
    #    ciphers/polarbear/aestab.cpp      # do not include!
    #    ciphers/polarbear/whirltab.cpp    # do not include!
    #    ciphers/nls/nlsref.cpp            # not implemented in EncryptorDecryptor
    #    ciphers/moustique/moustique.cpp   # not implemented in EncryptorDecryptor
    #    ciphers/cryptmt/altivec.cpp       # do not include!
    #    ciphers/cryptmt/sse2.cpp            # do not include!
    #    ciphers/decim/decim-128.c         # do not include!
    )
target_link_libraries(estream core)
