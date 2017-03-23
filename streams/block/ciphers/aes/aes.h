#pragma once

/**
 * Source: https://github.com/kokke/tiny-AES128-C
 */

#include "../../block_cipher.h"

namespace block {

    class aes : public block_cipher {

        /* Data structures */

        struct aes_ctx {
            aes_ctx()
                : key{0} {}

            uint8_t key[16];
        } _ctx;

    public:
        aes(unsigned rounds)
            : block_cipher(rounds) {}

        void keysetup(const std::uint8_t* key, const std::uint32_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const std::uint32_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    };
}
