#include "rc4.h"
#include <algorithm>


/*********************************************************************
* Filename:   arcfour.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the ARCFOUR encryption algorithm.
              Algorithm specification can be found here:
               * http://en.wikipedia.org/wiki/RC4
*********************************************************************/


namespace stream_ciphers {
namespace others {

#include <stdlib.h>

static const unsigned state_size = 256;

/*********************** FUNCTION DEFINITIONS ***********************/
static void arcfour_key_setup(std::uint8_t state[state_size], const std::uint8_t key[], const int len)
{
    int i, j;
    std::uint8_t t;

    for (i = 0; i < state_size; ++i)
        state[i] = i;
    for (i = 0, j = 0; i < state_size; ++i) {
        j = (j + state[i] + key[i % len]) % 256;
        t = state[i];
        state[i] = state[j];
        state[j] = t;
    }
}

// This does not hold state between calls. It always generates the
// stream starting from the first  output byte.
static void arcfour_generate_stream(std::uint8_t state[], std::uint8_t out[], const size_t len)
{
    int i, j;
    size_t idx;
    std::uint8_t t;

    for (idx = 0, i = 0, j = 0; idx < len; ++idx)  {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;
        t = state[i];
        state[i] = state[j];
        state[j] = t;
        out[idx] = state[(state[i] + state[j]) % 256];
    }
}

void rc4::keysetup(const u8* key, const u32 key_bitsize, const u32 iv_bitsize) {
    _ctx.key = std::make_unique<std::uint8_t[]>(key_bitsize / 8);
    std::copy_n(key, key_bitsize / 8, _ctx.key.get());
    arcfour_key_setup(_ctx.state, _ctx.key.get(), int(key_bitsize / 8));
    if (iv_bitsize > 0)
        throw std::runtime_error("RC4 is not using IV");
}

void rc4::ivsetup(const u8* iv) { }

void rc4::encrypt_bytes(const u8 *plaintext, u8 *ciphertext, const u32 ptx_size) {
    // prepare stream
    auto stream = std::make_unique<std::uint8_t[]>(ptx_size);
    arcfour_generate_stream(_ctx.state, stream.get(), ptx_size);
    // xor it with input
    for (unsigned i = 0; i < ptx_size; ++i) {
        ciphertext[i] = plaintext[i] ^ stream[i];
    }
}

void rc4::decrypt_bytes(const u8* ciphertext, u8* plaintext, const u32 ctx_size) {
    encrypt_bytes(ciphertext, plaintext, ctx_size);
}

} // namespace others
} // namespace stream_ciphers
