//
// Created by mhajas on 7/6/18.
//

#include "chaskey.h"
#include <streams/block/ciphers/lightweight/common/cipher.h>

namespace block {

    uint32_t chaskey::rol(uint32_t x, const uint8_t n) {
        return (x << n) | (x >> (32 - n));
    }

    uint32_t chaskey::ror(uint32_t x, const uint8_t n) {
        return (x >> n) | (x << (32 - n));
    }

    void chaskey::Encrypt(uint8_t *block) {
        uint32_t *v = (uint32_t *) block;
        uint32_t *k = (uint32_t *) _key;
        uint8_t i;

        /* Whitening */
        v[0] ^= READ_ROUND_KEY_DOUBLE_WORD(k[0]);
        v[1] ^= READ_ROUND_KEY_DOUBLE_WORD(k[1]);
        v[2] ^= READ_ROUND_KEY_DOUBLE_WORD(k[2]);
        v[3] ^= READ_ROUND_KEY_DOUBLE_WORD(k[3]);

        /* Chaskey permutation*/
        for (i = 0; i < _rounds; ++i) {
            v[0] += v[1];
            v[1] = rol(v[1], 5);
            v[1] ^= v[0];
            v[0] = rol(v[0], 16);
            v[2] += v[3];
            v[3] = rol(v[3], 8);
            v[3] ^= v[2];
            v[0] += v[3];
            v[3] = rol(v[3], 13);
            v[3] ^= v[0];
            v[2] += v[1];
            v[1] = rol(v[1], 7);
            v[1] ^= v[2];
            v[2] = rol(v[2], 16);
        }

        /* Whitening */
        v[0] ^= READ_ROUND_KEY_DOUBLE_WORD(k[0]);
        v[1] ^= READ_ROUND_KEY_DOUBLE_WORD(k[1]);
        v[2] ^= READ_ROUND_KEY_DOUBLE_WORD(k[2]);
        v[3] ^= READ_ROUND_KEY_DOUBLE_WORD(k[3]);
    }

    void chaskey::Decrypt(uint8_t *block) {
        uint32_t *v = (uint32_t *) block;
        uint32_t *k = (uint32_t *) _key;
        uint8_t i;

        /* Whitening */
        v[0] ^= READ_ROUND_KEY_DOUBLE_WORD(k[0]);
        v[1] ^= READ_ROUND_KEY_DOUBLE_WORD(k[1]);
        v[2] ^= READ_ROUND_KEY_DOUBLE_WORD(k[2]);
        v[3] ^= READ_ROUND_KEY_DOUBLE_WORD(k[3]);

        /* Chaskey permutation */
        for (i = 0; i < _rounds; ++i) {
            v[2] = ror(v[2], 16);
            v[1] ^= v[2];
            v[1] = ror(v[1], 7);
            v[2] -= v[1];
            v[3] ^= v[0];
            v[3] = ror(v[3], 13);
            v[0] -= v[3];
            v[3] ^= v[2];
            v[3] = ror(v[3], 8);
            v[2] -= v[3];
            v[0] = ror(v[0], 16);
            v[1] ^= v[0];
            v[1] = ror(v[1], 5);
            v[0] -= v[1];
        }

        /* Whitening */
        v[0] ^= READ_ROUND_KEY_DOUBLE_WORD(k[0]);
        v[1] ^= READ_ROUND_KEY_DOUBLE_WORD(k[1]);
        v[2] ^= READ_ROUND_KEY_DOUBLE_WORD(k[2]);
        v[3] ^= READ_ROUND_KEY_DOUBLE_WORD(k[3]);
    }

}
