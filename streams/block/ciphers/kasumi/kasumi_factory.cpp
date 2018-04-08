#include "kasumi_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void kasumi_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        _kasumi.key_schedule(key, keysize);
    }

    void kasumi_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void kasumi_factory::encrypt(const std::uint8_t* plaintext,
                               std::uint8_t* ciphertext) {
        _kasumi.encrypt_n(plaintext, ciphertext, 1, _rounds);
    }

    void kasumi_factory::decrypt(const std::uint8_t* ciphertext,
                               std::uint8_t* plaintext) {
        _kasumi.decrypt_n(ciphertext, plaintext, 1, _rounds);
    }

}