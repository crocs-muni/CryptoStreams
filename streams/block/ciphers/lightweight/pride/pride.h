//
// Created by mhajas on 7/21/18.
//

#pragma once

#include <streams/block/ciphers/lightweight/lightweight.h>
#include "pride_functions.h"

#define ROUND_CONSTANT_BYTE ROM_DATA_BYTE
#define READ_ROUND_CONSTANT_BYTE READ_ROM_DATA_BYTE

#define PRIDE_BLOCK_SIZE 8

#define PRIDE_KEY_SIZE 16
#define PRIDE_ROUND_KEYS_SIZE 168

#define PRIDE_NUMBER_OF_ROUNDS 20

namespace block {
    class pride : public lightweight<PRIDE_ROUND_KEYS_SIZE, PRIDE_BLOCK_SIZE> {

        /*
         *
         * Cipher constants
         *
         */
        ROUND_CONSTANT_BYTE round_constants[80] = {
                0xC1,0xA5,0x51,0xC5,0x82,0x4A,0xA2,0x8A,
                0x43,0xEF,0xF3,0x4F,0x04,0x94,0x44,0x14,
                0xC5,0x39,0x95,0xD9,0x86,0xDE,0xE6,0x9E,
                0x47,0x83,0x37,0x63,0x08,0x28,0x88,0x28,
                0xC9,0xCD,0xD9,0xED,0x8A,0x72,0x2A,0xB2,
                0x4B,0x17,0x7B,0x77,0x0C,0xBC,0xCC,0x3C,
                0xCD,0x61,0x1D,0x01,0x8E,0x06,0x6E,0xC6,
                0x4F,0xAB,0xBF,0x8B,0x10,0x50,0x10,0x50,
                0xD1,0xF5,0x61,0x15,0x92,0x9A,0xB2,0xDA,
                0x53,0x3F,0x03,0x9F,0x14,0xE4,0x54,0x64
        };

        void encryption_round_function(uint8_t *data,uint8_t *rkey, uint8_t *temp);

        void decryption_round_function(uint8_t *data,uint8_t *rkey, uint8_t *temp);

        bool _encrypt;

    public:
        pride(size_t rounds, bool encrypt) : lightweight(rounds), _encrypt(encrypt) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };
}
