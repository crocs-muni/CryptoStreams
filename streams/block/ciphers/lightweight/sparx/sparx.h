//
// Created by mhajas on 7/24/18.
//

#pragma once

#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/rotations/rot16.h>
#include <streams/block/ciphers/lightweight/common/rotations/rot32.h>

#define SPARX_BLOCK_SIZE128 16
#define SPARX_BLOCK_SIZE64 8

#define SPARX_KEY_SIZE 16

#define SPARX_ROUND_KEYS_SIZE128 528
#define SPARX_ROUND_KEYS_SIZE64 200

#define SPARX_NUMBER_OF_ROUNDS 8


namespace block {

    template <size_t KEY_SIZE, size_t BLOCK_SIZE>
    class sparx : public lightweight<KEY_SIZE, BLOCK_SIZE> {
    protected:

        void speckey(uint16_t *left, uint16_t *right)
        {
            *left = rot16r7(*left);
            *left += *right;

            *right = rot16l2(*right);
            *right ^= *left;
        }

        void speckey_inverse(uint16_t *left, uint16_t *right)
        {
            *right ^= *left;
            *right = rot16r2(*right);

            *left -= *right;
            *left = rot16l7(*left);
        }

    public:
        sparx(size_t rounds) : lightweight<KEY_SIZE, BLOCK_SIZE>(rounds) {}
    };

