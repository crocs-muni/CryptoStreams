//
// Created by mhajas on 7/21/18.
//

#include <streams/block/ciphers/lightweight/common/cipher.h>
#include "rc5_20.h"

namespace block {

    void rc5_20::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != RC5_20_KEY_SIZE) {
            throw std::runtime_error("RC5_20 function only support key size: " + std::to_string(RC5_20_KEY_SIZE));
        }

        uint32_t L[4];

        uint32_t *RoundKeys = (uint32_t *) _key;


        /* Initialize L, then S, then mix key into S */
        L[3] = 0;
        L[3] = (L[3] << 8) + key[15];
        L[3] = (L[3] << 8) + key[14];
        L[3] = (L[3] << 8) + key[13];
        L[3] = (L[3] << 8) + key[12];
        L[2] = (L[2] << 8) + key[11];
        L[2] = (L[2] << 8) + key[10];
        L[2] = (L[2] << 8) + key[9];
        L[2] = (L[2] << 8) + key[8];
        L[1] = (L[1] << 8) + key[7];
        L[1] = (L[1] << 8) + key[6];
        L[1] = (L[1] << 8) + key[5];
        L[1] = (L[1] << 8) + key[4];
        L[0] = (L[0] << 8) + key[3];
        L[0] = (L[0] << 8) + key[2];
        L[0] = (L[0] << 8) + key[1];
        L[0] = (L[0] << 8) + key[0];


        RoundKeys[0] = RC5_ROTL(READ_S_DOUBLE_WORD(S[0]), 3);
        L[0] = RC5_ROTL(L[0] + RoundKeys[0], RoundKeys[0]);

        RoundKeys[1] = RC5_ROTL(READ_S_DOUBLE_WORD(S[1]) + (RoundKeys[0] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[1] + L[0]), (RoundKeys[1] + L[0]));

        RoundKeys[2] = RC5_ROTL(READ_S_DOUBLE_WORD(S[2]) + (RoundKeys[1] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[2] + L[1]), (RoundKeys[2] + L[1]));

        RoundKeys[3] = RC5_ROTL(READ_S_DOUBLE_WORD(S[3]) + (RoundKeys[2] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[3] + L[2]), (RoundKeys[3] + L[2]));

        RoundKeys[4] = RC5_ROTL(READ_S_DOUBLE_WORD(S[4]) + (RoundKeys[3] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[4] + L[3]), (RoundKeys[4] + L[3]));

        RoundKeys[5] = RC5_ROTL(READ_S_DOUBLE_WORD(S[5]) + (RoundKeys[4] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[5] + L[0]), (RoundKeys[5] + L[0]));

        RoundKeys[6] = RC5_ROTL(READ_S_DOUBLE_WORD(S[6]) + (RoundKeys[5] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[6] + L[1]), (RoundKeys[6] + L[1]));

        RoundKeys[7] = RC5_ROTL(READ_S_DOUBLE_WORD(S[7]) + (RoundKeys[6] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[7] + L[2]), (RoundKeys[7] + L[2]));

        RoundKeys[8] = RC5_ROTL(READ_S_DOUBLE_WORD(S[8]) + (RoundKeys[7] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[8] + L[3]), (RoundKeys[8] + L[3]));

        RoundKeys[9] = RC5_ROTL(READ_S_DOUBLE_WORD(S[9]) + (RoundKeys[8] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[9] + L[0]), (RoundKeys[9] + L[0]));

        RoundKeys[10] = RC5_ROTL(READ_S_DOUBLE_WORD(S[10]) + (RoundKeys[9] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[10] + L[1]), (RoundKeys[10] + L[1]));

        RoundKeys[11] = RC5_ROTL(READ_S_DOUBLE_WORD(S[11]) + (RoundKeys[10] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[11] + L[2]), (RoundKeys[11] + L[2]));

        RoundKeys[12] = RC5_ROTL(READ_S_DOUBLE_WORD(S[12]) + (RoundKeys[11] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[12] + L[3]), (RoundKeys[12] + L[3]));

        RoundKeys[13] = RC5_ROTL(READ_S_DOUBLE_WORD(S[13]) + (RoundKeys[12] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[13] + L[0]), (RoundKeys[13] + L[0]));

        RoundKeys[14] = RC5_ROTL(READ_S_DOUBLE_WORD(S[14]) + (RoundKeys[13] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[14] + L[1]), (RoundKeys[14] + L[1]));

        RoundKeys[15] = RC5_ROTL(READ_S_DOUBLE_WORD(S[15]) + (RoundKeys[14] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[15] + L[2]), (RoundKeys[15] + L[2]));

        RoundKeys[16] = RC5_ROTL(READ_S_DOUBLE_WORD(S[16]) + (RoundKeys[15] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[16] + L[3]), (RoundKeys[16] + L[3]));

        RoundKeys[17] = RC5_ROTL(READ_S_DOUBLE_WORD(S[17]) + (RoundKeys[16] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[17] + L[0]), (RoundKeys[17] + L[0]));

        RoundKeys[18] = RC5_ROTL(READ_S_DOUBLE_WORD(S[18]) + (RoundKeys[17] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[18] + L[1]), (RoundKeys[18] + L[1]));

        RoundKeys[19] = RC5_ROTL(READ_S_DOUBLE_WORD(S[19]) + (RoundKeys[18] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[19] + L[2]), (RoundKeys[19] + L[2]));

        RoundKeys[20] = RC5_ROTL(READ_S_DOUBLE_WORD(S[20]) + (RoundKeys[19] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[20] + L[3]), (RoundKeys[20] + L[3]));

        RoundKeys[21] = RC5_ROTL(READ_S_DOUBLE_WORD(S[21]) + (RoundKeys[20] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[21] + L[0]), (RoundKeys[21] + L[0]));

        RoundKeys[22] = RC5_ROTL(READ_S_DOUBLE_WORD(S[22]) + (RoundKeys[21] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[22] + L[1]), (RoundKeys[22] + L[1]));

        RoundKeys[23] = RC5_ROTL(READ_S_DOUBLE_WORD(S[23]) + (RoundKeys[22] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[23] + L[2]), (RoundKeys[23] + L[2]));

        RoundKeys[24] = RC5_ROTL(READ_S_DOUBLE_WORD(S[24]) + (RoundKeys[23] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[24] + L[3]), (RoundKeys[24] + L[3]));

        RoundKeys[25] = RC5_ROTL(READ_S_DOUBLE_WORD(S[25]) + (RoundKeys[24] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[25] + L[0]), (RoundKeys[25] + L[0]));

        RoundKeys[26] = RC5_ROTL(READ_S_DOUBLE_WORD(S[26]) + (RoundKeys[25] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[26] + L[1]), (RoundKeys[26] + L[1]));

        RoundKeys[27] = RC5_ROTL(READ_S_DOUBLE_WORD(S[27]) + (RoundKeys[26] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[27] + L[2]), (RoundKeys[27] + L[2]));

        RoundKeys[28] = RC5_ROTL(READ_S_DOUBLE_WORD(S[28]) + (RoundKeys[27] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[28] + L[3]), (RoundKeys[28] + L[3]));

        RoundKeys[29] = RC5_ROTL(READ_S_DOUBLE_WORD(S[29]) + (RoundKeys[28] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[29] + L[0]), (RoundKeys[29] + L[0]));

        RoundKeys[30] = RC5_ROTL(READ_S_DOUBLE_WORD(S[30]) + (RoundKeys[29] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[30] + L[1]), (RoundKeys[30] + L[1]));

        RoundKeys[31] = RC5_ROTL(READ_S_DOUBLE_WORD(S[31]) + (RoundKeys[30] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[31] + L[2]), (RoundKeys[31] + L[2]));

        RoundKeys[32] = RC5_ROTL(READ_S_DOUBLE_WORD(S[32]) + (RoundKeys[31] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[32] + L[3]), (RoundKeys[32] + L[3]));

        RoundKeys[33] = RC5_ROTL(READ_S_DOUBLE_WORD(S[33]) + (RoundKeys[32] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[33] + L[0]), (RoundKeys[33] + L[0]));

        RoundKeys[34] = RC5_ROTL(READ_S_DOUBLE_WORD(S[34]) + (RoundKeys[33] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[34] + L[1]), (RoundKeys[34] + L[1]));

        RoundKeys[35] = RC5_ROTL(READ_S_DOUBLE_WORD(S[35]) + (RoundKeys[34] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[35] + L[2]), (RoundKeys[35] + L[2]));

        RoundKeys[36] = RC5_ROTL(READ_S_DOUBLE_WORD(S[36]) + (RoundKeys[35] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[36] + L[3]), (RoundKeys[36] + L[3]));

        RoundKeys[37] = RC5_ROTL(READ_S_DOUBLE_WORD(S[37]) + (RoundKeys[36] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[37] + L[0]), (RoundKeys[37] + L[0]));

        RoundKeys[38] = RC5_ROTL(READ_S_DOUBLE_WORD(S[38]) + (RoundKeys[37] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[38] + L[1]), (RoundKeys[38] + L[1]));

        RoundKeys[39] = RC5_ROTL(READ_S_DOUBLE_WORD(S[39]) + (RoundKeys[38] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[39] + L[2]), (RoundKeys[39] + L[2]));

        RoundKeys[40] = RC5_ROTL(READ_S_DOUBLE_WORD(S[40]) + (RoundKeys[39] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[40] + L[3]), (RoundKeys[40] + L[3]));

        RoundKeys[41] = RC5_ROTL(READ_S_DOUBLE_WORD(S[41]) + (RoundKeys[40] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[41] + L[0]), (RoundKeys[41] + L[0]));

        RoundKeys[0] = RC5_ROTL(RoundKeys[0] + (RoundKeys[41] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[0] + L[1]), (RoundKeys[0] + L[1]));

        RoundKeys[1] = RC5_ROTL(RoundKeys[1] + (RoundKeys[0] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[1] + L[2]), (RoundKeys[1] + L[2]));

        RoundKeys[2] = RC5_ROTL(RoundKeys[2] + (RoundKeys[1] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[2] + L[3]), (RoundKeys[2] + L[3]));

        RoundKeys[3] = RC5_ROTL(RoundKeys[3] + (RoundKeys[2] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[3] + L[0]), (RoundKeys[3] + L[0]));

        RoundKeys[4] = RC5_ROTL(RoundKeys[4] + (RoundKeys[3] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[4] + L[1]), (RoundKeys[4] + L[1]));

        RoundKeys[5] = RC5_ROTL(RoundKeys[5] + (RoundKeys[4] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[5] + L[2]), (RoundKeys[5] + L[2]));

        RoundKeys[6] = RC5_ROTL(RoundKeys[6] + (RoundKeys[5] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[6] + L[3]), (RoundKeys[6] + L[3]));

        RoundKeys[7] = RC5_ROTL(RoundKeys[7] + (RoundKeys[6] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[7] + L[0]), (RoundKeys[7] + L[0]));

        RoundKeys[8] = RC5_ROTL(RoundKeys[8] + (RoundKeys[7] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[8] + L[1]), (RoundKeys[8] + L[1]));

        RoundKeys[9] = RC5_ROTL(RoundKeys[9] + (RoundKeys[8] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[9] + L[2]), (RoundKeys[9] + L[2]));

        RoundKeys[10] = RC5_ROTL(RoundKeys[10] + (RoundKeys[9] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[10] + L[3]), (RoundKeys[10] + L[3]));

        RoundKeys[11] = RC5_ROTL(RoundKeys[11] + (RoundKeys[10] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[11] + L[0]), (RoundKeys[11] + L[0]));

        RoundKeys[12] = RC5_ROTL(RoundKeys[12] + (RoundKeys[11] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[12] + L[1]), (RoundKeys[12] + L[1]));

        RoundKeys[13] = RC5_ROTL(RoundKeys[13] + (RoundKeys[12] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[13] + L[2]), (RoundKeys[13] + L[2]));

        RoundKeys[14] = RC5_ROTL(RoundKeys[14] + (RoundKeys[13] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[14] + L[3]), (RoundKeys[14] + L[3]));

        RoundKeys[15] = RC5_ROTL(RoundKeys[15] + (RoundKeys[14] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[15] + L[0]), (RoundKeys[15] + L[0]));

        RoundKeys[16] = RC5_ROTL(RoundKeys[16] + (RoundKeys[15] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[16] + L[1]), (RoundKeys[16] + L[1]));

        RoundKeys[17] = RC5_ROTL(RoundKeys[17] + (RoundKeys[16] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[17] + L[2]), (RoundKeys[17] + L[2]));

        RoundKeys[18] = RC5_ROTL(RoundKeys[18] + (RoundKeys[17] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[18] + L[3]), (RoundKeys[18] + L[3]));

        RoundKeys[19] = RC5_ROTL(RoundKeys[19] + (RoundKeys[18] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[19] + L[0]), (RoundKeys[19] + L[0]));

        RoundKeys[20] = RC5_ROTL(RoundKeys[20] + (RoundKeys[19] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[20] + L[1]), (RoundKeys[20] + L[1]));

        RoundKeys[21] = RC5_ROTL(RoundKeys[21] + (RoundKeys[20] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[21] + L[2]), (RoundKeys[21] + L[2]));

        RoundKeys[22] = RC5_ROTL(RoundKeys[22] + (RoundKeys[21] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[22] + L[3]), (RoundKeys[22] + L[3]));

        RoundKeys[23] = RC5_ROTL(RoundKeys[23] + (RoundKeys[22] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[23] + L[0]), (RoundKeys[23] + L[0]));

        RoundKeys[24] = RC5_ROTL(RoundKeys[24] + (RoundKeys[23] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[24] + L[1]), (RoundKeys[24] + L[1]));

        RoundKeys[25] = RC5_ROTL(RoundKeys[25] + (RoundKeys[24] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[25] + L[2]), (RoundKeys[25] + L[2]));

        RoundKeys[26] = RC5_ROTL(RoundKeys[26] + (RoundKeys[25] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[26] + L[3]), (RoundKeys[26] + L[3]));

        RoundKeys[27] = RC5_ROTL(RoundKeys[27] + (RoundKeys[26] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[27] + L[0]), (RoundKeys[27] + L[0]));

        RoundKeys[28] = RC5_ROTL(RoundKeys[28] + (RoundKeys[27] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[28] + L[1]), (RoundKeys[28] + L[1]));

        RoundKeys[29] = RC5_ROTL(RoundKeys[29] + (RoundKeys[28] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[29] + L[2]), (RoundKeys[29] + L[2]));

        RoundKeys[30] = RC5_ROTL(RoundKeys[30] + (RoundKeys[29] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[30] + L[3]), (RoundKeys[30] + L[3]));

        RoundKeys[31] = RC5_ROTL(RoundKeys[31] + (RoundKeys[30] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[31] + L[0]), (RoundKeys[31] + L[0]));

        RoundKeys[32] = RC5_ROTL(RoundKeys[32] + (RoundKeys[31] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[32] + L[1]), (RoundKeys[32] + L[1]));

        RoundKeys[33] = RC5_ROTL(RoundKeys[33] + (RoundKeys[32] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[33] + L[2]), (RoundKeys[33] + L[2]));

        RoundKeys[34] = RC5_ROTL(RoundKeys[34] + (RoundKeys[33] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[34] + L[3]), (RoundKeys[34] + L[3]));

        RoundKeys[35] = RC5_ROTL(RoundKeys[35] + (RoundKeys[34] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[35] + L[0]), (RoundKeys[35] + L[0]));

        RoundKeys[36] = RC5_ROTL(RoundKeys[36] + (RoundKeys[35] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[36] + L[1]), (RoundKeys[36] + L[1]));

        RoundKeys[37] = RC5_ROTL(RoundKeys[37] + (RoundKeys[36] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[37] + L[2]), (RoundKeys[37] + L[2]));

        RoundKeys[38] = RC5_ROTL(RoundKeys[38] + (RoundKeys[37] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[38] + L[3]), (RoundKeys[38] + L[3]));

        RoundKeys[39] = RC5_ROTL(RoundKeys[39] + (RoundKeys[38] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[39] + L[0]), (RoundKeys[39] + L[0]));

        RoundKeys[40] = RC5_ROTL(RoundKeys[40] + (RoundKeys[39] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[40] + L[1]), (RoundKeys[40] + L[1]));

        RoundKeys[41] = RC5_ROTL(RoundKeys[41] + (RoundKeys[40] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[41] + L[2]), (RoundKeys[41] + L[2]));

        RoundKeys[0] = RC5_ROTL(RoundKeys[0] + (RoundKeys[41] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[0] + L[3]), (RoundKeys[0] + L[3]));

        RoundKeys[1] = RC5_ROTL(RoundKeys[1] + (RoundKeys[0] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[1] + L[0]), (RoundKeys[1] + L[0]));

        RoundKeys[2] = RC5_ROTL(RoundKeys[2] + (RoundKeys[1] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[2] + L[1]), (RoundKeys[2] + L[1]));

        RoundKeys[3] = RC5_ROTL(RoundKeys[3] + (RoundKeys[2] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[3] + L[2]), (RoundKeys[3] + L[2]));

        RoundKeys[4] = RC5_ROTL(RoundKeys[4] + (RoundKeys[3] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[4] + L[3]), (RoundKeys[4] + L[3]));

        RoundKeys[5] = RC5_ROTL(RoundKeys[5] + (RoundKeys[4] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[5] + L[0]), (RoundKeys[5] + L[0]));

        RoundKeys[6] = RC5_ROTL(RoundKeys[6] + (RoundKeys[5] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[6] + L[1]), (RoundKeys[6] + L[1]));

        RoundKeys[7] = RC5_ROTL(RoundKeys[7] + (RoundKeys[6] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[7] + L[2]), (RoundKeys[7] + L[2]));

        RoundKeys[8] = RC5_ROTL(RoundKeys[8] + (RoundKeys[7] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[8] + L[3]), (RoundKeys[8] + L[3]));

        RoundKeys[9] = RC5_ROTL(RoundKeys[9] + (RoundKeys[8] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[9] + L[0]), (RoundKeys[9] + L[0]));

        RoundKeys[10] = RC5_ROTL(RoundKeys[10] + (RoundKeys[9] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[10] + L[1]), (RoundKeys[10] + L[1]));

        RoundKeys[11] = RC5_ROTL(RoundKeys[11] + (RoundKeys[10] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[11] + L[2]), (RoundKeys[11] + L[2]));

        RoundKeys[12] = RC5_ROTL(RoundKeys[12] + (RoundKeys[11] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[12] + L[3]), (RoundKeys[12] + L[3]));

        RoundKeys[13] = RC5_ROTL(RoundKeys[13] + (RoundKeys[12] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[13] + L[0]), (RoundKeys[13] + L[0]));

        RoundKeys[14] = RC5_ROTL(RoundKeys[14] + (RoundKeys[13] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[14] + L[1]), (RoundKeys[14] + L[1]));

        RoundKeys[15] = RC5_ROTL(RoundKeys[15] + (RoundKeys[14] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[15] + L[2]), (RoundKeys[15] + L[2]));

        RoundKeys[16] = RC5_ROTL(RoundKeys[16] + (RoundKeys[15] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[16] + L[3]), (RoundKeys[16] + L[3]));

        RoundKeys[17] = RC5_ROTL(RoundKeys[17] + (RoundKeys[16] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[17] + L[0]), (RoundKeys[17] + L[0]));

        RoundKeys[18] = RC5_ROTL(RoundKeys[18] + (RoundKeys[17] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[18] + L[1]), (RoundKeys[18] + L[1]));

        RoundKeys[19] = RC5_ROTL(RoundKeys[19] + (RoundKeys[18] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[19] + L[2]), (RoundKeys[19] + L[2]));

        RoundKeys[20] = RC5_ROTL(RoundKeys[20] + (RoundKeys[19] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[20] + L[3]), (RoundKeys[20] + L[3]));

        RoundKeys[21] = RC5_ROTL(RoundKeys[21] + (RoundKeys[20] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[21] + L[0]), (RoundKeys[21] + L[0]));

        RoundKeys[22] = RC5_ROTL(RoundKeys[22] + (RoundKeys[21] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[22] + L[1]), (RoundKeys[22] + L[1]));

        RoundKeys[23] = RC5_ROTL(RoundKeys[23] + (RoundKeys[22] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[23] + L[2]), (RoundKeys[23] + L[2]));

        RoundKeys[24] = RC5_ROTL(RoundKeys[24] + (RoundKeys[23] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[24] + L[3]), (RoundKeys[24] + L[3]));

        RoundKeys[25] = RC5_ROTL(RoundKeys[25] + (RoundKeys[24] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[25] + L[0]), (RoundKeys[25] + L[0]));

        RoundKeys[26] = RC5_ROTL(RoundKeys[26] + (RoundKeys[25] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[26] + L[1]), (RoundKeys[26] + L[1]));

        RoundKeys[27] = RC5_ROTL(RoundKeys[27] + (RoundKeys[26] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[27] + L[2]), (RoundKeys[27] + L[2]));

        RoundKeys[28] = RC5_ROTL(RoundKeys[28] + (RoundKeys[27] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[28] + L[3]), (RoundKeys[28] + L[3]));

        RoundKeys[29] = RC5_ROTL(RoundKeys[29] + (RoundKeys[28] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[29] + L[0]), (RoundKeys[29] + L[0]));

        RoundKeys[30] = RC5_ROTL(RoundKeys[30] + (RoundKeys[29] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[30] + L[1]), (RoundKeys[30] + L[1]));

        RoundKeys[31] = RC5_ROTL(RoundKeys[31] + (RoundKeys[30] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[31] + L[2]), (RoundKeys[31] + L[2]));

        RoundKeys[32] = RC5_ROTL(RoundKeys[32] + (RoundKeys[31] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[32] + L[3]), (RoundKeys[32] + L[3]));

        RoundKeys[33] = RC5_ROTL(RoundKeys[33] + (RoundKeys[32] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[33] + L[0]), (RoundKeys[33] + L[0]));

        RoundKeys[34] = RC5_ROTL(RoundKeys[34] + (RoundKeys[33] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[34] + L[1]), (RoundKeys[34] + L[1]));

        RoundKeys[35] = RC5_ROTL(RoundKeys[35] + (RoundKeys[34] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[35] + L[2]), (RoundKeys[35] + L[2]));

        RoundKeys[36] = RC5_ROTL(RoundKeys[36] + (RoundKeys[35] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[36] + L[3]), (RoundKeys[36] + L[3]));

        RoundKeys[37] = RC5_ROTL(RoundKeys[37] + (RoundKeys[36] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[37] + L[0]), (RoundKeys[37] + L[0]));

        RoundKeys[38] = RC5_ROTL(RoundKeys[38] + (RoundKeys[37] + L[1]), 3);
        L[2] = RC5_ROTL(L[2] + (RoundKeys[38] + L[1]), (RoundKeys[38] + L[1]));

        RoundKeys[39] = RC5_ROTL(RoundKeys[39] + (RoundKeys[38] + L[2]), 3);
        L[3] = RC5_ROTL(L[3] + (RoundKeys[39] + L[2]), (RoundKeys[39] + L[2]));

        RoundKeys[40] = RC5_ROTL(RoundKeys[40] + (RoundKeys[39] + L[3]), 3);
        L[0] = RC5_ROTL(L[0] + (RoundKeys[40] + L[3]), (RoundKeys[40] + L[3]));

        RoundKeys[41] = RC5_ROTL(RoundKeys[41] + (RoundKeys[40] + L[0]), 3);
        L[1] = RC5_ROTL(L[1] + (RoundKeys[41] + L[0]), (RoundKeys[41] + L[0]));

    }

    uint32_t rc5_20::RC5_ROTL(uint32_t x, uint32_t n) {
        n &= WORD_SIZE - 1;

        switch (n) {
            case 17:
                x = ROR(x);
            case 18:
                x = ROR(x);
            case 19:
                x = ROR(x);
            case 20:
                x = ROR(x);
            case 21:
                x = ROR(x);
            case 22:
                x = ROR(x);
            case 23:
                x = ROR(x);
            case 24:
                x = ROR(x);
            case 25:
                x = ROR(x);
            case 26:
                x = ROR(x);
            case 27:
                x = ROR(x);
            case 28:
                x = ROR(x);
            case 29:
                x = ROR(x);
            case 30:
                x = ROR(x);
            case 31:
                x = ROR(x);
                return x;
        }

        switch (n) {
            case 16:
                x = ROL(x);
            case 15:
                x = ROL(x);
            case 14:
                x = ROL(x);
            case 13:
                x = ROL(x);
            case 12:
                x = ROL(x);
            case 11:
                x = ROL(x);
            case 10:
                x = ROL(x);
            case 9:
                x = ROL(x);
            case 8:
                x = ROL(x);
            case 7:
                x = ROL(x);
            case 6:
                x = ROL(x);
            case 5:
                x = ROL(x);
            case 4:
                x = ROL(x);
            case 3:
                x = ROL(x);
            case 2:
                x = ROL(x);
            case 1:
                x = ROL(x);
            case 0:
                break;
        }

        return x;
    }

    uint32_t rc5_20::RC5_ROTR(uint32_t x, uint32_t n) {
        n &= WORD_SIZE - 1;

        switch (n) {
            case 17:
                x = ROL(x);
            case 18:
                x = ROL(x);
            case 19:
                x = ROL(x);
            case 20:
                x = ROL(x);
            case 21:
                x = ROL(x);
            case 22:
                x = ROL(x);
            case 23:
                x = ROL(x);
            case 24:
                x = ROL(x);
            case 25:
                x = ROL(x);
            case 26:
                x = ROL(x);
            case 27:
                x = ROL(x);
            case 28:
                x = ROL(x);
            case 29:
                x = ROL(x);
            case 30:
                x = ROL(x);
            case 31:
                x = ROL(x);
                return x;
        }

        switch (n) {
            case 16:
                x = ROR(x);
            case 15:
                x = ROR(x);
            case 14:
                x = ROR(x);
            case 13:
                x = ROR(x);
            case 12:
                x = ROR(x);
            case 11:
                x = ROR(x);
            case 10:
                x = ROR(x);
            case 9:
                x = ROR(x);
            case 8:
                x = ROR(x);
            case 7:
                x = ROR(x);
            case 6:
                x = ROR(x);
            case 5:
                x = ROR(x);
            case 4:
                x = ROR(x);
            case 3:
                x = ROR(x);
            case 2:
                x = ROR(x);
            case 1:
                x = ROR(x);
            case 0:
                break;
        }

        return x;
    }

    void rc5_20::Encrypt(uint8_t *block) {
        uint32_t *Block = (uint32_t *) block;
        uint32_t *RoundKeys = (uint32_t *) _key;


        Block[0] = Block[0] + READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[0]);
        Block[1] = Block[1] + READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[1]);

        for (auto i = 1; i <= _rounds; i++) {
            Block[0] = RC5_ROTL(Block[0] ^ Block[1], Block[1]) +
                       READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[i * 2]);
            Block[1] = RC5_ROTL(Block[1] ^ Block[0], Block[0]) +
                       READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[(i * 2) + 1]);
        }
    }

    void rc5_20::Decrypt(uint8_t *block) {
        uint32_t *Block = (uint32_t *) block;
        uint32_t *RoundKeys = (uint32_t *) _key;

        for (auto i = _rounds; i >= 1; i--) {
            Block[1] = RC5_ROTR(Block[1] -
                                READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[(i * 2) + 1]),
                                Block[0]) ^ Block[0];
            Block[0] = RC5_ROTR(Block[0] -
                                READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[i * 2]),
                                Block[1]) ^ Block[1];
        }

        Block[1] = Block[1] - READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[1]);
        Block[0] = Block[0] - READ_ROUND_KEY_DOUBLE_WORD(RoundKeys[0]);    }
}