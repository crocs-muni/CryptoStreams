#pragma once

/**
 * Source: https://github.com/B-Con/crypto-algorithms
 */

#include "../../block_cipher.h"
#include "twofish.h"
namespace block {

    class twofish_factory : public block_cipher {

    public:
        twofish_factory(unsigned int rounds)
            : block_cipher(rounds),
            _twofish(Twofish()){}

        void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const uint64_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;

    private:
        TwofishKey _key;
        Twofish _twofish;
    };
}
