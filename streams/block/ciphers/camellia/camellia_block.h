//
// Created by Dusan Klinec on 25/03/2018.
//

#ifndef EACIRC_STREAMS_CAMELLIA_BLOCK_H
#define EACIRC_STREAMS_CAMELLIA_BLOCK_H

#include "../../block_cipher.h"
#include "camellia.h"

// Rounds are key-size dependent
#define CAMELLIA_BLOCK_FULL_ROUNDS 18

namespace block {
namespace camellia {
    class camellia : public block_cipher {
        mbedtls_camellia_context _ctx;
        bool _enc;

    public:
        camellia(std::size_t rounds = CAMELLIA_BLOCK_FULL_ROUNDS, bool encrypt = true);

        void keysetup(const std::uint8_t *key, std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t *iv, std::uint64_t ivsize) override;

        void encrypt(const std::uint8_t *plaintext,
                     std::uint8_t *ciphertext) override;

        void decrypt(const std::uint8_t *ciphertext,
                     std::uint8_t *plaintext) override;
    };
}
}

#endif //EACIRC_STREAMS_CAMELLIA_BLOCK_H
