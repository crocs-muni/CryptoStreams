//
// Created by mhajas on 7/21/18.
//

#ifndef CRYPTO_STREAMS_RECTANGLE_H
#define CRYPTO_STREAMS_RECTANGLE_H

#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

#define RC_BYTE ROM_DATA_BYTE
#define READ_RC_BYTE READ_ROM_DATA_BYTE

#define RECTANGLE_BLOCK_SIZE 8

#define RECTANGLE_KEY_SIZE_128 16
#define RECTANGLE_KEY_SIZE_80 10

#define RECTANGLE_ROUND_KEYS_SIZE 208

#define RECTANGLE_NUMBER_OF_ROUNDS 25

namespace block {

    class rectangle : public lightweight<RECTANGLE_ROUND_KEYS_SIZE, RECTANGLE_BLOCK_SIZE> {

    protected:
        RC_BYTE RC[25] = {
                0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0b, 0x16,
                0x0c, 0x19, 0x13, 0x07, 0x0f, 0x1f, 0x1e, 0x1c,
                0x18, 0x11, 0x03, 0x06, 0x0d, 0x1b, 0x17, 0x0e,
                0x1d };

    public:
        rectangle(size_t rounds) : lightweight(rounds) {}

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;

    };

    class rectangle_k80 : public rectangle {
    public:
        rectangle_k80(size_t rounds) : rectangle(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;
    };

    class rectangle_k128 : public rectangle {
    public:
        rectangle_k128(size_t rounds) : rectangle(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;
    };

}


#endif //CRYPTO_STREAMS_RECTANGLE_H
