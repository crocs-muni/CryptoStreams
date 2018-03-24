#include "twofish_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void twofish_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        _twofish.PrepareKey(key, keysize, &_key);
    }

    void twofish_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void twofish_factory::encrypt(const std::uint8_t* plaintext,
                 std::uint8_t* ciphertext) {
        _twofish.Encrypt_modified(&_key, plaintext, ciphertext, unsigned(_rounds));
    }

    void twofish_factory::decrypt(const std::uint8_t* ciphertext,
                 std::uint8_t* plaintext) {
        _twofish.Decrypt(&_key, ciphertext, plaintext); // for all 16 rounds
}
}
