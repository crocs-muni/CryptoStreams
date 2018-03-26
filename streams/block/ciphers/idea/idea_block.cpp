//
// Created by Dusan Klinec on 25/03/2018.
//

#include "idea_block.h"

namespace block {
namespace idea {
    idea::idea(size_t rounds, bool enc) : block_cipher(rounds) {
        this->_enc = enc;
    }

    void idea::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != 16) {
            throw std::runtime_error("idea supports only keys 128 bits");
        }
        if (_enc) {
            IDEA_set_encrypt_key(key, &_ctx);
        } else {
            IDEA_KEY_SCHEDULE tmp{};
            IDEA_set_encrypt_key(key, &tmp);
            IDEA_set_decrypt_key(&tmp, &_ctx);
        }
    }

    void idea::ivsetup(const std::uint8_t *iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void idea::encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) {
        if (!_enc){
            throw std::runtime_error("Camellia not initialized for encryption");
        }
        IDEA_ecb_encrypt(plaintext, ciphertext, &_ctx, static_cast<int>(_rounds));
    }

    void idea::decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) {
        if (_enc){
            throw std::runtime_error("Camellia not initialized for decryption");
        }
        IDEA_ecb_encrypt(ciphertext, plaintext, &_ctx, static_cast<int>(_rounds));
    }
} //namespace idea
} //namespace block
