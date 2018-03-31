// Source code base on code from: https://github.com/mmeh/simon-speck-cryptanalysis
// ===========================================================================
// SIMON implementation and cryptanalytic methods
// =========================================================================
// Copyright (c) 2013 Martin M. Lauridsen and Hoda A. Alkhzaimi.

// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include "simon.h"
#include <stdio.h>
#include <algorithm>
#include <stdexcept>
#include <eacirc-core/debug.h>

namespace block {

uint64_t simon::S(uint64_t state, int distance){
    if(distance > _ctx.WORD_SIZE || distance < - _ctx.WORD_SIZE)
        return -1;

    return (distance > 0) ?
    ((state << distance) | (state >> (_ctx.WORD_SIZE-distance))) & _ctx.WORD_MASK :
    ((state >> (-distance)) | (state << (_ctx.WORD_SIZE+distance))) & _ctx.WORD_MASK;
}

uint64_t simon::F(uint64_t state){
    return (S(state, 1) & S(state, 8)) ^ S(state, 2);
}

void simon::keySchedule(){
    uint64_t tmp;
    for(int i = _ctx.KEY_WORDS; i < _rounds; ++i){
        tmp = S(_ctx.key[i-1], -3);
        if(_ctx.KEY_WORDS == 4)
            tmp ^= _ctx.key[i-3];
        tmp ^= S(tmp, -1);
        //_ctx.key[i] = !_ctx.key[i-_ctx.KEY_WORDS] ^ tmp ^ z[_ctx.CONST_J][(i-_ctx.KEY_WORDS) % 62] ^ 3;
        _ctx.key[i] = _ctx.key[i-_ctx.KEY_WORDS] ^ _ctx.z[_ctx.CONST_J][(i-_ctx.KEY_WORDS) % 62] ^ tmp ^ _ctx.CONST_C;
    }

    /*for (int i = 0; i < _rounds; ++i)
        printf("%llx\n", _ctx.key[i]);
    printf("\n\n");*/
}

void simon::encrypt_f(uint64_t* left, uint64_t* right){
    encryptRounds(left, right, _rounds);
}

void simon::encryptRounds(uint64_t* left, uint64_t* right, int rounds) {
    uint64_t tmp;
    for(int i = 0; i < rounds; ++i){
        tmp = *left;
        *left = *right ^ F(*left) ^ _ctx.key[i];
        *right = tmp;
        //printf("L: %llx, R: %llx \n", *left, *right);
    }
}

void simon::decrypt_f(uint64_t* left, uint64_t* right){
    decryptRounds(left, right, _rounds);
}

void simon::decryptRounds(uint64_t* left, uint64_t* right, int rounds){
    uint64_t tmp;
    for(int i = 0; i < rounds; ++i){
        tmp = *right;
        *right = *left ^ F(*right) ^ _ctx.key[_rounds-i-1];
        *left = tmp;
    }
}

/*int simon::test_encryption(uint64_t R, uint64_t L, uint64_t REF_ENC_R, uint64_t REF_ENC_L){
    uint64_t ENC_R, ENC_L;

    ENC_R = R;
    ENC_L = L;
    keySchedule();
    encrypt(ENC_L, ENC_R);
    if(ENC_L != REF_ENC_L || ENC_R != REF_ENC_R)
        return -1;

    decrypt(ENC_L, ENC_R);
    if(ENC_L != L || ENC_R != R)
        return -2;
    return 0;
}*/

/**
 * @brief simon::test_vectors is not used in EAC implementation (but can be used in future
 *      and it was the source of our test vectors implementation)
 * @return
 */
int simon::test_vectors(){
    uint64_t R, L, ENC_R, ENC_L;

    //printf("_ctx.WORD_SIZE: %d, _ctx.KEY_WORDS: %d\n", _ctx.WORD_SIZE, _ctx.KEY_WORDS);


    // Simon32/64
    if (_ctx.WORD_SIZE == 16 && _ctx.KEY_WORDS == 4) {
        _ctx.key[3] = 0x1918;
        _ctx.key[2] = 0x1110;
        _ctx.key[1] = 0x0908;
        _ctx.key[0] = 0x0100;
        L = 0x6565;
        R = 0x6877;
        ENC_L = 0xc69b;
        ENC_R = 0xe9bb;
    }

    // Simon48/72
    if (_ctx.WORD_SIZE == 24 && _ctx.KEY_WORDS == 3) {
        _ctx.key[2] = 0x121110;
        _ctx.key[1] = 0x0a0908;
        _ctx.key[0] = 0x020100;
        L = 0x612067;
        R = 0x6e696c;
        ENC_L = 0xdae5ac;
        ENC_R = 0x292cac;
    }

    // Simon48/96
    if (_ctx.WORD_SIZE == 24 && _ctx.KEY_WORDS == 4) {
        _ctx.key[3] = 0x1a1918;
        _ctx.key[2] = 0x121110;
        _ctx.key[1] = 0x0a0908;
        _ctx.key[0] = 0x020100;
        L = 0x726963;
        R = 0x20646e;
        ENC_L = 0x6e06a5;
        ENC_R = 0xacf156;
    }

    // Simon64/96
    if (_ctx.WORD_SIZE == 32 && _ctx.KEY_WORDS == 3) {
        _ctx.key[2] = 0x13121110;
        _ctx.key[1] = 0x0b0a0908;
        _ctx.key[0] = 0x03020100;
        L = 0x6f722067;
        R = 0x6e696c63;
        ENC_L = 0x5ca2e27f;
        ENC_R = 0x111a8fc8;
    }

    // Simon64/128
    if (_ctx.WORD_SIZE == 32 && _ctx.KEY_WORDS == 4) {
        _ctx.key[3] = 0x1b1a1918;
        _ctx.key[2] = 0x13121110;
        _ctx.key[1] = 0x0b0a0908;
        _ctx.key[0] = 0x03020100;
        L = 0x656b696c;
        R = 0x20646e75;
        ENC_L = 0x44c8fc20;
        ENC_R = 0xb9dfa07a;
    }

    // Simon96/96
    if (_ctx.WORD_SIZE == 48 && _ctx.KEY_WORDS == 2) {
        _ctx.key[1] = 0x0d0c0b0a0908;
        _ctx.key[0] = 0x050403020100;
        L = 0x2072616c6c69;
        R = 0x702065687420;
        ENC_L = 0x602807a462b4;
        ENC_R = 0x69063d8ff082;
    }

    // Simon96/144
    if (_ctx.WORD_SIZE == 48 && _ctx.KEY_WORDS == 3) {
        _ctx.key[2] = 0x151413121110;
        _ctx.key[1] = 0x0d0c0b0a0908;
        _ctx.key[0] = 0x050403020100;
        L = 0x746168742074;
        R = 0x73756420666f;
        ENC_L = 0xecad1c6c451e;
        ENC_R = 0x3f59c5db1ae9;
    }

    // Simon128/128
    if (_ctx.WORD_SIZE == 64 && _ctx.KEY_WORDS == 2) {
        _ctx.key[1] = 0x0f0e0d0c0b0a0908;
        _ctx.key[0] = 0x0706050403020100;
        L = 0x6373656420737265;
        R = 0x6c6c657661727420;
        ENC_L = 0x49681b1e1e54fe3f;
        ENC_R = 0x65aa832af84e0bbc;
    }

    // Simon128/192
    if (_ctx.WORD_SIZE == 64 && _ctx.KEY_WORDS == 3) {
        _ctx.key[2] = 0x1716151413121110;
        _ctx.key[1] = 0x0f0e0d0c0b0a0908;
        _ctx.key[0] = 0x0706050403020100;
        L = 0x206572656874206e;
        R = 0x6568772065626972;
        ENC_L = 0xc4ac61effcdc0d4f;
        ENC_R = 0x6c9c8d6e2597b85b;
    }

    // Simon128/256
    if (_ctx.WORD_SIZE == 64 && _ctx.KEY_WORDS == 4) {
        _ctx.key[3] = 0x1f1e1d1c1b1a1918;
        _ctx.key[2] = 0x1716151413121110;
        _ctx.key[1] = 0x0f0e0d0c0b0a0908;
        _ctx.key[0] = 0x0706050403020100;
        L = 0x74206e69206d6f6f;
        R = 0x6d69732061207369;
        ENC_L = 0x8d2b5579afc8a3a0;
        ENC_R = 0x3bf72a87efe7b868;
    }

    keySchedule();

    //printf("L: %llx, R: %llx \n", L, R);
    encrypt_f(&L, &R);

    if(L != ENC_L || R != ENC_R)
        return -1;
    return 0;
}

void simon::keysetup(const std::uint8_t* key, const uint64_t keysize) {
    unsigned word_byte_size = _ctx.WORD_SIZE/8;
    ASSERT(keysize == _ctx.KEY_WORDS * word_byte_size);
    for (unsigned words_read = 0; words_read < _ctx.KEY_WORDS; ++words_read) {
        for (unsigned j = 0; j < word_byte_size; ++j) {
            _ctx.key[words_read] <<= 8;
            _ctx.key[words_read] += key[words_read*word_byte_size + j];
        }
    }
    keySchedule();
}

void simon::ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) {
    throw std::runtime_error("not implemented yet");
}

