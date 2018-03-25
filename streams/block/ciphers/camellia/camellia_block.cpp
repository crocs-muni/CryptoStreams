//
// Created by Dusan Klinec on 25/03/2018.
//

#include <streams/block/ciphers/gost/gost89.h>
#include "camellia_block.h"

namespace block {
namespace camellia {
    camellia::camellia(size_t rounds, bool enc) : block_cipher(rounds) {
        this->_enc = enc;
        mbedtls_camellia_init(&_ctx);
    }

    void camellia::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != 16 && keysize != 24 && keysize != 32) {
            throw std::runtime_error("Camellia supports only keys 128, 192, 256 bits");
        }
        if (_enc) {
            mbedtls_camellia_setkey_enc(&_ctx, key, static_cast<unsigned int>(keysize * 8));
        } else {
            mbedtls_camellia_setkey_dec(&_ctx, key, static_cast<unsigned int>(keysize * 8));
        }
    }

    void camellia::ivsetup(const std::uint8_t *iv, const std::uint64_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void camellia::encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) {
        if (!_enc){
            throw std::runtime_error("Camellia not initialized for encryption");
        }
        mbedtls_camellia_crypt_ecb(&_ctx, 0, plaintext, ciphertext, static_cast<unsigned int>(_rounds));
    }

    void camellia::decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) {
        if (_enc){
            throw std::runtime_error("Camellia not initialized for decryption");
        }
        mbedtls_camellia_crypt_ecb(&_ctx, 0, ciphertext, plaintext, static_cast<unsigned int>(_rounds));
    }
} //namespace camellia
} //namespace block
