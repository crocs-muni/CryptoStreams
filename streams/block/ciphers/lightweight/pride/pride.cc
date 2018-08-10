//
// Created by mhajas on 7/21/18.
//

#include <cstring>
#include <streams/block/ciphers/lightweight/common/cipher.h>
#include "pride.h"

namespace block {

    void pride::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != PRIDE_KEY_SIZE) {
            throw std::runtime_error("PRIDE function only support key size: " + std::to_string(PRIDE_KEY_SIZE));
        }

        if (_encrypt) {
            uint8_t i, j;
            memcpy((void*)_key,(void*)key,16);
            for(i=0;i<19;i++) memcpy((void*)(_key+16+8*i),(void*)(key+8),8);
            for(i=0;i<20;i++){
                for(j=0;j<4;j++) _key[8*(i+1)+2*j+1] += READ_ROUND_CONSTANT_BYTE(round_constants[4*i+j]);
            }
        } else {
            uint8_t i, j;
            memcpy((void *) _key, (void *) key, PRIDE_KEY_SIZE);
            for (i = 0; i < (PRIDE_NUMBER_OF_ROUNDS); i++)
                memcpy((void *) (_key + PRIDE_KEY_SIZE + 8 * i), (void *) (key + 8), PRIDE_BLOCK_SIZE);
            for (i = 0; i < PRIDE_NUMBER_OF_ROUNDS; i++) {
                for (j = 0; j < 4; j++)
                    _key[8 * (i + 1) + 2 * j + 1] += READ_ROUND_CONSTANT_BYTE(round_constants[76 - 4 * i + j]);
            }
        }
    }

    void pride::Encrypt(uint8_t *block) {
        uint8_t i;
        uint8_t temp[4];

        for(i=0;i<8;i++) block[i] ^= READ_ROUND_KEY_BYTE(_key[i]);
        for(i=0;i<19;i++) encryption_round_function(block,_key+8*(i+1), temp);
        for(i=0;i<8;i++) block[i] ^= READ_ROUND_KEY_BYTE(_key[i + 160]);
        S_layer(block);
        for(i=0;i<8;i++) block[i] ^= READ_ROUND_KEY_BYTE(_key[i]);
    }

    void pride::Decrypt(uint8_t *block) {
        uint8_t i;
        uint8_t temp[4];

        for(i=0;i<8;i++) block[i] ^= READ_ROUND_KEY_BYTE(_key[i]);
        S_layer(block);
        for(i=1;i<PRIDE_NUMBER_OF_ROUNDS;i++) decryption_round_function(block,_key+PRIDE_BLOCK_SIZE*i, temp);
        for(i=0;i<PRIDE_BLOCK_SIZE;i++) block[i] ^= READ_ROUND_KEY_BYTE(_key[i])^READ_ROUND_KEY_BYTE(_key[160+i]);
    }

    void pride::encryption_round_function(uint8_t *data, uint8_t *rkey, uint8_t *temp) {
        uint8_t i;

        for(i=0;i<8;i++) data[i] ^= READ_ROUND_KEY_BYTE(rkey[i]);

        S_layer(data);
        L_layer(data, temp);
    }

    void pride::decryption_round_function(uint8_t *data, uint8_t *rkey, uint8_t *temp) {
        uint8_t i;

        for(i=0;i<8;i++) data[i] ^= READ_ROUND_KEY_BYTE(rkey[i]);

        L_layerInv(data, temp);
        S_layer(data);
    }
}