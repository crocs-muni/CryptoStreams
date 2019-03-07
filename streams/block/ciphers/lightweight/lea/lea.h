//
// Created by mhajas on 7/21/18.
//

#pragma once

#define LEA_BLOCK_SIZE 16
#define LEA_KEY_SIZE 16
#define LEA_ROUND_KEYS_SIZE 384
#define LEA_NUMBER_OF_ROUNDS 24

#define RK(x, y) READ_ROUND_KEY_DOUBLE_WORD((x)[y])

#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

namespace block {
    class lea : public lightweight<LEA_ROUND_KEYS_SIZE, LEA_BLOCK_SIZE> {
        RAM_DATA_DOUBLE_WORD DELTA[4]= {0xc3efe9db, 0x88c4d604, 0xe789f229, 0xc6f98763};

    public:
        lea(size_t rounds) : lightweight(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };
}
