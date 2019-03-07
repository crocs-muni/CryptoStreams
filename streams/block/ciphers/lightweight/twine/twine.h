//
// Created by mhajas on 7/24/18.
//

#pragma once


#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

#define DATA_SBOX_BYTE RAM_DATA_BYTE
#define READ_SBOX_BYTE READ_RAM_DATA_BYTE

#define DATA_KS_BYTE RAM_DATA_BYTE
#define READ_KS_BYTE READ_RAM_DATA_BYTE

#define TWINE_BLOCK_SIZE 8

#define TWINE_KEY_SIZE 10
#define TWINE_ROUND_KEYS_SIZE 144

#define TWINE_NUMBER_OF_ROUNDS 35

namespace block {
    class twine : public lightweight<TWINE_ROUND_KEYS_SIZE, TWINE_BLOCK_SIZE> {
    private:
        DATA_SBOX_BYTE Sbox[16] =
        {
            0x0c, 0x00, 0x0f, 0x0a,
            0x02, 0x0b, 0x09, 0x05,
            0x08, 0x03, 0x0d, 0x07,
            0x01, 0x0e, 0x06, 0x04
        };

        DATA_KS_BYTE RCON[35] =
        {
            0x01, 0x02, 0x04, 0x08,
            0x10, 0x20, 0x03, 0x06,
            0x0c, 0x18, 0x30, 0x23,
            0x05, 0x0a, 0x14, 0x28,
            0x13, 0x26, 0x0f, 0x1e,
            0x3c, 0x3b, 0x35, 0x29,
            0x11, 0x22, 0x07, 0x0e,
            0x1c, 0x38, 0x33, 0x25,
            0x09, 0x12, 0x24
        };

    public:
        twine(size_t rounds) : lightweight(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };
}
