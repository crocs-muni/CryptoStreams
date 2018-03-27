#include "idea_factory.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
namespace block {


    void idea_factory::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        ideaInit(&_ctx, key, keysize);
    }

    void idea_factory::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void idea_factory::encrypt(const std::uint8_t* plaintext,
                              std::uint8_t* ciphertext) {
        ideaEncryptBlock(&_ctx, plaintext, ciphertext, _rounds);
    }

    void idea_factory::decrypt(const std::uint8_t* ciphertext,
                              std::uint8_t* plaintext) {
        ideaDecryptBlock(&_ctx, ciphertext, plaintext, _rounds);
    }

}
