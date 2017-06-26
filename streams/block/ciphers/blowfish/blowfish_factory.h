#pragma once

/**
 * Source: https://github.com/B-Con/crypto-algorithms
 */

#include "../../block_cipher.h"
#include "blowfish.h"
namespace block {

    class blowfish_factory : public block_cipher {

    public:
        blowfish_factory(unsigned int rounds)
            : block_cipher(rounds) {}

        void keysetup(const std::uint8_t* key, const std::uint32_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const std::uint32_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    private:
        BLOWFISH_KEY _keystruct;
    };
}
