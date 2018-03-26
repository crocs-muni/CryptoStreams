//
// Created by Dusan Klinec on 25/03/2018.
//

#include "cast_block.h"

namespace block {
namespace cast {
    cast::cast(size_t rounds) : block_cipher(rounds) {

    }

    void cast::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != 16 && keysize != 10) {
            throw std::runtime_error("cast supports only keys 100, 128 bits");
        }
        CAST_set_key(&_ctx, (int) keysize, key);
    }

    void cast::ivsetup(const std::uint8_t *iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void cast::encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) {
        CAST_ecb_encrypt(plaintext, ciphertext, &_ctx, 1, static_cast<int>(_rounds));
    }

    void cast::decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) {
        CAST_ecb_encrypt(ciphertext, plaintext, &_ctx, 0, static_cast<int>(_rounds));
    }
} //namespace cast
} //namespace block
