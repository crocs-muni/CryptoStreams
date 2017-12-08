/**
 * Source: https://github.com/ralphankele/nsa-cipher-simon/
 */

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

#pragma once

#include <stdlib.h>

#include <vector>

#include "../../block_cipher.h"
#include "simon.h"

namespace block {

//Definitions
//16 (4), 24 (3,4), 32 (3,4), 48 (2,3), 64 (2,3,4)
//#define WORD_SIZE 16
//#define KEY_WORDS 4

//#if (WORD_SIZE == 64)
//    #define WORD_MASK (0xffffffffffffffffull)
//#else
//    #define WORD_MASK ((0x1ull << (WORD_SIZE&63)) - 1)
//#endif

//#define CONST_C ((0xffffffffffffffffull ^ 0x3ull) & WORD_MASK)

//#if (WORD_SIZE == 16)
//    #define ROUNDS (32)
//    #define CONST_J (0)
//#elif (WORD_SIZE == 24)
//    #if (KEY_WORDS == 3)
//        #define ROUNDS (36)
//        #define CONST_J (0)
//    #elif (KEY_WORDS == 4)
//        #define ROUNDS (36)
//        #define CONST_J (1)
//    #endif
//#elif (WORD_SIZE == 32)
//    #if (KEY_WORDS == 3)
//        #define ROUNDS (42)
//        #define CONST_J (2)
//    #elif (KEY_WORDS == 4)
//        #define ROUNDS (44)
//        #define CONST_J (3)
//    #endif
//#elif (WORD_SIZE == 48)
//    #if (KEY_WORDS == 2)
//        #define ROUNDS (52)
//        #define CONST_J (2)
//    #elif (KEY_WORDS == 3)
//        #define ROUNDS (54)
//        #define CONST_J (3)
//    #endif
//#elif (WORD_SIZE == 64)
//    #if (KEY_WORDS == 2)
//        #define ROUNDS (68)
//        #define CONST_J (2)
//    #elif (KEY_WORDS == 3)
//        #define ROUNDS (69)
//        #define CONST_J (3)
//    #elif (KEY_WORDS == 4)
//        #define ROUNDS (72)
//        #define CONST_J (4)
//    #endif
//#endif

class simon : public block_cipher {

    /* Data structures */

    struct simon_ctx {
        simon_ctx(unsigned rounds, unsigned block_bit_len, unsigned key_bit_size)
            : WORD_SIZE(block_bit_len/2)
            , KEY_WORDS(key_bit_size/WORD_SIZE)
            , key(std::max(KEY_WORDS, rounds))
            , z{
                {1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
                {1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
                {1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
                {1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
                {1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
            } {

            if (WORD_SIZE == 64) {
                WORD_MASK = 0xffffffffffffffffull;
            } else {
                WORD_MASK = ((0x1ull << (WORD_SIZE&63)) - 1);
            }
            CONST_C = ((0xffffffffffffffffull ^ 0x3ull) & WORD_MASK);
            if (WORD_SIZE == 16) {
                CONST_J = 0;
            } else if (WORD_SIZE == 24) {
                if (KEY_WORDS == 3) {
                    CONST_J = 0;
                } else if (KEY_WORDS == 4) {
                    CONST_J = 1;
                }
            } else if (WORD_SIZE == 32) {
                if (KEY_WORDS == 3) {
                    CONST_J = 2;
                } else if (KEY_WORDS == 4) {
                    CONST_J = 3;
                }
            } else if (WORD_SIZE == 48) {
                if (KEY_WORDS == 2) {
                    CONST_J = 2;
                } else if (KEY_WORDS == 3) {
                    CONST_J = 3;
                }
            } else if (WORD_SIZE == 64) {
                if (KEY_WORDS == 2) {
                    CONST_J = 2;
                } else if (KEY_WORDS == 3) {
                    CONST_J = 3;
                } else if (KEY_WORDS == 4) {
                    CONST_J = 4;
                }
            }
        }

        int32_t WORD_SIZE;
        uint32_t KEY_WORDS;
        uint64_t WORD_MASK;
        uint64_t CONST_C;
        uint32_t CONST_J;
        std::vector<uint64_t> key;


        uint64_t z[5][62];
    } _ctx;

public:
    simon(std::size_t rounds, std::size_t block_size, std::size_t key_size)
        : block_cipher(rounds)
        , _ctx(unsigned(rounds), unsigned(block_size * 8), unsigned(key_size * 8)) {}

    void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override;

    void ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) override;

    void encrypt(const std::uint8_t* plaintext,
                 std::uint8_t* ciphertext) override;

    void decrypt(const std::uint8_t* ciphertext,
                 std::uint8_t* plaintext) override;

private:
    //Functions
    void keySchedule();
    void encrypt_f(uint64_t* left, uint64_t* right);
    void encryptRounds(uint64_t* left, uint64_t* right, int rounds);
    void decrypt_f(uint64_t* left, uint64_t* right);
    void decryptRounds(uint64_t* left, uint64_t* right, int rounds);

    //Helper Functions
    uint64_t S(uint64_t state, int distance);
    uint64_t F(uint64_t state);

    //Test-Functions
    int test_vectors();
    /*int test_encryption(uint64_t R, uint64_t L);*/
};
}
