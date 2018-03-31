#include "noekeon_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void noekeon_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        _noekeon.key_schedule(key, keysize);
    }

    void noekeon_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void noekeon_factory::encrypt(const std::uint8_t* plaintext,
                                 std::uint8_t* ciphertext) {
        _noekeon.encrypt_n(plaintext, ciphertext, 1, _rounds);
    }

    void noekeon_factory::decrypt(const std::uint8_t* ciphertext,
                                 std::uint8_t* plaintext) {
        _noekeon.decrypt_n(ciphertext, plaintext, 1, _rounds);
    }

}