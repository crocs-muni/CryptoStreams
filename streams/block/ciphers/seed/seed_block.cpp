//
// Created by Dusan Klinec on 25/03/2018.
//

#include "seed_block.h"

namespace block {
namespace seed {
    seed::seed(size_t rounds) : block_cipher(rounds) {

    }

    void seed::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != 16) {
            throw std::runtime_error("seed supports only keys 128 bits");
        }
        SEED_set_key(key, &_ctx);
    }

    void seed::ivsetup(const std::uint8_t *iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void seed::encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) {
        SEED_encrypt(plaintext, ciphertext, &_ctx, static_cast<int>(_rounds));
    }

    void seed::decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) {
        SEED_decrypt(ciphertext, plaintext, &_ctx, static_cast<int>(_rounds));
    }
} //namespace seed
} //namespace block
