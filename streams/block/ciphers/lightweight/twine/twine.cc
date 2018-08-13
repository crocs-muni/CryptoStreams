//
// Created by mhajas on 7/24/18.
//

#include "twine.h"

namespace block {

    void twine::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        uint8_t KeyR[20];

        uint8_t i;
        uint8_t temp, temp1, temp2, temp3;

        uint16_t *master_key = (uint16_t*)key;


        for (i = 0; i < 20; i++)
        {
            KeyR[i] = (master_key[(i / 4)] >> (4 * (i & 0x03))) & 0x0F;
        }

        for (i = 0; i < 35; i++)
        {
            _key[4 * i + 0] = KeyR[1] ^ (KeyR[3] << 4);
            _key[4 * i + 1] = KeyR[4] ^ (KeyR[6] << 4);
            _key[4 * i + 2] = KeyR[13] ^ (KeyR[14] << 4);
            _key[4 * i + 3] = KeyR[15] ^ (KeyR[16] << 4);

            KeyR[1] = KeyR[1] ^ READ_SBOX_BYTE(Sbox[KeyR[0]]);
            KeyR[4] = KeyR[4] ^ READ_SBOX_BYTE(Sbox[KeyR[16]]);
            KeyR[7] = KeyR[7] ^ (READ_KS_BYTE(RCON[i]) >> 3);
            KeyR[19] = KeyR[19] ^ (READ_KS_BYTE(RCON[i]) & 0x07);

            temp = KeyR[0];
            KeyR[0] = KeyR[1];
            KeyR[1] = KeyR[2];
            KeyR[2] = KeyR[3];
            KeyR[3] = temp;

            temp = KeyR[0];
            temp1 = KeyR[1];
            temp2 = KeyR[2];
            temp3 = KeyR[3];

            KeyR[0] = KeyR[4];
            KeyR[1] = KeyR[5];
            KeyR[2] = KeyR[6];
            KeyR[3] = KeyR[7];

            KeyR[4] = KeyR[8];
            KeyR[5] = KeyR[9];
            KeyR[6] = KeyR[10];
            KeyR[7] = KeyR[11];

            KeyR[8] = KeyR[12];
            KeyR[9] = KeyR[13];
            KeyR[10] = KeyR[14];
            KeyR[11] = KeyR[15];

            KeyR[12] = KeyR[16];
            KeyR[13] = KeyR[17];
            KeyR[14] = KeyR[18];
            KeyR[15] = KeyR[19];

            KeyR[16] = temp;
            KeyR[17] = temp1;
            KeyR[18] = temp2;
            KeyR[19] = temp3;
        }

