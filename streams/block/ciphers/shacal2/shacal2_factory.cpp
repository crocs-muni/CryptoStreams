#include "shacal2_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void shacal2_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        _shacal2.key_schedule(key, keysize);
    }

    void shacal2_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void shacal2_factory::encrypt(const std::uint8_t* plaintext,
                                 std::uint8_t* ciphertext) {
        _shacal2.encrypt_n(plaintext, ciphertext, 1, _rounds);
    }

    void shacal2_factory::decrypt(const std::uint8_t* ciphertext,
                                 std::uint8_t* plaintext) {
        _shacal2.decrypt_n(ciphertext, plaintext, 1, _rounds);
    }

}