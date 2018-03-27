#pragma once


#include "../../block_cipher.h"
#include <linux/types.h>
#include "kuznyechik.h"

namespace block {

    class kuznyechik_factory : public block_cipher {

    public:
        kuznyechik_factory(unsigned int rounds)
                : block_cipher(rounds)
        {
            kuzn_context_init(&_ctx);
        }

        void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t* iv, const uint64_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    private:
        kuzn_ctx _ctx;
    };
}
