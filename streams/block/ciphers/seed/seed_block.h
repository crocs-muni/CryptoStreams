#pragma once

#include "../../block_cipher.h"
#include "seed.h"

// Rounds are key-size dependent
#define SEED_BLOCK_FULL_ROUNDS 16

namespace block {
namespace seed {
    class seed : public block_cipher {
        SEED_KEY_SCHEDULE _ctx;

    public:
        seed(std::size_t rounds = SEED_BLOCK_FULL_ROUNDS);

        void keysetup(const std::uint8_t *key, std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t *iv, std::uint64_t ivsize) override;

        void encrypt(const std::uint8_t *plaintext,
                     std::uint8_t *ciphertext) override;

        void decrypt(const std::uint8_t *ciphertext,
                     std::uint8_t *plaintext) override;
    };
}
}
