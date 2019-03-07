//
// Created by mhajas on 7/8/18.
//

#pragma once

#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

#define SBOX_BYTE RAM_DATA_BYTE
#define READ_SBOX_BYTE READ_RAM_DATA_BYTE

#define LBLOCK_BLOCK_SIZE 8
#define LBLOCK_KEY_SIZE 10
#define LBLOCK_ROUND_KEYS_SIZE 128
#define LBLOCK_NUMBER_OF_ROUNDS 32

namespace block {
    class lblock : public lightweight<LBLOCK_ROUND_KEYS_SIZE, LBLOCK_BLOCK_SIZE> {

        /*
         *
         * Cipher S-boxes
         *
         */
        SBOX_BYTE S0[16] = {14, 9, 15, 0, 13, 4, 10, 11, 1, 2, 8, 3, 7, 6, 12, 5};
        SBOX_BYTE S1[16] = {4, 11, 14, 9, 15, 13, 0, 10, 7, 12, 5, 6, 2, 8, 1, 3};
        SBOX_BYTE S2[16] = {1, 14, 7, 12, 15, 13, 0, 6, 11, 5, 9, 3, 2, 4, 8, 10};
        SBOX_BYTE S3[16] = {7, 6, 8, 11, 0, 15, 3, 14, 9, 10, 12, 13, 5, 2, 4, 1};
        SBOX_BYTE S4[16] = {14, 5, 15, 0, 7, 2, 12, 13, 1, 8, 4, 9, 11, 10, 6, 3};
        SBOX_BYTE S5[16] = {2, 13, 11, 12, 15, 14, 0, 9, 7, 10, 6, 3, 1, 8, 4, 5};
        SBOX_BYTE S6[16] = {11, 9, 4, 14, 0, 15, 10, 13, 6, 12, 5, 7, 3, 8, 1, 2};
        SBOX_BYTE S7[16] = {13, 10, 15, 0, 14, 4, 9, 11, 2, 1, 8, 3, 7, 5, 12, 6};
        SBOX_BYTE S8[16] = {8, 7, 14, 5, 15, 13, 0, 6, 11, 12, 9, 10, 2, 4, 1, 3};
        SBOX_BYTE S9[16] = {11, 5, 15, 0, 7, 2, 9, 13, 4, 8, 1, 12, 14, 10, 3, 6};

    public:
        lblock(size_t rounds) : lightweight(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };
}
