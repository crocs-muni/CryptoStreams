//
// Created by mhajas on 7/21/18.
//

#include <streams/block/ciphers/lightweight/common/rotations/rot32.h>
#include "lea.h"

namespace block {

    void lea::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != LEA_KEY_SIZE) {
            throw std::runtime_error("LEA function only support key size: " + std::to_string(LEA_KEY_SIZE));
        }

        uint32_t *rk = (uint32_t *) _key;
        uint32_t *t = (uint32_t *) key;
        uint32_t delta[4] = {READ_RAM_DATA_DOUBLE_WORD(DELTA[0]), READ_RAM_DATA_DOUBLE_WORD(DELTA[1]),
                             READ_RAM_DATA_DOUBLE_WORD(DELTA[2]), READ_RAM_DATA_DOUBLE_WORD(DELTA[3])};
        uint32_t ri = 0;
        int32_t i;

        uint32_t t0 = t[0];
        uint32_t t1 = t[1];
        uint32_t t2 = t[2];
        uint32_t t3 = t[3];

        for (i = 0; i < LEA_NUMBER_OF_ROUNDS; ++i, ++ri) {
            uint32_t tmp = delta[i & 3];

            t0 = rot32l1(t0 + tmp);
            t1 = rot32l3(t1 + rot32l1(tmp));
            t2 = rot32l6(t2 + rot32l2(tmp));
            t3 = rot32l11(t3 + rot32l3(tmp));
            delta[i & 3] = rot32l4(tmp);

            rk[ri] = t1; // rk1, rk3, rk5
            rk[++ri] = t3; // rk4
            rk[++ri] = t2; // rk2
            rk[++ri] = t0; // rk0
        }
    }

    void lea::Encrypt(uint8_t *block) {
        uint32_t *blk = (uint32_t *) block;
        uint32_t *rk = (uint32_t *) _key;
        uint8_t i;

        uint32_t b0 = blk[0];
        uint32_t b1 = blk[1];
        uint32_t b2 = blk[2];
        uint32_t b3 = blk[3];

        for (i = 0; i < LEA_NUMBER_OF_ROUNDS; i += 4, rk += 4) {
            b3 = rot32r3((b2 ^ RK(rk, 1)) + (b3 ^ RK(rk, 0)));
            b2 = rot32r5((b1 ^ RK(rk, 2)) + (b2 ^ RK(rk, 0)));
            b1 = rot32l9((b0 ^ RK(rk, 3)) + (b1 ^ RK(rk, 0)));

            rk += 4;
            b0 = rot32r3((b3 ^ RK(rk, 1)) + (b0 ^ RK(rk, 0)));
            b3 = rot32r5((b2 ^ RK(rk, 2)) + (b3 ^ RK(rk, 0)));
            b2 = rot32l9((b1 ^ RK(rk, 3)) + (b2 ^ RK(rk, 0)));

            rk += 4;
            b1 = rot32r3((b0 ^ RK(rk, 1)) + (b1 ^ RK(rk, 0)));
            b0 = rot32r5((b3 ^ RK(rk, 2)) + (b0 ^ RK(rk, 0)));
            b3 = rot32l9((b2 ^ RK(rk, 3)) + (b3 ^ RK(rk, 0)));

            rk += 4;
            b2 = rot32r3((b1 ^ RK(rk, 1)) + (b2 ^ RK(rk, 0)));
            b1 = rot32r5((b0 ^ RK(rk, 2)) + (b1 ^ RK(rk, 0)));
            b0 = rot32l9((b3 ^ RK(rk, 3)) + (b0 ^ RK(rk, 0)));
        }

        blk[0] = b0;
        blk[1] = b1;
        blk[2] = b2;
        blk[3] = b3;
    }

    void lea::Decrypt(uint8_t *block) {
        uint32_t* blk = (uint32_t*) block;
        uint32_t* rk = (uint32_t*) _key;
        int8_t i;

        uint32_t b0 = blk[0];
        uint32_t b1 = blk[1];
        uint32_t b2 = blk[2];
        uint32_t b3 = blk[3];

        for (i = LEA_NUMBER_OF_ROUNDS - 1, rk += 92; i >= 0; i -= 4, rk -= 4) {
            b0 = (rot32r9(b0) - (b3 ^ RK(rk, 3))) ^ RK(rk, 0);
            b1 = (rot32l5(b1) - (b0 ^ RK(rk, 2))) ^ RK(rk, 0);
            b2 = (rot32l3(b2) - (b1 ^ RK(rk, 1))) ^ RK(rk, 0);

            rk -= 4;
            b3 = (rot32r9(b3) - (b2 ^ RK(rk, 3))) ^ RK(rk, 0);
            b0 = (rot32l5(b0) - (b3 ^ RK(rk, 2))) ^ RK(rk, 0);
            b1 = (rot32l3(b1) - (b0 ^ RK(rk, 1))) ^ RK(rk, 0);

            rk -= 4;
            b2 = (rot32r9(b2) - (b1 ^ RK(rk, 3))) ^ RK(rk, 0);
            b3 = (rot32l5(b3) - (b2 ^ RK(rk, 2))) ^ RK(rk, 0);
            b0 = (rot32l3(b0) - (b3 ^ RK(rk, 1))) ^ RK(rk, 0);

            rk -= 4;
            b1 = (rot32r9(b1) - (b0 ^ RK(rk, 3))) ^ RK(rk, 0);
            b2 = (rot32l5(b2) - (b1 ^ RK(rk, 2))) ^ RK(rk, 0);
            b3 = (rot32l3(b3) - (b2 ^ RK(rk, 1))) ^ RK(rk, 0);
        }

        blk[0] = b0;
        blk[1] = b1;
        blk[2] = b2;
        blk[3] = b3;
    }
}
