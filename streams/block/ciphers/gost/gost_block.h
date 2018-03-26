//
// Created by Dusan Klinec on 23/03/2018.
//

#ifndef EACIRC_STREAMS_GOST_BLOCK_H
#define EACIRC_STREAMS_GOST_BLOCK_H

/**
 * Source: https://github.com/gost-engine/engine/blob/master/gost89.c
 */

#include "../../block_cipher.h"
#include "gost89.h"
#define GOST_BLOCK_FULL_ROUNDS 32

namespace block {

    class gost : public block_cipher {
        gost_ctx _ctx;

    public:
        gost(std::size_t rounds = GOST_BLOCK_FULL_ROUNDS);

        void keysetup(const std::uint8_t* key, std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t* iv, std::uint64_t ivsize) override;

        void encrypt(const std::uint8_t* plaintext,
                     std::uint8_t* ciphertext) override;

        void decrypt(const std::uint8_t* ciphertext,
                     std::uint8_t* plaintext) override;
    };
}


#endif //EACIRC_STREAMS_GOST_BLOCK_H
