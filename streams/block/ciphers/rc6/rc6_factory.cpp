
#include "rc6_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void rc6_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        rc6Init(&_ctx, key, keysize, _rounds);
    }

    void rc6_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void rc6_factory::encrypt(const std::uint8_t* plaintext,
                                   std::uint8_t* ciphertext) {
        rc6EncryptBlock(&_ctx, plaintext, ciphertext, _rounds);
    }

    void rc6_factory::decrypt(const std::uint8_t* ciphertext,
                                   std::uint8_t* plaintext) {
        rc6DecryptBlock(&_ctx, ciphertext, plaintext, _rounds);
    }

}
