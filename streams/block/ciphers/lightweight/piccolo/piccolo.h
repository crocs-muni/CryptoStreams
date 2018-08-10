//
// Created by mhajas on 7/21/18.
//

#ifndef CRYPTO_STREAMS_PICCOLO_H
#define CRYPTO_STREAMS_PICCOLO_H

#include <streams/block/ciphers/lightweight/lightweight.h>
#include <streams/block/ciphers/lightweight/common/cipher.h>

#define SBOX_BYTE ROM_DATA_BYTE
#define READ_SBOX_BYTE READ_ROM_DATA_BYTE
#define CON80_DOUBLE_WORD ROM_DATA_DOUBLE_WORD
#define READ_CON80_DOUBLE_WORD READ_ROM_DATA_DOUBLE_WORD
#define GF16_MUL_BYTE ROM_DATA_BYTE
#define READ_GF16_MUL_BYTE READ_ROM_DATA_BYTE

#define PICCOLO_BLOCK_SIZE 8
#define PICCOLO_KEY_SIZE 10
#define PICCOLO_ROUND_KEYS_SIZE 108

#define PICCOLO_NUMBER_OF_ROUNDS 25

namespace block {

    class piccolo : public lightweight<PICCOLO_ROUND_KEYS_SIZE, PICCOLO_BLOCK_SIZE> {
        /* SBOX */
        SBOX_BYTE PICCOLO_SBOX[16] =
                {
                        0x0e, 0x04, 0x0b, 0x02,
                        0x03, 0x08, 0x00, 0x09,
                        0x01, 0x0a, 0x07, 0x0f,
                        0x06, 0x0c, 0x05, 0x0d
                };

        /* GF[2^4] multiplication by 2 */
        GF16_MUL_BYTE GF16_MUL2[16] =
                {
                        0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e,
                        0x03, 0x01, 0x07, 0x05, 0x0b, 0x09, 0x0f, 0x0d
                };

        /* GF[2^4] multiplication by 3 */
        GF16_MUL_BYTE GF16_MUL3[16] =
                {
                        0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09,
                        0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02
                };
        /* Key schedule constants */
        CON80_DOUBLE_WORD CON80[25] =
                {
                        0x293d071c,
                        0x253e1f1a,
                        0x213f1718,
                        0x3d382f16,
                        0x39392714,
                        0x353a3f12,
                        0x313b3710,
                        0x0d344f0e,
                        0x0935470c,
                        0x05365f0a,
                        0x01375708,
                        0x1d306f06,
                        0x19316704,
                        0x15327f02,
                        0x11337700,
                        0x6d2c8f3e,
                        0x692d873c,
                        0x652e9f3a,
                        0x612f9738,
                        0x7d28af36,
                        0x7929a734,
                        0x752abf32,
                        0x712bb730,
                        0x4d24cf2e,
                        0x4925c72c
                };

        /* calculate p0 + p1 + 2*p2 + 3*p3 in GF[2^4] with caract. poly = x^4 + x + 1 */
        uint8_t polyEval(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3);

        uint16_t F(uint16_t x);

        void RP(uint16_t *x0, uint16_t *x1, uint16_t *x2, uint16_t *x3);

    public:
        piccolo(size_t rounds) : lightweight(rounds) {}

        void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override;

        void Encrypt(uint8_t *block) override;

        void Decrypt(uint8_t *block) override;

    };

}


#endif //CRYPTO_STREAMS_PICCOLO_H
