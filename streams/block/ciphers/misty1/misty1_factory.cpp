#include "misty1_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void misty1_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        _misty1.key_schedule(key, keysize);
    }

    void misty1_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void misty1_factory::encrypt(const std::uint8_t* plaintext,
                                 std::uint8_t* ciphertext) {
        _misty1.encrypt_n(plaintext, ciphertext, 1, _rounds);
    }

    void misty1_factory::decrypt(const std::uint8_t* ciphertext,
                                 std::uint8_t* plaintext) {
        _misty1.decrypt_n(ciphertext, plaintext, 1, _rounds);
    }

}