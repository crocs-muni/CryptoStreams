#pragma once

#include <cstdint>
#include <cstddef>

namespace block {

    struct block_cipher {
        block_cipher(std::size_t rounds)
            : _rounds(rounds) { }

        virtual ~block_cipher() = default;

        /**
         * Key setup. It is the user's responsibility to select the values of
         * keysize and ivsize from the set of supported values specified
         * above.
         */
        virtual void keysetup(const std::uint8_t* key, const std::uint64_t keysize) = 0;

        /**
         * IV setup. After having called ECRYPT_keysetup(), the user is
         * allowed to call ECRYPT_ivsetup() different times in order to
         * encrypt/decrypt different messages with the same key but different
         * IV's.
         */
        virtual void ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) = 0;

        virtual void encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext) = 0;
        virtual void decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext) = 0;

    protected:
        std::size_t _rounds;
    };

}