    class sparx_b128 : public sparx<SPARX_ROUND_KEYS_SIZE128, SPARX_BLOCK_SIZE128> {
    private:
        void round_f(uint16_t *block, uint16_t *roundKeys)
        {
            uint16_t temp;


            /* first branch */
            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[0]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[1]);
            speckey(&block[0], &block[1]);

            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[2]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[3]);
            speckey(&block[0], &block[1]);

            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[4]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[5]);
            speckey(&block[0], &block[1]);

            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[6]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[7]);
            speckey(&block[0], &block[1]);


            /* second branch */
            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[8]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[9]);
            speckey(&block[2], &block[3]);

            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[10]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[11]);
            speckey(&block[2], &block[3]);

            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[12]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[13]);
            speckey(&block[2], &block[3]);

            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[14]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[15]);
            speckey(&block[2], &block[3]);


            /* third branch */
            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[16]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[17]);
            speckey(&block[4], &block[5]);

            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[18]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[19]);
            speckey(&block[4], &block[5]);

            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[20]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[21]);
            speckey(&block[4], &block[5]);

            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[22]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[23]);
            speckey(&block[4], &block[5]);


            /* fourth branch */
            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[24]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[25]);
            speckey(&block[6], &block[7]);

            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[26]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[27]);
            speckey(&block[6], &block[7]);

            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[28]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[29]);
            speckey(&block[6], &block[7]);

            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[30]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[31]);
            speckey(&block[6], &block[7]);


            /* linear layer */
            /*uint16_t t[4];
            t[0] = block[0];
            t[1] = block[1];
            t[2] = block[2];
            t[3] = block[3];

            temp = block[0] ^ block[1] ^ block[2] ^ block[3];
            temp = rot16l8(temp);

            block[0] ^= temp;
            block[1] ^= temp;
            block[2] ^= temp;
            block[3] ^= temp;

            block[0] ^= block[6];
            block[1] ^= block[5];
            block[2] ^= block[4];
            block[3] ^= block[7];

            temp = block[0];
            block[0] = block[2];
            block[2] = temp;

            block[4] = t[0];
            block[5] = t[1];
            block[6] = t[2];
            block[7] = t[3];*/


            /*temp = block[0] ^ block[1] ^ block[2] ^ block[3];
            temp = rot16l8(temp);

            block[4] ^= block[2] ^ temp;
            block[5] ^= block[1] ^ temp;
            block[6] ^= block[0] ^ temp;
            block[7] ^= block[3] ^ temp;

            temp = block[0];
            block[0] = block[4];
            block[4] = temp;

            temp = block[1];
            block[1] = block[5];
            block[5] = temp;

            temp = block[2];
            block[2] = block[6];
            block[6] = temp;

            temp = block[3];
            block[3] = block[7];
            block[7] = temp;*/


            uint32_t *Block = (uint32_t *)block;
            uint32_t t = Block[0] ^ Block[1];
            uint32_t tx[2];

            tx[0] = Block[0];
            tx[1] = Block[1];

            t = rot32l8(t) ^ rot32r8(t);
            Block[0] ^= t;
            Block[1] ^= t;

            t = Block[0];
            Block[0] = (Block[0] & 0xffff0000) | (Block[1] & 0x0000ffff);
            Block[1] = (Block[1] & 0xffff0000) | (t & 0x0000ffff);

            Block[0] ^= Block[2];
            Block[1] ^= Block[3];

            Block[2] = tx[0];
            Block[3] = tx[1];
        }

        void round_f_inverse(uint16_t *block, uint16_t *roundKeys)
        {
            uint16_t temp;


            /* linear layer */
            /*uint16_t t[4];
            t[0] = block[4];
            t[1] = block[5];
            t[2] = block[6];
            t[3] = block[7];

            temp = block[4] ^ block[5] ^ block[6] ^ block[7];
            temp = rot16l8(temp);

            block[4] ^= temp;
            block[5] ^= temp;
            block[6] ^= temp;
            block[7] ^= temp;

            block[4] ^= block[2];
            block[5] ^= block[1];
            block[6] ^= block[0];
            block[7] ^= block[3];

            temp = block[4];
            block[4] = block[6];
            block[6] = temp;

            block[0] = t[0];
            block[1] = t[1];
            block[2] = t[2];
            block[3] = t[3];*/


            /*temp = block[0];
            block[0] = block[4];
            block[4] = temp;

            temp = block[1];
            block[1] = block[5];
            block[5] = temp;

            temp = block[2];
            block[2] = block[6];
            block[6] = temp;

            temp = block[3];
            block[3] = block[7];
            block[7] = temp;

            temp = block[0] ^ block[1] ^ block[2] ^ block[3];
            temp = rot16l8(temp);

            block[4] ^= block[2] ^ temp;
            block[5] ^= block[1] ^ temp;
            block[6] ^= block[0] ^ temp;
            block[7] ^= block[3] ^ temp;*/


            uint32_t *Block = (uint32_t *)block;
            uint32_t t = Block[2] ^ Block[3];
            uint32_t tx[2];

            tx[0] = Block[2];
            tx[1] = Block[3];

            t = rot32l8(t) ^ rot32r8(t);
            Block[2] ^= t;
            Block[3] ^= t;

            t = Block[2];
            Block[2] = (Block[2] & 0xffff0000) | (Block[3] & 0x0000ffff);
            Block[3] = (Block[3] & 0xffff0000) | (t & 0x0000ffff);

            Block[2] ^= Block[0];
            Block[3] ^= Block[1];

            Block[0] = tx[0];
            Block[1] = tx[1];


            /* fourth branch */
            speckey_inverse(&block[6], &block[7]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[31]);
            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[30]);

            speckey_inverse(&block[6], &block[7]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[29]);
            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[28]);

            speckey_inverse(&block[6], &block[7]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[27]);
            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[26]);

            speckey_inverse(&block[6], &block[7]);
            block[7] ^= READ_ROUND_KEY_WORD(roundKeys[25]);
            block[6] ^= READ_ROUND_KEY_WORD(roundKeys[24]);


            /* third branch */
            speckey_inverse(&block[4], &block[5]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[23]);
            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[22]);

            speckey_inverse(&block[4], &block[5]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[21]);
            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[20]);

            speckey_inverse(&block[4], &block[5]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[19]);
            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[18]);

            speckey_inverse(&block[4], &block[5]);
            block[5] ^= READ_ROUND_KEY_WORD(roundKeys[17]);
            block[4] ^= READ_ROUND_KEY_WORD(roundKeys[16]);


            /* second branch */
            speckey_inverse(&block[2], &block[3]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[15]);
            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[14]);

            speckey_inverse(&block[2], &block[3]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[13]);
            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[12]);

            speckey_inverse(&block[2], &block[3]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[11]);
            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[10]);

            speckey_inverse(&block[2], &block[3]);
            block[3] ^= READ_ROUND_KEY_WORD(roundKeys[9]);
            block[2] ^= READ_ROUND_KEY_WORD(roundKeys[8]);


            /* first branch */
            speckey_inverse(&block[0], &block[1]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[7]);
            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[6]);

            speckey_inverse(&block[0], &block[1]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[5]);
            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[4]);

            speckey_inverse(&block[0], &block[1]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[3]);
            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[2]);

            speckey_inverse(&block[0], &block[1]);
            block[1] ^= READ_ROUND_KEY_WORD(roundKeys[1]);
            block[0] ^= READ_ROUND_KEY_WORD(roundKeys[0]);
        }


    public:
        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;

    public:
        sparx_b128(size_t rounds) : sparx(rounds) {}
    };

    class sparx_b64 : public sparx<SPARX_ROUND_KEYS_SIZE64, SPARX_BLOCK_SIZE64> {
    private:
        void round_f(uint32_t *left, uint32_t *right, uint32_t *roundKeys)
        {
            uint32_t temp;

            uint16_t *b0_l = (uint16_t *)left;
            uint16_t *b0_r = (uint16_t *)left + 1;

            uint16_t *b1_l = (uint16_t *)right;
            uint16_t *b1_r = (uint16_t *)right + 1;


            /* left branch */
            *left ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[0]);
            speckey(b0_l, b0_r);

            *left ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[1]);
            speckey(b0_l, b0_r);

            *left ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[2]);
            speckey(b0_l, b0_r);


            /* right branch */
            *right ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[3]);
            speckey(b1_l, b1_r);

            *right ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[4]);
            speckey(b1_l, b1_r);

            *right ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[5]);
            speckey(b1_l, b1_r);


            /* linear layer */
            temp = *left;
            *right ^= *left ^ rot32l8(*left) ^ rot32r8(*left);
            *left = *right;
            *right = temp;
        }

        void round_f_inverse(uint32_t *left, uint32_t *right, uint32_t *roundKeys)
        {
            uint32_t temp;

            uint16_t *b0_l = (uint16_t *)left;
            uint16_t *b0_r = (uint16_t *)left + 1;

            uint16_t *b1_l = (uint16_t *)right;
            uint16_t *b1_r = (uint16_t *)right + 1;


            /* linear layer */
            temp = *right;
            *left ^= *right ^ rot32l8(*right) ^ rot32r8(*right);
            *right = *left;
            *left = temp;


            /* right branch */
            speckey_inverse(b1_l, b1_r);
            *right ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[5]);

            speckey_inverse(b1_l, b1_r);
            *right ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[4]);

            speckey_inverse(b1_l, b1_r);
            *right ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[3]);


            /* left branch */
            speckey_inverse(b0_l, b0_r);
            *left ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[2]);

            speckey_inverse(b0_l, b0_r);
            *left ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[1]);

            speckey_inverse(b0_l, b0_r);
            *left ^= READ_ROUND_KEY_DOUBLE_WORD(roundKeys[0]);
        }


    public:
        sparx_b64(size_t rounds) : sparx(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;
    };
}
