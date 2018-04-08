#pragma once

#include "../../block_cipher.h"
#include "shacal2.h"

namespace block {

    class shacal2_factory : public block_cipher {

    public:
        shacal2_factory(unsigned int rounds)
                : block_cipher(rounds)
        {}

        void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const uint64_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    private:
        Botan::SHACAL2 _shacal2;
    };
}
