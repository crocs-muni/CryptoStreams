#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "kuznyechik_factory.h"

namespace block {


    void kuznyechik_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        kuzn_set_key(&_ctx, key);
    }

    void kuznyechik_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void kuznyechik_factory::encrypt(const std::uint8_t* plaintext,
                                   std::uint8_t* ciphertext) {
        kuzn_encrypt(&_ctx, plaintext, ciphertext/*, unsigned(_rounds)*/);
    }

    void kuznyechik_factory::decrypt(const std::uint8_t* ciphertext,
                                   std::uint8_t* plaintext) {
        kuzn_decrypt(&_ctx, ciphertext, plaintext/*, _rounds*/);
    }
}
