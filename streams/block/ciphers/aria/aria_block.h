//
// Created by Dusan Klinec on 25/03/2018.
//

#ifndef EACIRC_STREAMS_ARIA_BLOCK_H
#define EACIRC_STREAMS_ARIA_BLOCK_H

// Rounds are key-size dependent
#define ARIA_BLOCK_FULL_ROUNDS 12

#include "../../block_cipher.h"
#include "aria.h"

namespace block {
namespace aria {
    class aria : public block_cipher {
        ARIA_KEY _ctx;
        bool _enc;

    public:
        aria(std::size_t rounds = ARIA_BLOCK_FULL_ROUNDS, bool encrypt = true);

        void keysetup(const std::uint8_t *key, std::uint64_t keysize) override;

        void ivsetup(const std::uint8_t *iv, std::uint64_t ivsize) override;

        void encrypt(const std::uint8_t *plaintext,
                     std::uint8_t *ciphertext) override;

        void decrypt(const std::uint8_t *ciphertext,
                     std::uint8_t *plaintext) override;
    };
}
}


#endif //EACIRC_STREAMS_ARIA_BLOCK_H
