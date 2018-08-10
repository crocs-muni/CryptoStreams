//
// Created by mhajas on 7/6/18.
//

#ifndef CRYPTO_STREAMS_CHASKEY_H
#define CRYPTO_STREAMS_CHASKEY_H

#include <streams/block/ciphers/lightweight/lightweight.h>
#include "../../../block_cipher.h"


#define CHASKEY_BLOCK_SIZE 16
#define CHASKEY_KEY_SIZE 16
#define CHASKEY_ROUND_KEYS_SIZE 16
#define CHASKEY_NUMBER_OF_ROUNDS 16

namespace block {

    class chaskey : public lightweight<CHASKEY_KEY_SIZE, CHASKEY_BLOCK_SIZE> {

        static inline uint32_t rol(uint32_t x, const uint8_t n);

        static inline uint32_t ror(uint32_t x, const uint8_t n);
    public:
        chaskey(size_t rounds) : lightweight(rounds) {};

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };

} //namespace block


#endif //CRYPTO_STREAMS_CHASKEY_H
