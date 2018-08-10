//
// Created by mhajas on 7/21/18.
//

#ifndef CRYPTO_STREAMS_ROAD_RUNNER_H
#define CRYPTO_STREAMS_ROAD_RUNNER_H


#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

#define ROTL(x)   (((x)<<1)|((x)>>7))

#define ROAD_RUNNER_BLOCK_SIZE 8

#define ROAD_RUNNER_KEY_SIZE80 10
#define ROAD_RUNNER_ROUND_KEYS_SIZE80 124

#define ROAD_RUNNER_KEY_SIZE128 16
#define ROAD_RUNNER_ROUND_KEYS_SIZE128 148

#define ROAD_RUNNER_NUMBER_OF_ROUNDS80 10
#define ROAD_RUNNER_NUMBER_OF_ROUNDS128 12


namespace block {

    template <size_t ROAD_RUNNER_KEY_SIZE>
    class road_runner : public lightweight<ROAD_RUNNER_KEY_SIZE, ROAD_RUNNER_BLOCK_SIZE> {

    protected:
        bool _encrypt;
        void rrr_sbox(uint8_t *data)
        {
            uint8_t temp = data[3];
            data[3] &= data[2];
            data[3] ^= data[1];
            data[1] |= data[2];
            data[1] ^= data[0];
            data[0] &= data[3];
            data[0] ^= temp;
            temp &= data[1];
            data[2] ^= temp;
        }

        void rrr_L(uint8_t *data)
        {
            uint8_t temp = data[0];
            temp = ROTL(temp);
            temp ^= data[0];
            temp = ROTL(temp);
            data[0] ^= temp;
        }

        void rrr_SLK(uint8_t *data,uint8_t *roundKey)
        {
            uint8_t i;
            rrr_sbox(data);
            for(i=0;i<4;i++) rrr_L(data+i);
            for(i=0;i<4;i++) data[i] ^= READ_ROUND_KEY_BYTE(roundKey[i]);
        }

        void rrr_enc_dec_round(uint8_t *block, uint8_t *roundKey,uint8_t round)
        {
            uint8_t i, temp[4];

            for(i=0;i<4;i++) temp[i] = block[i];

            rrr_SLK(block,roundKey);
            rrr_SLK(block,roundKey+4);
            block[3] ^= round;
            rrr_SLK(block,roundKey+8);
            rrr_sbox(block);

            for(i=0;i<4;i++) block[i] ^= block[i+4];
            for(i=0;i<4;i++) block[i+4] = temp[i];
        }

    public:
        road_runner(size_t rounds, bool _encrypt) : lightweight<ROAD_RUNNER_KEY_SIZE, ROAD_RUNNER_BLOCK_SIZE>(rounds), _encrypt(_encrypt) {}


    };

    /**
     * RoadRunner with 16B key size
     */
    class road_runner_k128 : public road_runner<ROAD_RUNNER_ROUND_KEYS_SIZE128> {
    public:
        road_runner_k128(size_t rounds, bool encrypt) : road_runner(rounds, encrypt) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;

    };

    /**
     * RoadRunner with 10B key size
     */
    class road_runner_k80 : public road_runner<ROAD_RUNNER_ROUND_KEYS_SIZE80> {

    public:
        road_runner_k80(size_t rounds, bool encrypt) : road_runner(rounds, encrypt) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;

    };
}


#endif //CRYPTO_STREAMS_ROAD_RUNNER_H
