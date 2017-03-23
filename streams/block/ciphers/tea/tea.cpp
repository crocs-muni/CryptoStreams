#include "tea.h"
#include "../common_fun.h"
#include <iostream>

namespace block {

    static const std::uint32_t _delta = 0x9e3779b9;

    void tea::keysetup(const std::uint8_t* key, const std::uint32_t keysize) {
        if (keysize != 16)
            throw std::runtime_error("tea keysize should be 16 B");

        // copy key of std::uint8_t to array of std::uint32_t
        for (int i = 0; i < 4; i++)
            _ctx.key[i] = u8_to_u32_copy(key + 4 * i);
    }

    void tea::ivsetup(const std::uint8_t* iv, const std::uint32_t ivsize) {
        throw std::runtime_error("not implemented yet");
    }

    void
    tea::encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext) {
        std::uint32_t input[2];
        // copy plaintext of std::uint8_t to array of std::uint32_t
        for (int j = 0; j < 2; j++)
            input[j] = u8_to_u32_copy(plaintext + 4 * j);

        std::uint32_t sum = 0;
        for (unsigned j = 0; j < _rounds; j++) {
            sum += _delta;
            input[0] += ((input[1] << 4) + _ctx.key[0]) ^ (input[1] + sum) ^
                        ((input[1] >> 5) + _ctx.key[1]);

            input[1] += ((input[0] << 4) + _ctx.key[2]) ^ (input[0] + sum) ^
                        ((input[0] >> 5) + _ctx.key[3]);
        }
        for (int j = 0; j < 2; j++)
            u32_to_u8_copy(ciphertext + 4 * j, input[j]);
    }

    void
    tea::decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext) {
        std::uint32_t input[2];
        // copy plaintext of std::uint8_t to array of std::uint32_t
        for (int j = 0; j < 2; j++)
            input[j] = u8_to_u32_copy(ciphertext + 4 * j);

        std::uint32_t sum = _delta * _rounds;

        for (unsigned j = 0; j < _rounds; j++) {
            input[1] -= ((input[0] << 4) + _ctx.key[2]) ^ (input[0] + sum) ^
                        ((input[0] >> 5) + _ctx.key[3]);

            input[0] -= ((input[1] << 4) + _ctx.key[0]) ^ (input[1] + sum) ^
                        ((input[1] >> 5) + _ctx.key[1]);
            sum -= _delta;
        }
        for (int j = 0; j < 2; j++)
                u32_to_u8_copy(plaintext + 4 * j, input[j]);
    }
}
