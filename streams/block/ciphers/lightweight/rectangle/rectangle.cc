//
// Created by mhajas on 7/21/18.
//

#include "rectangle.h"

namespace block {

    void rectangle::Encrypt(uint8_t *block) {
        uint16_t *block16 = (uint16_t*)block;
        uint16_t *roundKeys16 = (uint16_t*)_key;

        uint16_t w0 = *block16;
        uint16_t w1 = *(block16+1);
        uint16_t w2 = *(block16+2);
        uint16_t w3 = *(block16+3);

        uint16_t sbox0, sbox1;
        uint8_t i;
        for ( i = 0; i < RECTANGLE_NUMBER_OF_ROUNDS; ++i ) {
            // AddRoundKey
            w0 ^= READ_ROUND_KEY_WORD(*(roundKeys16));
            w1 ^= READ_ROUND_KEY_WORD(*(roundKeys16+1));
            w2 ^= READ_ROUND_KEY_WORD(*(roundKeys16+2));
            w3 ^= READ_ROUND_KEY_WORD(*(roundKeys16+3));
            roundKeys16 += 4;
            // SubColumn
            sbox0 =  w2;
            w2    ^= w1;
            w1    =  ~w1;
            sbox1 =  w0;
            w0    &= w1;
            w1    |= w3;
            w1    ^= sbox1;
            w3    ^= sbox0;
            w0    ^= w3;
            w3    &= w1;
            w3    ^= w2;
            w2    |= w0;
            w2    ^= w1;
            w1    ^= sbox0;
            // ShiftRow
            w1 = (w1<<1  | w1 >> 15);
            w2 = (w2<<12 | w2 >> 4);
            w3 = (w3<<13 | w3 >> 3);
        }
        // last AddRoundKey
        w0 ^= READ_ROUND_KEY_WORD(*(roundKeys16));
        w1 ^= READ_ROUND_KEY_WORD(*(roundKeys16+1));
        w2 ^= READ_ROUND_KEY_WORD(*(roundKeys16+2));
        w3 ^= READ_ROUND_KEY_WORD(*(roundKeys16+3));
        // store cipher text
        *block16 = w0;
        *(block16+1) = w1;
        *(block16+2) = w2;
        *(block16+3) = w3;
    }

    void rectangle::Decrypt(uint8_t *block) {
        uint16_t *block16 = (uint16_t*)block;
        uint16_t *roundKeys16 = (uint16_t*)_key;
        // point to the start address of round 26
        roundKeys16 += 100;

        uint16_t w0 = *block16;
        uint16_t w1 = *(block16+1);
        uint16_t w2 = *(block16+2);
        uint16_t w3 = *(block16+3);

        uint16_t sbox0;
        uint8_t i;
        for ( i = 0; i < RECTANGLE_NUMBER_OF_ROUNDS; ++i ) {
            //Inverse AddRoundKey
            w0 ^= READ_ROUND_KEY_WORD(*(roundKeys16));
            w1 ^= READ_ROUND_KEY_WORD(*(roundKeys16+1));
            w2 ^= READ_ROUND_KEY_WORD(*(roundKeys16+2));
            w3 ^= READ_ROUND_KEY_WORD(*(roundKeys16+3));
            roundKeys16 -= 4;
            // Inverse ShiftRow
            w1 = (w1>>1  | w1 << 15);
            w2 = (w2>>12 | w2 << 4);
            w3 = (w3>>13 | w3 << 3);
            // Invert SubColumn
            sbox0 =  w0;
            w0    &= w2;
            w0    ^= w3;
            w3    |= sbox0;
            w3    ^= w2;
            w1    ^= w3;
            w2    =  w1;
            w1    ^= sbox0;
            w1    ^= w0;
            w3    =  ~w3;
            sbox0 =  w3;
            w3    |= w1;
            w3    ^= w0;
            w0    &= w1;
            w0    ^= sbox0;
        }
        // last AddRoundKey
        w0 ^= READ_ROUND_KEY_WORD(*(roundKeys16));
        w1 ^= READ_ROUND_KEY_WORD(*(roundKeys16+1));
        w2 ^= READ_ROUND_KEY_WORD(*(roundKeys16+2));
        w3 ^= READ_ROUND_KEY_WORD(*(roundKeys16+3));
        // store plain text
        *block16 = w0;
        *(block16+1) = w1;
        *(block16+2) = w2;
        *(block16+3) = w3;
    }

