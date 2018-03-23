//
// Created by Dusan Klinec on 23/03/2018.
//

#include "gost_block.h"
#include <stdexcept>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

namespace block {
    gost::gost(size_t rounds) : block_cipher(rounds) {
        gost_init((&this->_ctx), &GostR3411_94_TestParamSet);
    }

    void gost::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
        if (keysize != 32){
            throw std::runtime_error("Gost keysize has to be exactly 32 B");
        }
        gost_key(&_ctx, key);
    }

    void gost::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void gost::encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext) {
        gost_enc(&_ctx, plaintext, ciphertext, 1, static_cast<unsigned int>(_rounds));
    }

    void gost::decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext) {
        gost_dec(&_ctx, ciphertext, plaintext, 1, static_cast<unsigned int>(_rounds));
    }
} //namespace block

#pragma clang diagnostic pop