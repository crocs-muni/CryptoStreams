#pragma once

#include "../../block_cipher.h"
#include "idea.h"

// Rounds are key-size dependent
#define IDEA_BLOCK_FULL_ROUNDS 8

namespace block {
namespace idea {
    class idea : public block_cipher {
        IDEA_KEY_SCHEDULE _ctx;
        bool _enc;

    public:
        idea(std::size_t rounds = IDEA_BLOCK_FULL_ROUNDS, bool enc = true);

        void keysetup(const std::uint8_t *key, std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t *iv, std::uint64_t ivsize) override;

        void encrypt(const std::uint8_t *plaintext,
                     std::uint8_t *ciphertext) override;

        void decrypt(const std::uint8_t *ciphertext,
                     std::uint8_t *plaintext) override;
    };
}
}