void simon::encrypt(const std::uint8_t* plaintext,
             std::uint8_t* ciphertext) {
    std::uint64_t left = 0;
    std::uint64_t right = 0;
    unsigned word_byte_size = _ctx.WORD_SIZE/8;
    for (unsigned i = 0; i < word_byte_size; ++i) {
        left <<= 8;
        left += plaintext[i];
        right <<= 8;
        right += plaintext[word_byte_size+i];
    }
    encrypt_f(&left, &right);
    for (int i = word_byte_size-1; i > -1; --i) {
        ciphertext[i] = uint8_t(left);
        left >>= 8;
        ciphertext[word_byte_size+i] = uint8_t(right);
        right >>= 8;
    }
}

void simon::decrypt(const std::uint8_t* ciphertext,
             std::uint8_t* plaintext) {
    std::uint64_t left = 0;
    std::uint64_t right = 0;
    unsigned word_byte_size = _ctx.WORD_SIZE/8;
    for (unsigned i = 0; i < word_byte_size; ++i) {
        left <<= 8;
        left += ciphertext[i];
        right <<= 8;
        right += ciphertext[word_byte_size+i];
    }
    decrypt_f(&left, &right);
    for (int i = word_byte_size-1; i > -1; --i) {
        plaintext[i] = uint8_t(left);
        left >>= 8;
        plaintext[word_byte_size+i] = uint8_t(right);
        right >>= 8;
    }
}

} // namespace block
