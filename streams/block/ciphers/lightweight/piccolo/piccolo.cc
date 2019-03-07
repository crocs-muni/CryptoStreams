//
// Created by mhajas on 7/21/18.
//

#include <iostream>
#include "piccolo.h"

namespace block {

    void piccolo::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != PICCOLO_KEY_SIZE) {
            throw std::runtime_error("PICCOLO function only support key size: " + std::to_string(PICCOLO_KEY_SIZE));
        }

        uint8_t i;
        uint8_t m;
        uint16_t *mk = (uint16_t *) key;
        uint32_t _rk;
        uint16_t *rk = (uint16_t *) _key;
        uint16_t *wk = (uint16_t *) (&_key[100]);

        wk[0] = (mk[0] & 0xff00) | (mk[1] & 0x00ff);
        wk[1] = (mk[1] & 0xff00) | (mk[0] & 0x00ff);
        wk[2] = (mk[4] & 0xff00) | (mk[3] & 0x00ff);
        wk[3] = (mk[3] & 0xff00) | (mk[4] & 0x00ff);

        m = 0;
        for (i = 0; i < PICCOLO_NUMBER_OF_ROUNDS; ++i) {
            _rk = READ_CON80_DOUBLE_WORD(CON80[i]);
            switch (m) {
                case 0:
                case 2:
                    _rk ^= *(uint32_t *) (&mk[2]);
                    break;
                case 3:
                    _rk ^= ((uint32_t) (mk[4]) << 16) | (uint32_t) (mk[4]);
                    break;
                case 1:
                case 4:
                    _rk ^= *(uint32_t *) (&mk[0]);
                    break;
            }
            *(uint32_t *) &rk[2 * i] = _rk;
            if (m == 4) {
                m = 0;
            } else {
                m++;
            }
        }
    }

    void piccolo::Encrypt(uint8_t *block) {
        uint8_t i;
        uint16_t *x3 = (uint16_t *)block;
        uint16_t *x2 = x3 + 1;
        uint16_t *x1 = x3 + 2;
        uint16_t *x0 = x3 + 3;
        uint16_t *rk = (uint16_t *)_key;

        *x2 ^= READ_ROUND_KEY_WORD(rk[51]);
        *x0 ^= READ_ROUND_KEY_WORD(rk[50]);
        for (i = 0; i < _rounds - 1; ++i)
        {
            *x1 = *x1 ^ F(*x0) ^ READ_ROUND_KEY_WORD(rk[2 * i]);
            *x3 = *x3 ^ F(*x2) ^ READ_ROUND_KEY_WORD(rk[2 * i + 1]);
            RP(x0, x1, x2, x3);
        }
        *x1 = *x1 ^ F(*x0) ^ READ_ROUND_KEY_WORD(rk[2*PICCOLO_NUMBER_OF_ROUNDS - 2]);
        *x3 = *x3 ^ F(*x2) ^ READ_ROUND_KEY_WORD(rk[2*PICCOLO_NUMBER_OF_ROUNDS - 1]);
        *x0 ^= READ_ROUND_KEY_WORD(rk[52]);
        *x2 ^= READ_ROUND_KEY_WORD(rk[53]);
    }

    void piccolo::Decrypt(uint8_t *block) {
        uint8_t i;
        uint16_t *x3 = (uint16_t *) block;
        uint16_t *x2 = x3 + 1;
        uint16_t *x1 = x3 + 2;
        uint16_t *x0 = x3 + 3;
        uint16_t *rk = (uint16_t *) _key;

        *x2 ^= READ_ROUND_KEY_WORD(rk[53]);
        *x0 ^= READ_ROUND_KEY_WORD(rk[52]);
        for (i = 0; i < _rounds - 1; ++i) {
            if ((i & 0x01) == 0) {
                *x1 = *x1 ^ F(*x0) ^ READ_ROUND_KEY_WORD(rk[2 * PICCOLO_NUMBER_OF_ROUNDS - 2 * i - 2]);
                *x3 = *x3 ^ F(*x2) ^ READ_ROUND_KEY_WORD(rk[2 * PICCOLO_NUMBER_OF_ROUNDS - 2 * i - 1]);
            } else {
                *x1 = *x1 ^ F(*x0) ^ READ_ROUND_KEY_WORD(rk[2 * PICCOLO_NUMBER_OF_ROUNDS - 2 * i - 1]);
                *x3 = *x3 ^ F(*x2) ^ READ_ROUND_KEY_WORD(rk[2 * PICCOLO_NUMBER_OF_ROUNDS - 2 * i - 2]);
            }
            RP(x0, x1, x2, x3);
        }
        *x1 = *x1 ^ F(*x0) ^ READ_ROUND_KEY_WORD(rk[0]);
        *x3 = *x3 ^ F(*x2) ^ READ_ROUND_KEY_WORD(rk[1]);
        *x0 ^= READ_ROUND_KEY_WORD(rk[50]);
        *x2 ^= READ_ROUND_KEY_WORD(rk[51]);

    }

    uint8_t piccolo::polyEval(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3) {
        /* uint8_t y = p0 ^ p1 ^ gf16_mul2(p2) ^ gf16_mul3(p3); */
        uint8_t y = p0 ^ p1 ^ READ_GF16_MUL_BYTE(GF16_MUL2[p2]) ^ READ_GF16_MUL_BYTE(GF16_MUL3[p3]);

        return y;
    }

    uint16_t piccolo::F(uint16_t x) {
        uint8_t x0;
        uint8_t x1;
        uint8_t x2;
        uint8_t x3;
        uint8_t y0;
        uint8_t y1;
        uint8_t y2;
        uint8_t y3;


        x3 = (x >>  0) & 0x0f;
        x2 = (x >>  4) & 0x0f;
        x1 = (x >>  8) & 0x0f;
        x0 = (x >> 12) & 0x0f;

        x3 = READ_SBOX_BYTE(PICCOLO_SBOX[x3]);
        x2 = READ_SBOX_BYTE(PICCOLO_SBOX[x2]);
        x1 = READ_SBOX_BYTE(PICCOLO_SBOX[x1]);
        x0 = READ_SBOX_BYTE(PICCOLO_SBOX[x0]);

        y0 = polyEval(x2, x3, x0, x1);
        y1 = polyEval(x3, x0, x1, x2);
        y2 = polyEval(x0, x1, x2, x3);
        y3 = polyEval(x1, x2, x3, x0);
        y0 = READ_SBOX_BYTE(PICCOLO_SBOX[y0]);
        y1 = READ_SBOX_BYTE(PICCOLO_SBOX[y1]);
        y2 = READ_SBOX_BYTE(PICCOLO_SBOX[y2]);
        y3 = READ_SBOX_BYTE(PICCOLO_SBOX[y3]);

        return (y0 << 12) | (y1 << 8) | (y2 << 4) | y3;
    }

    void piccolo::RP(uint16_t *x0, uint16_t *x1, uint16_t *x2, uint16_t *x3) {
        uint16_t y0;
        uint16_t y1;
        uint16_t y2;
        uint16_t y3;


        y0 = (*x1 & 0xff00) | (*x3 & 0x00ff);
        y1 = (*x2 & 0xff00) | (*x0 & 0x00ff);
        y2 = (*x3 & 0xff00) | (*x1 & 0x00ff);
        y3 = (*x0 & 0xff00) | (*x2 & 0x00ff);

        *x0 = y0;
        *x1 = y1;
        *x2 = y2;
        *x3 = y3;
    }
}
