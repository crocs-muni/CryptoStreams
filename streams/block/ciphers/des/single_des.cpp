#include "single_des.h"
#include "des.h"

#include <algorithm>
#include <stdexcept>
#include <cstring>

namespace block {
bool single_des_next_bit(const std::uint8_t* key, const unsigned i) {
    const unsigned byte_pos = 6 - (i / 8);
    return key[byte_pos] & (0b1 << (i % 8));
}

void single_des::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
    if (keysize != 7) {
        throw std::runtime_error("DES keysize has to be 7 B long.");
    }

    memset(_ctx.key, 0, sizeof(uint8_t) * 8);
    for (unsigned i = 0; i < 7 * 8; ++i) {
        const unsigned byte_pos = 7 - (i / 7);
        _ctx.key[byte_pos] |= single_des_next_bit(key, i) << ((i % 7) + 1);
    }
    if (_ctx.en) {
        des_key_setup(_ctx.key, _ctx.schedule, DES_ENCRYPT);
    } else {
        des_key_setup(_ctx.key, _ctx.schedule, DES_DECRYPT);
    }
}

void single_des::ivsetup(const std::uint8_t* iv, const uint64_t ivsize) {
    throw std::runtime_error("not implemented yet");
}

void single_des::encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext) {
    des_crypt(plaintext, ciphertext, _ctx.schedule, unsigned(_rounds));
}

void single_des::decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext) {
    des_crypt(ciphertext, plaintext, _ctx.schedule, unsigned(_rounds));
}
} //namespace block
