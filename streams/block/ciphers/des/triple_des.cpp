#include "triple_des.h"
#include "des.h"

#include <algorithm>
#include <stdexcept>

namespace block {
void triple_des::keysetup(const std::uint8_t* key, const std::uint32_t keysize) {
    std::copy_n(key, keysize, _ctx.key);
    if (_ctx.en) {
        three_des_key_setup(_ctx.key, _ctx.schedule, DES_ENCRYPT);
    } else {
        three_des_key_setup(_ctx.key, _ctx.schedule, DES_DECRYPT);
    }
}

void triple_des::ivsetup(const std::uint8_t* iv, const std::uint32_t ivsize) {
    throw std::runtime_error("not implemented yet");
}

void triple_des::encrypt(const std::uint8_t* plaintext,
             std::uint8_t* ciphertext) {
    three_des_crypt(plaintext, ciphertext, _ctx.schedule, _rounds);
}

void triple_des::decrypt(const std::uint8_t* ciphertext,
             std::uint8_t* plaintext) {
    three_des_crypt(ciphertext, plaintext, _ctx.schedule, _rounds);
}
} //namespace block
