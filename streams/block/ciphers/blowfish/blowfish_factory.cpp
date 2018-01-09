#include "blowfish_factory.h"
#include "blowfish.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void blowfish_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        blowfish_key_setup(key, &_keystruct, keysize, unsigned(_rounds));
    }

    void blowfish_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void blowfish_factory::encrypt(const std::uint8_t* plaintext,
                 std::uint8_t* ciphertext) {
        blowfish_encrypt(plaintext, ciphertext, &_keystruct, unsigned(_rounds));
    }

    void blowfish_factory::decrypt(const std::uint8_t* ciphertext,
                 std::uint8_t* plaintext) {
        blowfish_decrypt(ciphertext, plaintext, &_keystruct, unsigned(_rounds));
}
}