    void rectangle_k80::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != RECTANGLE_KEY_SIZE_80) {
            throw std::runtime_error("RECTANGLE_K80 function only support key size: "
                                     + std::to_string(RECTANGLE_KEY_SIZE_80));
        }

        uint8_t key8[10];
        uint8_t i;
        for ( i = 0; i < RECTANGLE_KEY_SIZE_80; ++i) {
            key8[i] = key[i];
        }

        uint16_t *key16 = (uint16_t*)key8;
        uint16_t *roundKeys16 = (uint16_t*)_key;

        roundKeys16[0] = key16[0];
        roundKeys16[1] = key16[1];
        roundKeys16[2] = key16[2];
        roundKeys16[3] = key16[3];

        uint8_t sbox0, sbox1;
        uint8_t temp[4];
        uint16_t tempk0;
        for ( i = 1; i <= RECTANGLE_NUMBER_OF_ROUNDS; ++i) {
            temp[0] = key8[0];
            temp[1] = key8[2];
            temp[2] = key8[4];
            temp[3] = key8[6];
            // sbox
            sbox0    =  key8[4];
            key8[4]  ^= key8[2];
            key8[2]  =  ~key8[2];
            sbox1    =  key8[0];
            key8[0]  &= key8[2];
            key8[2]  |= key8[6];
            key8[2]  ^= sbox1;
            key8[6]  ^= sbox0;
            key8[0]  ^= key8[6];
            key8[6]  &= key8[2];
            key8[6]  ^= key8[4];
            key8[4]  |= key8[0];
            key8[4]  ^= key8[2];
            key8[2]  ^= sbox0;
            key8[0]  =  (key8[0]&0x0f) ^ (temp[0]&0xf0);
            key8[2]  =  (key8[2]&0x0f) ^ (temp[1]&0xf0);
            key8[4]  =  (key8[4]&0x0f) ^ (temp[2]&0xf0);
            key8[6]  =  (key8[6]&0x0f) ^ (temp[3]&0xf0);
            // generalized feistel
            tempk0     =  *(key16);
            *(key16)   =  *(key16+1);
            *(key16+1) =  *(key16+2);
            *(key16+2) =  *(key16+3);
            *(key16+3) =  *(key16+4);
            *(key16+4) =  tempk0;
            *(key16)   ^= ((tempk0<<8)|(tempk0>>8));
            tempk0     =  *(key16+2);
            *(key16+3) ^= ((tempk0<<12)|(tempk0>>4));
            // add round constant
            *key8 ^= READ_RC_BYTE(RC[i-1]);

            roundKeys16[4*i]   = key16[0];
            roundKeys16[4*i+1] = key16[1];
            roundKeys16[4*i+2] = key16[2];
            roundKeys16[4*i+3] = key16[3];
        }

    }

    void rectangle_k128::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        uint8_t key8[16];
        uint8_t i;
        for (i = 0; i < RECTANGLE_KEY_SIZE_128; ++i) {
            key8[i] = key[i];
        }

        uint16_t *key16 = (uint16_t *) key8;
        uint16_t *roundKeys16 = (uint16_t *) _key;

        roundKeys16[0] = key16[0];
        roundKeys16[1] = key16[2];
        roundKeys16[2] = key16[4];
        roundKeys16[3] = key16[6];

        uint8_t sbox0, sbox1;
        uint16_t halfRow2;
        uint32_t tempRow0;
        for (i = 1; i <= RECTANGLE_NUMBER_OF_ROUNDS; ++i) {
            // sbox
            sbox0 = key8[8];
            key8[8] ^= key8[4];
            key8[4] = ~key8[4];
            sbox1 = key8[0];
            key8[0] &= key8[4];
            key8[4] |= key8[12];
            key8[4] ^= sbox1;
            key8[12] ^= sbox0;
            key8[0] ^= key8[12];
            key8[12] &= key8[4];
            key8[12] ^= key8[8];
            key8[8] |= key8[0];
            key8[8] ^= key8[4];
            key8[4] ^= sbox0;
            // generalized feistel
            tempRow0 = *((uint32_t *) key8);
            *((uint32_t *) key8) = (tempRow0 << 8 | tempRow0 >> 24) ^ *((uint32_t *) key8 + 1);
            *((uint32_t *) key8 + 1) = *((uint32_t *) key8 + 2);
            halfRow2 = *(key16 + 4);
            *(key16 + 4) = *(key16 + 5) ^ *(key16 + 6);
            *(key16 + 5) = halfRow2 ^ *(key16 + 7);
            *((uint32_t *) key8 + 3) = tempRow0;
            // add round constant
            *key8 ^= READ_RC_BYTE(RC[i - 1]);
            // store round keys
            roundKeys16[4 * i] = key16[0];
            roundKeys16[4 * i + 1] = key16[2];
            roundKeys16[4 * i + 2] = key16[4];
            roundKeys16[4 * i + 3] = key16[6];
        }

    }
}
