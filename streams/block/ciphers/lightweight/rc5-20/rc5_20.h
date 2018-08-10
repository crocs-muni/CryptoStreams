//
// Created by mhajas on 7/21/18.
//

#ifndef CRYPTO_STREAMS_RC5_20_H
#define CRYPTO_STREAMS_RC5_20_H

#include <streams/block/ciphers/lightweight/lightweight.h>

#define S_DOUBLE_WORD RAM_DATA_DOUBLE_WORD
#define READ_S_DOUBLE_WORD READ_RAM_DATA_DOUBLE_WORD
#define WORD_SIZE 32

#define RC5_20_BLOCK_SIZE 8

#define RC5_20_KEY_SIZE 16
#define RC5_20_ROUND_KEYS_SIZE 168

#define RC5_20_NUMBER_OF_ROUNDS 20

namespace block {

    class rc5_20 : public lightweight<RC5_20_ROUND_KEYS_SIZE, RC5_20_BLOCK_SIZE> {
        /*
         *
         * Cipher constants
         *
         */
        S_DOUBLE_WORD S[42] =
            {
                    0xb7e15163,
                    0x5618cb1c,
                    0xf45044d5,
                    0x9287be8e,
                    0x30bf3847,
                    0xcef6b200,
                    0x6d2e2bb9,
                    0x0b65a572,
                    0xa99d1f2b,
                    0x47d498e4,
                    0xe60c129d,
                    0x84438c56,
                    0x227b060f,
                    0xc0b27fc8,
                    0x5ee9f981,
                    0xfd21733a,
                    0x9b58ecf3,
                    0x399066ac,
                    0xd7c7e065,
                    0x75ff5a1e,
                    0x1436d3d7,
                    0xb26e4d90,
                    0x50a5c749,
                    0xeedd4102,
                    0x8d14babb,
                    0x2b4c3474,
                    0xc983ae2d,
                    0x67bb27e6,
                    0x05f2a19f,
                    0xa42a1b58,
                    0x42619511,
                    0xe0990eca,
                    0x7ed08883,
                    0x1d08023c,
                    0xbb3f7bf5,
                    0x5976f5ae,
                    0xf7ae6f67,
                    0x95e5e920,
                    0x341d62d9,
                    0xd254dc92,
                    0x708c564b,
                    0x0ec3d004
            };

        static inline uint32_t ROL(uint32_t x)
        {
            return ((x << 1) | (x >> (WORD_SIZE - 1)));
        }

        static inline uint32_t ROR(uint32_t x)
        {
            return ((x >> 1) | (x << (WORD_SIZE - 1)));
        }

        uint32_t RC5_ROTL(uint32_t x, uint32_t n);
        uint32_t RC5_ROTR(uint32_t x, uint32_t n);

    public:
        rc5_20(size_t rounds) : lightweight(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };

}


#endif //CRYPTO_STREAMS_RC5_20_H
