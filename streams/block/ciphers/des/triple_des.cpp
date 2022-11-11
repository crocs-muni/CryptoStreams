#include "triple_des.h"
#include "des.h"

#include <algorithm>
#include <stdexcept>
#include <memory>

namespace block {
bool triple_des_next_bit(const std::uint8_t* key, const unsigned i) {
    const unsigned byte_pos = 20 - (i / 8);
    return key[byte_pos] & (0b1 << (i % 8));
}

void triple_des::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
    if (keysize != 21) {
        throw std::runtime_error("3-DES keysize has to be 21 B long.");
    }

    memset(_ctx.key, 0, sizeof(uint8_t) * 24);
    for (unsigned i = 0; i < 21 * 8; ++i) {
        const unsigned byte_pos = 23 - (i / 7);
        _ctx.key[byte_pos] |= triple_des_next_bit(key, i) << ((i % 7) + 1);
    }
    if (_ctx.en) {
        three_des_key_setup(_ctx.key, _ctx.schedule, DES_ENCRYPT);
    } else {
        three_des_key_setup(_ctx.key, _ctx.schedule, DES_DECRYPT);
    }
}

void triple_des::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
    throw std::runtime_error("not implemented yet");
}

void triple_des::encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext) {
    three_des_crypt(plaintext, ciphertext, _ctx.schedule, unsigned(_rounds));
}

void triple_des::decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext) {
    three_des_crypt(ciphertext, plaintext, _ctx.schedule, unsigned(_rounds));
}
} //namespace block
