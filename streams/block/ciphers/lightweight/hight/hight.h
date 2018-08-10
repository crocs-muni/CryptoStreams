//
// Created by mhajas on 7/8/18.
//

#ifndef CRYPTO_STREAMS_HIGHT_H
#define CRYPTO_STREAMS_HIGHT_H

#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

#define DELTA_BYTE ROM_DATA_BYTE
#define READ_DELTA_BYTE READ_ROM_DATA_BYTE

#define HIGHT_BLOCK_SIZE 8
#define HIGHT_KEY_SIZE 16
#define HIGHT_ROUND_KEYS_SIZE 136 /* 8 bytes whitening keys + 128 bytes subkeys */
#define HIGHT_NUMBER_OF_ROUNDS 32

namespace block {

    class hight : public lightweight<HIGHT_ROUND_KEYS_SIZE, HIGHT_BLOCK_SIZE> {
        /*
         *
         * Cipher constants
         *
         */
        DELTA_BYTE _delta[128] =
                {
                        0x5A, 0x6D, 0x36, 0x1B, 0x0D, 0x06, 0x03, 0x41,
                        0x60, 0x30, 0x18, 0x4C, 0x66, 0x33, 0x59, 0x2C,
                        0x56, 0x2B, 0x15, 0x4A, 0x65, 0x72, 0x39, 0x1C,
                        0x4E, 0x67, 0x73, 0x79, 0x3C, 0x5E, 0x6F, 0x37,
                        0x5B, 0x2D, 0x16, 0x0B, 0x05, 0x42, 0x21, 0x50,
                        0x28, 0x54, 0x2A, 0x55, 0x6A, 0x75, 0x7A, 0x7D,
                        0x3E, 0x5F, 0x2F, 0x17, 0x4B, 0x25, 0x52, 0x29,
                        0x14, 0x0A, 0x45, 0x62, 0x31, 0x58, 0x6C, 0x76,
                        0x3B, 0x1D, 0x0E, 0x47, 0x63, 0x71, 0x78, 0x7C,
                        0x7E, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x43, 0x61,
                        0x70, 0x38, 0x5C, 0x6E, 0x77, 0x7B, 0x3D, 0x1E,
                        0x4F, 0x27, 0x53, 0x69, 0x34, 0x1A, 0x4D, 0x26,
                        0x13, 0x49, 0x24, 0x12, 0x09, 0x04, 0x02, 0x01,
                        0x40, 0x20, 0x10, 0x08, 0x44, 0x22, 0x11, 0x48,
                        0x64, 0x32, 0x19, 0x0C, 0x46, 0x23, 0x51, 0x68,
                        0x74, 0x3A, 0x5D, 0x2E, 0x57, 0x6B, 0x35, 0x5A
                };

        static inline uint8_t rol(uint8_t x);

        static inline uint8_t ror(uint8_t x);

        /*
         *
         * Auxiliary round function F0
         * ... x - the input data byte
         * Returns: the auxiliary round function F0 result byte
         *
         */
        static inline uint8_t F0(uint8_t x);

        /*
         *
         * Auxiliary round function F1
         * ... x - the input data byte
         * Returns: the auxiliary round function F1 result byte
         *
         */
        static inline uint8_t F1(uint8_t x);

        void EncryptInitialTransfomation(uint8_t *x, const uint8_t *wk);

        void EncryptRoundFunction(uint8_t *x, const uint8_t *sk);

        void EncryptFinalTransfomation(uint8_t *x, const uint8_t *wk);

        void DecryptInitialTransfomation(uint8_t *x, const uint8_t *wk);

        void DecryptRoundFunction(uint8_t *x, const uint8_t *sk);

        void DecryptFinalTransfomation(uint8_t *x, const uint8_t *wk);

    public:
        hight(size_t rounds) : lightweight(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };

}


#endif //CRYPTO_STREAMS_HIGHT_H