        _key[140] = KeyR[1] ^ (KeyR[3] << 4);
        _key[141] = KeyR[4] ^ (KeyR[6] << 4);
        _key[142] = KeyR[13] ^ (KeyR[14] << 4);
        _key[143] = KeyR[15] ^ (KeyR[16] << 4);
    }

    void twine::Encrypt(uint8_t *block) {
        uint8_t i, r;


        for (r = 0; r < _rounds; r++)
        {
            /* F-function */
            for (i = 0; i < 8; ++i)
            {
                block[i] ^= READ_SBOX_BYTE(
                        Sbox[
                                block[i] & 0x0F ^
                                0x0F & READ_ROUND_KEY_BYTE(
                                        _key[r * 4 + i / 2]
                                ) >> (4 * (i % 2))
                        ]
                ) << 4;
            }

            /* Output */
            i = block[0];

            /*0 <-1 */
            block[0] &= 0xF0;
            block[0] ^= block[0] >> 4;

            /*1 <-2 */
            block[0] &= 0x0F;
            block[0] ^= block[1] << 4;

            /* 2 <-11 */
            block[1] &= 0xF0;
            block[1] ^= block[5] >>4;

            /* 11 <-14 */
            block[5] &= 0x0F;
            block[5] ^= block[7] << 4;

            /* 14 <-15 */
            block[7] &= 0xF0;
            block[7] ^= block[7] >> 4;

            /* 15 <-12 */
            block[7] &= 0x0F;
            block[7] ^= block[6] << 4;

            /* 12 <-5 */
            block[6] &= 0xF0;
            block[6] ^= block[2] >> 4;

            /* 5 <-0 */
            block[2] &= 0x0F;
            block[2] ^=i << 4;

            i = block[1];

            /*3 <-6 */
            block[1] &= 0x0F;
            block[1] ^= block[3] << 4;

            /*6 <-9 */
            block[3] &= 0xF0;
            block[3] ^= block[4] >> 4;

            /*9 <-10 */
            block[4] &= 0x0F;
            block[4] ^= block[5] << 4;

            /* 10 <-13 */
            block[5] &= 0xF0;
            block[5] ^= block[6] >> 4;

            /* 13 <-8 */
            block[6] &= 0x0F;
            block[6] ^= block[4] << 4;

            /* 8 <-7 */
            block[4] &= 0xF0;
            block[4] ^= block[3] >> 4;

            /* 7 <-4 */
            block[3] &= 0x0F;
            block[3] ^= block[2] << 4;

            /* 4 <-3 */
            block[2] &= 0xF0;
            block[2] ^= i >> 4;

        }

        /* F-function */
        for (i = 0; i < 8; ++i)
        {
            block[i] ^= READ_SBOX_BYTE(
                    Sbox[
                            block[i] & 0x0F ^
                            0x0F & READ_ROUND_KEY_BYTE(
                                    _key[r * 4 + i / 2]
                            ) >> (4 * (i % 2))
                    ]
            ) << 4;
        }

    }

    void twine::Decrypt(uint8_t *block) {
        uint8_t i, r;


        for (r = _rounds; r > 0; r--)
        {
            for (i = 0; i < 8; i++)
            {
                block[i] ^= READ_SBOX_BYTE(
                        Sbox[
                                block[i] & 0x0F ^
                                0x0F & READ_ROUND_KEY_BYTE(
                                        _key[r * 4 + i / 2]
                                ) >> (4 * (i % 2))
                        ]
                ) << 4;
            }

            /* Output */
            i = block[0];

            /* 0 <-5 */
            block[0] &= 0xF0;
            block[0] ^= block[2] >> 4;

            /* 5 <-12 */
            block[2] &= 0x0F;
            block[2] ^= block[6] << 4;

            /* 12 <-15 */
            block[6] &= 0xF0;
            block[6] ^= block[7] >> 4;

            /* 15 <-14  */
            block[7] &= 0xF;
            block[7] ^= block[7] << 4;

            /* 14 <-11 */
            block[7] &= 0xF0;
            block[7] ^= block[5] >> 4;

            /* 11 <-2 */
            block[5] &= 0x0F;
            block[5] ^= block[1] << 4;

            /* 2 <-1 */
            block[1] &= 0xF0;
            block[1] ^= block[0] >> 4;

            /* 1 <-0  */
            block[0] &= 0x0F;
            block[0] ^= i << 4;

            i = block[1];

            /* 3 <-4 */
            block[1] &= 0x0F;
            block[1] ^= block[2] << 4;

            /* 4 <-7 */
            block[2] &= 0xF0;
            block[2] ^= block[3] >> 4;

            /* 7 <-8 */
            block[3] &= 0x0F;
            block[3] ^= block[4] << 4;

            /* 8 <-13 */
            block[4] &= 0xF0;
            block[4] ^= block[6] >> 4;

            /* 13 <-10 */
            block[6] &= 0x0F;
            block[6] ^= block[5] << 4;

            /* 10 <-9 */
            block[5] &= 0xF0;
            block[5] ^= block[4] >> 4;

            /* 9 <-6 */
            block[4] &= 0x0F;
            block[4] ^= block[3] << 4;

            /* 6 <-3 */
            block[3] &= 0xF0;
            block[3] ^= i >> 4;

        }

        for (i = 0; i < 8; i++)
        {
            block[i] ^= READ_SBOX_BYTE(
                    Sbox[
                            block[i] & 0x0F ^
                            0x0F & READ_ROUND_KEY_BYTE(
                                    _key[i / 2]
                            ) >> (4 * (i % 2))
                    ]
            ) << 4;
        }

    }
}