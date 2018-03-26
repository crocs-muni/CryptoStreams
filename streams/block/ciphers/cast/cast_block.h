#pragma once

#include "../../block_cipher.h"
#include "cast.h"

// Rounds are key-size dependent
#define CAST_BLOCK_FULL_ROUNDS 16

namespace block {
namespace cast {
    class cast : public block_cipher {
        CAST_KEY _ctx;

    public:
        cast(std::size_t rounds = CAST_BLOCK_FULL_ROUNDS);

        void keysetup(const std::uint8_t *key, std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t *iv, std::uint64_t ivsize) override;

        void encrypt(const std::uint8_t *plaintext,
                     std::uint8_t *ciphertext) override;

        void decrypt(const std::uint8_t *ciphertext,
                     std::uint8_t *plaintext) override;
    };
}
}
