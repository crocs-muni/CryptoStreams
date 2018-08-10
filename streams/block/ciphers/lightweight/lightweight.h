//
// Created by mhajas on 7/6/18.
//

#ifndef CRYPTO_STREAMS_LIGHTWEIGHT_H
#define CRYPTO_STREAMS_LIGHTWEIGHT_H

#include <streams/block/block_cipher.h>
#include <stdexcept>
#include <algorithm>
#include <string>

namespace block {

    template <size_t KEY_SIZE, size_t BLOCK_SIZE>
    class lightweight : public block_cipher {
    public:
        lightweight(size_t rounds) : block_cipher(rounds) {};

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override {
            if (keysize != KEY_SIZE) {
                throw std::runtime_error("Used lightweight function only support key size: " + std::to_string(KEY_SIZE));
            }

            std::copy_n(key, keysize, _key);
        }

        void ivsetup(const std::uint8_t *iv, const std::uint64_t ivsize) override {
            throw std::runtime_error("IVs are not implemented for lightweight algorithms");
        }

        void encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) override {
            std::copy_n(plaintext, BLOCK_SIZE, ciphertext);
            Encrypt(ciphertext);
        }

        void decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) override {
            std::copy_n(ciphertext, BLOCK_SIZE, plaintext);
            Decrypt(plaintext);
        }

        virtual void Encrypt(uint8_t *block) = 0;
        virtual void Decrypt(uint8_t *block) = 0;

    protected:
        uint8_t _key[KEY_SIZE];
    };

} // namespace


#endif //CRYPTO_STREAMS_LIGHTWEIGHT_H
