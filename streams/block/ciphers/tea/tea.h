#pragma once

/**
 * Source: https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
 */

#include "../../block_cipher.h"

namespace block {

    class tea : public block_cipher {

        /* Data structures */

        struct tea_ctx {

            tea_ctx()
                : key{0} {}

            std::uint32_t key[4];
        } _ctx;

    public:
        tea(unsigned rounds)
            : block_cipher(rounds) {}

        void keysetup(const std::uint8_t* key, const uint32_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const uint32_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    };
}
