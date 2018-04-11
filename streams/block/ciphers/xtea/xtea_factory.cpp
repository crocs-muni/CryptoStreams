#include "xtea_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void xtea_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        _xtea.key_schedule(key, keysize);
    }

    void xtea_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void xtea_factory::encrypt(const std::uint8_t* plaintext,
                              std::uint8_t* ciphertext) {
        _xtea.encrypt_n(plaintext, ciphertext, 1, _rounds);
    }

    void xtea_factory::decrypt(const std::uint8_t* ciphertext,
                              std::uint8_t* plaintext) {
        _xtea.decrypt_n(ciphertext, plaintext, 1, _rounds);
    }

}