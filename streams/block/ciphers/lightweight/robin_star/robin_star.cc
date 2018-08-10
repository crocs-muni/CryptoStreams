//
// Created by mhajas on 7/22/18.
//

#include <streams/block/ciphers/lightweight/common/rotations/rot16.h>
#include "robin_star.h"

namespace block {

    void robin_star::Encrypt(uint8_t *block) {
        /*
         *
         * Access state as 16-bit values
         * Assumes little-endian machine
         *
         */
        uint16_t *data = (uint16_t *)block;
        uint16_t *key = (uint16_t *)_key;
        uint16_t t = 0;
        uint16_t u;
        uint8_t i, j;


        /* Initial key addition */
        for (j = 0; j < 8; j++)
        {
            data[j] ^= READ_ROUND_KEY_WORD(key[j]);
        }

        for (i = 0; i < ROBIN_STAR_NUMBER_OF_ROUNDS; i++)
        {
            /* Round constant */
            u = t;
            data[0] ^= u;
            for (j = 1; j < 8; j++)
            {
                u = rot16l1(u);
                data[j] ^= u;
            }
            t += 2199;

            /* SBox layer (bitsliced) */
            SBOX(data);

            /* LBox layer (tables) */
            for (j = 0; j < 8; j++)
            {
                data[j] = READ_LBOX_WORD(LBox2[data[j] >> 8]) ^
                          READ_LBOX_WORD(LBox1[data[j] & 0xff]);
            }

            /* Key addition */
            for (j = 0; j < 8; j++)
            {
                data[j] ^= READ_ROUND_KEY_WORD(key[j]);
            }
        }    }

    void robin_star::Decrypt(uint8_t *block) {
        /*
         *
         * Access state as 16-bit values
         * Assumes little-endian machine
         *
         */
        uint16_t *data = (uint16_t *)block;
        uint16_t *key = (uint16_t *)_key;
        uint16_t t = 32985;
        uint16_t u;
        uint8_t i, j;


        /* Initial key addition */
        for (j = 0; j < 8; j++)
        {
            data[j] ^= READ_ROUND_KEY_WORD(key[j]);
        }

        for (i = 0; i < ROBIN_STAR_NUMBER_OF_ROUNDS; i++)
        {
            /* LBox layer (tables) */
            for (j = 0; j < 8; j++)
            {
                data[j] = READ_LBOX_WORD(LBox2[data[j] >> 8]) ^
                          READ_LBOX_WORD(LBox1[data[j] & 0xff]);
            }

            /* SBox layer (bitsliced) */
            SBOX(data);

            /* Key addition */
            for (j = 0; j < 8; j++)
            {
                data[j] ^= READ_ROUND_KEY_WORD(key[j]);
            }

            /* Round constant */
            u = t;
            data[0] ^= u;
            for (j = 1; j < 8; j++)
            {
                u = rot16l1(u);
                data[j] ^= u;
            }
            t -= 2199;
        }    }
}