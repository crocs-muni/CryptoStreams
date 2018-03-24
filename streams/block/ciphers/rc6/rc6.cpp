/**
 * @file rc6.c
 * @brief RC6-32/20 block cipher
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCrypto Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @section Description
 *
 * RC6 is a symmetric key block cipher derived from RC5
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.0
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "rc6.h"

//Fill block of memory
#ifndef cryptoMemset
#include <string.h>
#define cryptoMemset(p, value, length) (void) memset(p, value, length)
#endif

//Copy block of memory
#ifndef cryptoMemcpy
#include <string.h>
#include <algorithm>

#define cryptoMemcpy(dest, src, length) (void) memcpy(dest, src, length)
#endif

#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))

#define ROR32(a, n) (((a) >> (n)) | ((a) << (32 - (n))))

#define STORE32LE(a, p) \
    ((uint8_t *)(p))[0] = ((uint32_t)(a) >> 0) & 0xFFU, \
    ((uint8_t *)(p))[1] = ((uint32_t)(a) >> 8) & 0xFFU, \
    ((uint8_t *)(p))[2] = ((uint32_t)(a) >> 16) & 0xFFU, \
    ((uint8_t *)(p))[3] = ((uint32_t)(a) >> 24) & 0xFFU

#define LOAD32LE(p) ( \
    ((uint32_t)(((uint8_t *)(p))[0]) << 0) | \
    ((uint32_t)(((uint8_t *)(p))[1]) << 8) | \
    ((uint32_t)(((uint8_t *)(p))[2]) << 16) | \
    ((uint32_t)(((uint8_t *)(p))[3]) << 24))


//Check crypto library configuration
#if (RC6_SUPPORT == ENABLED)

//RC6 magic constants
#define P32 0xB7E15163
#define Q32 0x9E3779B9

//Common interface for encryption algorithms
/*
const CipherAlgo rc6CipherAlgo =
        {
                "RC6",
                sizeof(Rc6Context),
                CIPHER_ALGO_TYPE_BLOCK,
                RC6_BLOCK_SIZE,
                (CipherAlgoInit) rc6Init,
                NULL,
                NULL,
                (CipherAlgoEncryptBlock) rc6EncryptBlock,
                (CipherAlgoDecryptBlock) rc6DecryptBlock
        };
*/

/**
 * @brief Initialize a RC6 context using the supplied key
 * @param[in] context Pointer to the RC6 context to initialize
 * @param[in] key Pointer to the key
 * @param[in] keyLen Length of the key
 * @return Error code
 **/

int rc6Init(Rc6Context *context, const uint8_t *key, size_t keyLen, unsigned int rounds)
{
    unsigned int c;
    unsigned int i;
    unsigned int j;
    unsigned int s;
    unsigned int v;
    uint32_t a;
    uint32_t b;

    //Invalid key length?
    if(keyLen > RC6_MAX_KEY_SIZE)
        return 1;

    //Convert the secret key from bytes to words
    cryptoMemset(context->l, 0, RC6_MAX_KEY_SIZE);
    cryptoMemcpy(context->l, key, keyLen);

    //Calculate the length of the key in words
    c = (keyLen > 0) ? (keyLen + 3) / 4 : 1;

    //Initialize the first element of S
    context->s[0] = P32;

    //Initialize array S to a particular fixed pseudo random bit pattern
    for(i = 1; i < (2 * rounds + 4); i++)
        context->s[i] = context->s[i - 1] + Q32;

    //Initialize variables
    i = 0;
    j = 0;
    a = 0;
    b = 0;

    //Number of iterations
    v = 3 * (c > (2 * rounds + 4)? c : (2 * rounds + 4));//std::max(c, (2 * rounds + 4));

    //Key expansion
    for(s = 0; s < v; s++)
    {
        context->s[i] += a + b;
        context->s[i] = ROL32(context->s[i], 3);
        a = context->s[i];

        context->l[j] += a + b;
        context->l[j] = ROL32(context->l[j], (a + b) % 32);
        b = context->l[j];

        if(++i >= (2 * rounds + 4))
            i = 0;
        if(++j >= c)
            j = 0;
    }

    //No error to report
    return 0;
}


/**
 * @brief Encrypt a 16-byte block using RC6 algorithm
 * @param[in] context Pointer to the RC6 context
 * @param[in] input Plaintext block to encrypt
 * @param[out] output Ciphertext block resulting from encryption
 **/

void rc6EncryptBlock(Rc6Context *context, const uint8_t *input, uint8_t *output, unsigned int rounds)
{
    unsigned int i;
    uint32_t t;
    uint32_t u;

    //Load the 4 working registers with the plaintext
    uint32_t a = LOAD32LE(input + 0);
    uint32_t b = LOAD32LE(input + 4);
    uint32_t c = LOAD32LE(input + 8);
    uint32_t d = LOAD32LE(input + 12);

    //First, update B and D
    b += context->s[0];
    d += context->s[1];

    //Apply 20 rounds
    for(i = 1; i <= rounds; i++)
    {
        t = (b * (2 * b + 1));
        t = ROL32(t, 5);

        u = (d * (2 * d + 1));
        u = ROL32(u, 5);

        a ^= t;
        a = ROL32(a, u % 32) + context->s[2 * i];

        c ^= u;
        c = ROL32(c, t % 32) + context->s[2 * i + 1];

        t = a;
        a = b;
        b = c;
        c = d;
        d = t;
    }

    //Update A and C
    a += context->s[2 * rounds + 2];
    c += context->s[2 * rounds + 3];

    //The resulting value is the ciphertext
    STORE32LE(a, output + 0);
    STORE32LE(b, output + 4);
    STORE32LE(c, output + 8);
    STORE32LE(d, output + 12);
}


/**
 * @brief Decrypt a 16-byte block using RC6 algorithm
 * @param[in] context Pointer to the RC6 context
 * @param[in] input Ciphertext block to decrypt
 * @param[out] output Plaintext block resulting from decryption
 **/

void rc6DecryptBlock(Rc6Context *context, const uint8_t *input, uint8_t *output, unsigned int rounds)
{
    unsigned int i;
    uint32_t t;
    uint32_t u;

    //Load the 4 working registers with the ciphertext
    uint32_t a = LOAD32LE(input + 0);
    uint32_t b = LOAD32LE(input + 4);
    uint32_t c = LOAD32LE(input + 8);
    uint32_t d = LOAD32LE(input + 12);

    //First, update C and A
    c -= context->s[2 * rounds + 3];
    a -= context->s[2 * rounds + 2];

    //Apply 20 rounds
    for(i = rounds; i > 0; i--)
    {
        t = d;
        d = c;
        c = b;
        b = a;
        a = t;

        u = (d * (2 * d + 1));
        u = ROL32(u, 5);

        t = (b * (2 * b + 1));
        t = ROL32(t, 5);

        c -= context->s[2 * i + 1];
        c = ROR32(c, t % 32) ^ u;

        a -= context->s[2 * i];
        a = ROR32(a, u % 32) ^ t;
    }

    //Update D and B
    d -= context->s[1];
    b -= context->s[0];

    //The resulting value is the plaintext
    STORE32LE(a, output + 0);
    STORE32LE(b, output + 4);
    STORE32LE(c, output + 8);
    STORE32LE(d, output + 12);
}

#endif