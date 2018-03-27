#include "single_des.h"
#include "des.h"

#include <algorithm>
#include <stdexcept>

namespace block {
void single_des::keysetup(const std::uint8_t* key, const std::uint64_t keysize) {
    std::copy_n(key, keysize, _ctx.key);
    // spread each last bit of 7 bytes into 8 bytes (ignore parity bits)
    _ctx.key[7] = 0;
    for (unsigned i = 0; i < 7; ++i) {
        _ctx.key[7] |= (_ctx.key[i] & 0x1) << i;
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
