#include "rc4.h"
#include <algorithm>
#include <core/memory.h>


/*********************************************************************
* Filename:   arcfour.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the ARCFOUR encryption algorithm.
              Algorithm specification can be found here:
               * http://en.wikipedia.org/wiki/RC4
*********************************************************************/


namespace block {

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

void rc4::keysetup(const std::uint8_t* key, const std::uint32_t keysize) {
    _ctx.key = std::make_unique<std::uint8_t[]>(keysize);
    std::copy_n(key, keysize, _ctx.key.get());
    arcfour_key_setup(_ctx.state, _ctx.key.get(), int(keysize));
}

void rc4::ivsetup(const std::uint8_t* iv, const std::uint32_t ivsize) {
    throw std::runtime_error("not implemented yet");
}

void rc4::encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext) {
    // prepare stream
    auto stream = std::make_unique<std::uint8_t[]>(_block_size);
    arcfour_generate_stream(_ctx.state, stream.get(), _block_size);
    // xor it with input
    for (unsigned i = 0; i < _block_size; ++i) {
        ciphertext[i] = plaintext[i] ^ stream[i];
    }
}

void rc4::decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext) {
    encrypt(ciphertext, plaintext);
}

} // namespace block
