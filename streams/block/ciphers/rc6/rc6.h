#pragma once

/**
 * Source: nist AES competition submission, downloadable now only from:
 *          https://www.schneier.com/books/applied_cryptography/source.html
 */

#include "../../block_cipher.h"
#include "../common_fun.h"

#include "rc6_ref.h"
#include <algorithm>

namespace block {
namespace rc6 {

    class rc6 : public block_cipher {

        /* Data structures */

        struct rc6_ctx {
            rc6_ctx() {
                cipher.mode = MODE_ECB;

            }

            keyInstance key;
            cipherInstance cipher;
        } _ctx;
        bool _decrypt;

    public:
        rc6(std::size_t rounds, bool encrypt)
            : block_cipher(rounds)
            , _decrypt(!encrypt) { }

        void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override {
            std::vector<char> hex_key(keysize * 2 + 1);
            bin_to_hex(key, keysize, hex_key);
            hex_key[keysize * 2] = 0; // terminating 0
            makeKey(&_ctx.key, _decrypt, int(keysize * 8), hex_key.data());
        }

        void ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) override {
            char mutable_iv[ivsize];
            std::copy(iv, iv + ivsize, mutable_iv);
            cipherInit(&_ctx.cipher, _decrypt, mutable_iv);
        }

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override {
            const int block_len = 16;
            unsigned char mutable_plaintext[block_len];
            std::copy(plaintext, plaintext + block_len, mutable_plaintext);
            blockEncrypt(&_ctx.cipher, &_ctx.key,
                         mutable_plaintext, block_len * 8,
                         ciphertext, _rounds);
        }

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override {
            const int block_len = 16;
            unsigned char mutable_ciphertext[block_len];
            std::copy(ciphertext, ciphertext + block_len, mutable_ciphertext);
            blockDecrypt(&_ctx.cipher, &_ctx.key,
                         mutable_ciphertext, block_len * 8,
                         plaintext, _rounds);
        }
    };

} // namespace rc6
} // namespace block
