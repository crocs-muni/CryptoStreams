//
// Created by Dusan Klinec on 25/03/2018.
//

#include "aria_block.h"

namespace block {
namespace aria {
    aria::aria(size_t rounds, bool enc) : block_cipher(rounds) {
        this->_enc = enc;
    }

    void aria::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != 16 && keysize != 24 && keysize != 32) {
            throw std::runtime_error("Aria supports only keys 128, 192, 256 bits");
        }
        if (_enc) {
            aria_set_encrypt_key(key, static_cast<const int>(keysize * 8), &_ctx);
        } else {
            aria_set_decrypt_key(key, static_cast<const int>(keysize * 8), &_ctx);
        }
    }

    void aria::ivsetup(const std::uint8_t *iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void aria::encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) {
        if (!_enc){
            throw std::runtime_error("aria not initialized for encryption");
        }
        //_ctx.rounds = static_cast<unsigned int>(_rounds);
        aria_encrypt(plaintext, ciphertext, &_ctx, static_cast<unsigned int>(_rounds), true);
    }

    void aria::decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) {
        if (_enc){
            throw std::runtime_error("aria not initialized for decryption");
        }
        //_ctx.rounds = static_cast<unsigned int>(_rounds);
        aria_encrypt(ciphertext, plaintext, &_ctx, static_cast<unsigned int>(_rounds), false);
    }
} //namespace aria
} //namespace block
