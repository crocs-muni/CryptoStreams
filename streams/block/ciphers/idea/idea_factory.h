#pragma once

#include "../../block_cipher.h"
#include "idea.h"

namespace block {

    class idea_factory : public block_cipher {

    public:
        idea_factory(unsigned int rounds)
                : block_cipher(rounds)
        {}

        void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const uint64_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    private:
        IdeaContext _ctx;
    };
}
