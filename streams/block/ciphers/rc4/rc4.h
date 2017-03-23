#pragma once

/**
 * Source: https://github.com/B-Con/crypto-algorithms
 */

#include "../../block_cipher.h"
#include <memory>

namespace block {

    class rc4 : public block_cipher {

        /* Data structures */

        struct rc4_ctx {

            rc4_ctx()
                : state{0}
                , key_size(0) {}

            std::uint8_t state[256];
            std::unique_ptr<std::uint8_t[]> key;
            std::uint32_t key_size;
        } _ctx;

        unsigned _block_size;

    public:
        rc4(unsigned rounds, unsigned block_size)
            : block_cipher(rounds)
            , _block_size(block_size) {}

        void keysetup(const std::uint8_t* key, const std::uint32_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const std::uint32_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    };
}
