//
// Created by mhajas on 7/8/18.
//

#include "hight.h"

namespace block {


    uint8_t hight::rol(uint8_t x) {
        return ((x << 1) & 0xFF) ^ ((x >> 7) & 0xFF);
    }

    uint8_t hight::ror(uint8_t x) {
        return ((x >> 1) & 0xFF) ^ ((x << 7) & 0xFF);
    }

    uint8_t hight::F1(uint8_t x) {
        uint8_t temp1, temp2, temp3;

        temp1 = ror(x);
        temp1 = ror(temp1);

        temp2 = ror(temp1);
        temp2 = ror(temp2);

        temp3 = ror(temp2);

        return temp1 ^ temp2 ^ temp3;
    }

    uint8_t hight::F0(uint8_t x) {
        uint8_t temp1, temp2, temp3;

        temp1 = rol(x);
        temp2 = rol(temp1);
        temp3 = ror(x);

        return temp1 ^ temp2 ^ temp3;
    }

    void hight::EncryptInitialTransfomation(uint8_t *x, const uint8_t *wk) {
        x[0] = x[0] + READ_ROUND_KEY_BYTE(wk[0]);
        x[2] = x[2] ^ READ_ROUND_KEY_BYTE(wk[1]);
        x[4] = x[4] + READ_ROUND_KEY_BYTE(wk[2]);
        x[6] = x[6] ^ READ_ROUND_KEY_BYTE(wk[3]);
    }

    void hight::EncryptRoundFunction(uint8_t *x, const uint8_t *sk) {
        uint8_t temp6 = x[6];
        uint8_t temp7 = x[7];

        x[7] = x[6];
        x[6] = x[5] + (F1(x[4]) ^ READ_ROUND_KEY_BYTE(sk[2]));
        x[5] = x[4];
        x[4] = x[3] ^ (F0(x[2]) + READ_ROUND_KEY_BYTE(sk[1]));
        x[3] = x[2];
        x[2] = x[1] + (F1(x[0]) ^ READ_ROUND_KEY_BYTE(sk[0]));
        x[1] = x[0];
        x[0] = temp7 ^ (F0(temp6) + READ_ROUND_KEY_BYTE(sk[3]));
    }

    void hight::EncryptFinalTransfomation(uint8_t *x, const uint8_t *wk) {
        uint8_t temp = x[0];

        x[0] = x[1] + READ_ROUND_KEY_BYTE(wk[4]);
        x[1] = x[2];
        x[2] = x[3] ^ READ_ROUND_KEY_BYTE(wk[5]);
        x[3] = x[4];
        x[4] = x[5] + READ_ROUND_KEY_BYTE(wk[6]);
        x[5] = x[6];
        x[6] = x[7] ^ READ_ROUND_KEY_BYTE(wk[7]);
        x[7] = temp;
    }

    void hight::DecryptInitialTransfomation(uint8_t *x, const uint8_t *wk) {
        x[0] = x[0] - READ_ROUND_KEY_BYTE(wk[0]);
        x[2] = x[2] ^ READ_ROUND_KEY_BYTE(wk[1]);
        x[4] = x[4] - READ_ROUND_KEY_BYTE(wk[2]);
        x[6] = x[6] ^ READ_ROUND_KEY_BYTE(wk[3]);
    }

    void hight::DecryptRoundFunction(uint8_t *x, const uint8_t *sk) {
        uint8_t temp0 = x[0];

        x[0] = x[1];
        x[1] = x[2] - (F1(x[0]) ^ READ_ROUND_KEY_BYTE(sk[0]));
        x[2] = x[3];
        x[3] = x[4] ^ (F0(x[2]) + READ_ROUND_KEY_BYTE(sk[1]));
        x[4] = x[5];
        x[5] = x[6] - (F1(x[4]) ^ READ_ROUND_KEY_BYTE(sk[2]));
        x[6] = x[7];
        x[7] = temp0 ^ (F0(x[6]) + READ_ROUND_KEY_BYTE(sk[3]));
    }

    void hight::DecryptFinalTransfomation(uint8_t *x, const uint8_t *wk) {
        uint8_t temp = x[7];

        x[7] = x[6] ^ READ_ROUND_KEY_BYTE(wk[7]);
        x[6] = x[5];
        x[5] = x[4] - READ_ROUND_KEY_BYTE(wk[6]);
        x[4] = x[3];
        x[3] = x[2] ^ READ_ROUND_KEY_BYTE(wk[5]);
        x[2] = x[1];
        x[1] = x[0] - READ_ROUND_KEY_BYTE(wk[4]);
        x[0] = temp;
    }

    void hight::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != HIGHT_KEY_SIZE) {
            throw std::runtime_error("HIGHT function only support key size: " + std::to_string(HIGHT_KEY_SIZE));
        }

        uint8_t i, j, index;

        _key[0] = key[12];
        _key[1] = key[13];
        _key[2] = key[14];
        _key[3] = key[15];

        _key[4] = key[0];
        _key[5] = key[1];
        _key[6] = key[2];
        _key[7] = key[3];

        for (i = 0; i < 8; ++i) {
            for (j = 0; j < 8; ++j) {
                index = (j - i + 8) & 0x07;
                _key[16 * i + j + 8] = key[index] + READ_DELTA_BYTE(_delta[16 * i + j]);
                _key[16 * i + j + 16] = key[index + 8] + READ_DELTA_BYTE(_delta[16 * i + j + 8]);
            }
        }
    }

    void hight::Encrypt(uint8_t *block) {
        uint8_t i;
        uint8_t *prk = _key + 8;

        EncryptInitialTransfomation(block, _key);

        for (i = 0; i < HIGHT_NUMBER_OF_ROUNDS; ++i) {
            EncryptRoundFunction(block, prk);
            prk += 4;
        }

        EncryptFinalTransfomation(block, _key);
    }

    void hight::Decrypt(uint8_t *block) {
        uint8_t i;
        uint8_t *prk = _key + 132;

        DecryptFinalTransfomation(block, _key);

        for(i = 0; i < HIGHT_NUMBER_OF_ROUNDS; ++i) {
            DecryptRoundFunction(block, prk);
            prk -= 4;
        }

        DecryptInitialTransfomation(block, _key);
    }
}
