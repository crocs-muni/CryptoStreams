//
// Created by mhajas on 7/21/18.
//

#include <cstring>
#include "road_runner.h"

namespace block {

    void road_runner_k128::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != ROAD_RUNNER_KEY_SIZE128) {
            throw std::runtime_error("ROAD-RUNNER function only support key sizes: "
                                     + std::to_string(ROAD_RUNNER_KEY_SIZE128) + ", "
                                     + std::to_string(ROAD_RUNNER_KEY_SIZE80));
        }

        if (_encrypt) {
            uint8_t i;
            memcpy((void*)_key,(void*)key, ROAD_RUNNER_KEY_SIZE128);
            for(i=ROAD_RUNNER_KEY_SIZE128;i<ROAD_RUNNER_ROUND_KEYS_SIZE128-ROAD_RUNNER_KEY_SIZE128;i+=ROAD_RUNNER_KEY_SIZE128) memcpy((void*)(_key+i),(void*)key, ROAD_RUNNER_KEY_SIZE128);
            memcpy((void*)(_key+ROAD_RUNNER_ROUND_KEYS_SIZE128-4),(void*)key, 4);
        } else {
            uint8_t i, j, k = 0, m = 8;
            for(i=0;i<ROAD_RUNNER_NUMBER_OF_ROUNDS128;i++)
            {
                for(j=0;j<12;j++)
                {
                    _key[k] = key[m];
                    k++;
                    m = (m+1)%ROAD_RUNNER_KEY_SIZE128;
                }
                m = (m+8)%ROAD_RUNNER_KEY_SIZE128;
            }
        }
    }

    void road_runner_k128::Decrypt(uint8_t *block) {
        uint8_t i, temp[4] = {0}, key_ctr = 0;
        for(i=0;i<4;i++) block[i] ^= READ_ROUND_KEY_BYTE(this->_key[i+20]);
        for(i=1;i<=_rounds;i++)
        {
            rrr_enc_dec_round(block,this->_key+key_ctr,i);
            key_ctr += 12;
        }
        for(i=0;i<4;i++) temp[i] = block[i];
        for(i=0;i<4;i++) block[i] = block[i+4]^READ_ROUND_KEY_BYTE(this->_key[i+8]);
        for(i=0;i<4;i++) block[i+4] = temp[i];
    }

    void road_runner_k128::Encrypt(uint8_t *block) {
        uint8_t i, temp[4] = {0}, key_ctr = 4;
        for(i=0;i<4;i++) block[i] ^= READ_ROUND_KEY_BYTE(this->_key[i]);
        for(i=_rounds;i>0;i--)
        {
            rrr_enc_dec_round(block,this->_key+key_ctr,i);
            key_ctr += 12;
        }
        for(i=0;i<4;i++) temp[i] = block[i];
        for(i=0;i<4;i++) block[i] = block[i+4]^READ_ROUND_KEY_BYTE(this->_key[i+4]);
        for(i=0;i<4;i++) block[i+4] = temp[i];
    }

    void road_runner_k80::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
        if (keysize != ROAD_RUNNER_KEY_SIZE80) {
            throw std::runtime_error("ROAD-RUNNER function only support key sizes: "
                                     + std::to_string(ROAD_RUNNER_KEY_SIZE128) + ", "
                                     + std::to_string(ROAD_RUNNER_KEY_SIZE80));
        }

        if (_encrypt) {
            uint8_t i;
            memcpy((void*)_key,(void*)key, ROAD_RUNNER_KEY_SIZE80);
            for(i=ROAD_RUNNER_KEY_SIZE80;i<ROAD_RUNNER_ROUND_KEYS_SIZE80-ROAD_RUNNER_KEY_SIZE80;i+=ROAD_RUNNER_KEY_SIZE80) memcpy((void*)(_key+i),(void*)key, ROAD_RUNNER_KEY_SIZE80);
            memcpy((void*)(_key+ROAD_RUNNER_ROUND_KEYS_SIZE80-4),(void*)key, 4);
        } else {
            uint8_t i, j, k = 0, m = 2;
            for(i=0;i<ROAD_RUNNER_NUMBER_OF_ROUNDS80;i++)
            {
                for(j=0;j<12;j++)
                {
                    _key[k] = key[m];
                    k++;
                    m = (m+1)%ROAD_RUNNER_KEY_SIZE80;
                }
                m = (m+6)%ROAD_RUNNER_KEY_SIZE80;
            }
        }
    }

    void road_runner_k80::Encrypt(uint8_t *block) {
        uint8_t i, temp[4] = {0}, key_ctr = 4;
        for(i=0;i<4;i++) block[i] ^= READ_ROUND_KEY_BYTE(this->_key[i]);
        for(i=_rounds;i>0;i--)
        {
            rrr_enc_dec_round(block,this->_key+key_ctr,i);
            key_ctr += 12;
        }
        for(i=0;i<4;i++) temp[i] = block[i];
        for(i=0;i<4;i++) block[i] = block[i+4]^READ_ROUND_KEY_BYTE(this->_key[i+4]);
        for(i=0;i<4;i++) block[i+4] = temp[i];
    }

    void road_runner_k80::Decrypt(uint8_t *block) {
        uint8_t i, temp[4] = {0}, key_ctr = 0;
        for(i=0;i<4;i++) block[i] ^= READ_ROUND_KEY_BYTE(this->_key[i+2]);
        for(i=1;i<=_rounds;i++)
        {
            rrr_enc_dec_round(block,this->_key+key_ctr,i);
            key_ctr += 12;
        }
        for(i=0;i<4;i++) temp[i] = block[i];
        for(i=0;i<4;i++) block[i] = block[i+4]^READ_ROUND_KEY_BYTE(this->_key[i+8]);
        for(i=0;i<4;i++) block[i+4] = temp[i];
    }
}