#pragma once

/**
 * Source: https://www.schneier.com/books/applied_cryptography/source.html
 */

#include <stdexcept>

#include "../../block_cipher.h"
#include "std_defs.h"

namespace block {
namespace twofish {

    class twofish : public block_cipher {

    public:
        twofish(std::size_t rounds)
            : block_cipher(rounds) { }

        void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override {
            set_key(reinterpret_cast<const u4byte *>(key), keysize * 8); // key_len is in bits
        }

        void ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) override {
            throw std::runtime_error("not implemented yet");
        }

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override {
            twofish_encrypt(reinterpret_cast<const u4byte *>(plaintext),
                            reinterpret_cast<u4byte *>(ciphertext),
                            _rounds);
        }

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override{
            twofish_decrypt(reinterpret_cast<const u4byte *>(ciphertext),
                            reinterpret_cast<u4byte *>(plaintext),
                            _rounds);
        }
    };


} // namespace twofish
} // namespace block
