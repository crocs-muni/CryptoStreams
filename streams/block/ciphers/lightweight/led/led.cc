//
// Created by mhajas on 7/21/18.
//

#include "led.h"

namespace block {

    void led::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != LED_KEY_SIZE) {
            throw std::runtime_error("LEA function only support key size: " + std::to_string(LED_KEY_SIZE));
        }

        uint8_t i;

        for(i = 0; i < LED_ROUND_KEYS_SIZE; i ++)
        {
            if(i % 2)
            {
                _key[i] = key[i >> 1] & 0xF;
            }
            else
            {
                _key[i] = (key[i >> 1] >> 4) & 0xF;
            }
        }
    }

    void led::Encrypt(uint8_t *block) {
        int8_t i,j;
        uint8_t state[4][4];

        for(i = 0; i < 16; i++)
        {
            if(i % 2)
            {
                state[i / 4][i % 4] = block[i >> 1] & 0xF;
            }
            else
            {
                state[i / 4][i % 4] = (block[i >> 1] >> 4) & 0xF;
            }
        }

        AddKey(state, _key, 0);
        for(i = 0; i < (LED_NUMBER_OF_ROUNDS >> 2); i++)
        {
            for(j = 0; j < 4; j++)
            {
                AddConstants(state, i * 4 + j);
                SCShRMCS(state);
            }
            AddKey(state, _key, i + 1);
        }

        for(i = 0; i < 8; i++)
        {
            block[i] = ((state[(2 * i) / 4][(2 * i) % 4] & 0xF) << 4) |
                       (state[(2 * i + 1) / 4][(2 * i + 1) % 4] & 0xF);
        }
    }

    void led::Decrypt(uint8_t *block) {
        int8_t i, j;
        uint8_t state[4][4];

        for(i = 0; i < 16; i++)
        {
            if(i % 2)
            {
                state[i / 4][i % 4] = block[i >> 1] & 0xF;
            }
            else
            {
                state[i / 4][i % 4] = (block[i >> 1] >> 4) & 0xF;
            }
        }

        for(i = (LED_NUMBER_OF_ROUNDS >> 2) - 1; i >= 0; i--)
        {
            AddKey(state, _key, i+1);
            for(j = 3; j >= 0; j--)
            {
                invSCShRMCS(state);
                AddConstants(state, i * 4 + j);
            }
        }
        AddKey(state, _key, 0);

        for(i = 0; i < 8; i++)
        {
            block[i] = ((state[(2 * i) / 4][(2 * i) % 4] & 0xF) << 4) |
                       (state[(2 * i + 1) / 4][(2 * i + 1) % 4] & 0xF);
        }
    }

    void led::AddKey(uint8_t (*state)[4], uint8_t *keyBytes, uint8_t half) {
        uint8_t i, j;

        for(i = 0; i < 4; i++)
        {
            for(j = 0; j < 4; j++)
            {
                state[i][j] ^= READ_ROUND_KEY_BYTE(keyBytes[(4 * i + j + half * 16) % LED_ROUND_KEYS_SIZE]);
            }
        }
    }

    void led::SCShRMCS(uint8_t (*state)[4]) {
        uint8_t c, r;
        uint16_t v;
        uint8_t os[4][4];


        memcpy(os, state, 16);

        for(c = 0; c < 4; c++)
        {
            v = 0;

            for(r = 0; r < 4; r++)
            {
                v ^= READ_ROUND_TABLE_WORD(RndTab[r][(os[r][(r + c) & 3])]);
            }

            for(r = 1; r <= 4; r++)
            {
                state[4 - r][c] = ((uint8_t) v) & 0xF;
                v >>= 4;
            }
        }
    }

    void led::AddConstants(uint8_t (*state)[4], uint8_t r) {
        uint8_t tmp;


        /*
        state[1][0] ^= 1;
        state[2][0] ^= 2;
        state[3][0] ^= 3;
        */

        /* Added from reference implementation and merged with the above code */
        state[0][0] ^= 5;   /* (    ((KEY_SIZE>>1) & 0xf)); */
        state[1][0] ^= 4;   /* (1 ^ ((KEY_SIZE>>1) & 0xf)); */
        state[2][0] ^= 2;   /* (2 ^ ((KEY_SIZE<<3) & 0xf)); */
        state[3][0] ^= 3;   /* (3 ^ ((KEY_SIZE<<3) & 0xf)); */

        tmp = (READ_ROUND_CONSTANT_BYTE(RC[r]) >> 3) & 7;
        state[0][1] ^= tmp;
        state[2][1] ^= tmp;

        tmp = READ_ROUND_CONSTANT_BYTE(RC[r]) & 7;
        state[1][1] ^= tmp;
        state[3][1] ^= tmp;
    }

    void led::invSCShRMCS(uint8_t (*state)[4]) {
        uint8_t c, r;
        uint16_t v;
        uint8_t os[4][4];


        memcpy(os, state, 16);

        for(c = 0; c < 4; c++)
        {
            v = 0;

            for(r = 0; r < 4; r++)
            {
                v ^= READ_INVERSE_ROUND_TABLE_WORD(invRndTab[r][(os[r][c])]);
            }

            for(r = 1; r <= 4; r ++)
            {
                state[4 - r][(c + (4 - r)) & 3] = READ_INVERSE_SBOX_BYTE(invSbox[((uint8_t) v) & 0xF]);
                v >>= 4;
            }
        }
    }


}
