/*
 * REFERENCE IMPLEMENTATION OF STREAM CIPHER GRAIN VERSION 1
 *
 * Filename: grain-v1.c
 *
 * Author:
 * Martin Hell
 * Dept. of Information Technology
 * P.O. Box 118
 * SE-221 00 Lund, Sweden,
 * email: martin@it.lth.se
 *
 * Synopsis:
 *  This file contains functions that implement the
 *  stream cipher Grain. It also implements functions
 *  specified by the ECRYPT API.
 *
 *  NOTE: Implementation is not optimized for speed
 *  since the cipher is purely hardware oriented.
 */
#include "ecrypt-sync.h"
#include <iostream>

void ECRYPT_Grain::ECRYPT_init(void) {}
/*
 * Function: grain_keystream
 *
 * Synopsis
 *  Generates a new bit and updates the internal state of the cipher.
 */
u8 grain_keystream(GRAIN_ctx* ctx, int numRounds = 13) {

    u8 i, NBit, LBit, outbit;
    /* Calculate feedback and output bits */

    outbit =
            ctx->NFSR
                    [2]; //^ctx->NFSR[15]^ctx->NFSR[36]^ctx->NFSR[45]^ctx->NFSR[64]^ctx->NFSR[73]^ctx->NFSR[89]^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
    NBit = ctx->LFSR
                   [0];  //^ctx->NFSR[0]^ctx->NFSR[26]^ctx->NFSR[56]^ctx->NFSR[91]^ctx->NFSR[96]^(ctx->NFSR[3]&ctx->NFSR[67])^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
    LBit = ctx->LFSR[0]; //^ctx->LFSR[7]^ctx->LFSR[38]^ctx->LFSR[70]^ctx->LFSR[81]^ctx->LFSR[96];

    if (numRounds > 1) {
        outbit ^=
                ctx->NFSR
                        [15]; //^ctx->NFSR[36]^ctx->NFSR[45]^ctx->NFSR[64]^ctx->NFSR[73]^ctx->NFSR[89]^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= ctx->NFSR
                        [0]; //^ctx->NFSR[26]^ctx->NFSR[56]^ctx->NFSR[91]^ctx->NFSR[96]^(ctx->NFSR[3]&ctx->NFSR[67])^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
    }
    if (numRounds > 2) {
        outbit ^=
                ctx->NFSR
                        [36]; //^ctx->NFSR[45]^ctx->NFSR[64]^ctx->NFSR[73]^ctx->NFSR[89]^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= ctx->NFSR
                        [26]; //^ctx->NFSR[56]^ctx->NFSR[91]^ctx->NFSR[96]^(ctx->NFSR[3]&ctx->NFSR[67])^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
        LBit ^= ctx->LFSR[7]; //^ctx->LFSR[38]^ctx->LFSR[70]^ctx->LFSR[81]^ctx->LFSR[96];
    }
    if (numRounds > 3) {
        outbit ^=
                ctx->NFSR
                        [45]; //^ctx->NFSR[64]^ctx->NFSR[73]^ctx->NFSR[89]^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= ctx->NFSR
                        [56]; //^ctx->NFSR[91]^ctx->NFSR[96]^(ctx->NFSR[3]&ctx->NFSR[67])^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
    }
    if (numRounds > 4) {
        outbit ^=
                ctx->NFSR
                        [64]; //^ctx->NFSR[73]^ctx->NFSR[89]^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= ctx->NFSR
                        [91];  //^ctx->NFSR[96]^(ctx->NFSR[3]&ctx->NFSR[67])^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
        LBit ^= ctx->LFSR[38]; //^ctx->LFSR[70]^ctx->LFSR[81]^ctx->LFSR[96];
    }
    if (numRounds > 5) {
        outbit ^=
                ctx->NFSR
                        [73]; //^ctx->NFSR[89]^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= ctx->NFSR
                        [96]; //^(ctx->NFSR[3]&ctx->NFSR[67])^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
    }
    if (numRounds > 6) {
        outbit ^=
                ctx->NFSR
                        [89]; //^ctx->LFSR[93]^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[3] &
                 ctx->NFSR
                         [67]); //^(ctx->NFSR[11]&ctx->NFSR[13])^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
        LBit ^= ctx->LFSR[70];  //^ctx->LFSR[81]^ctx->LFSR[96];
    }
    if (numRounds > 7) {
        outbit ^=
                ctx->LFSR
                        [93]; //^(ctx->NFSR[12]&ctx->LFSR[8])^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[11] &
                 ctx->NFSR
                         [13]); //^(ctx->NFSR[17]&ctx->NFSR[18])^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
    }
    if (numRounds > 8) {
        outbit ^=
                (ctx->NFSR[12] &
                 ctx->LFSR
                         [8]); //^(ctx->LFSR[13]&ctx->LFSR[20])^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[17] &
                 ctx->NFSR
                         [18]); //^(ctx->NFSR[27]&ctx->NFSR[59])^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
        LBit ^= ctx->LFSR[70];  //^ctx->LFSR[81]^ctx->LFSR[96];
    }
    if (numRounds > 9) {
        outbit ^=
                (ctx->LFSR[13] &
                 ctx->LFSR
                         [20]); //^(ctx->NFSR[95]&ctx->LFSR[42])^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[27] &
                 ctx->NFSR
                         [59]); //^(ctx->NFSR[40]&ctx->NFSR[48])^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
    }
    if (numRounds > 10) {
        outbit ^=
                (ctx->NFSR[95] &
                 ctx->LFSR
                         [42]); //^(ctx->LFSR[60]&ctx->LFSR[79])^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[40] &
                 ctx->NFSR[48]); //^(ctx->NFSR[61]&ctx->NFSR[65])^(ctx->NFSR[68]&ctx->NFSR[84]);
        LBit ^= ctx->LFSR[81];   //^ctx->LFSR[96];
    }
    if (numRounds > 11) {
        outbit ^= (ctx->LFSR[60] & ctx->LFSR[79]); //^(ctx->NFSR[12]&ctx->NFSR[95]&ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[61] & ctx->NFSR[65]);   //^(ctx->NFSR[68]&ctx->NFSR[84]);
    }
    if (numRounds > 12) {
        outbit ^= (ctx->NFSR[12] & ctx->NFSR[95] & ctx->LFSR[95]);
        NBit ^= (ctx->NFSR[68] & ctx->NFSR[84]);
        LBit ^= ctx->LFSR[96];
    }
    /* Update registers */
    for (i = 1; i < (ctx->keysize); ++i) {
        ctx->NFSR[i - 1] = ctx->NFSR[i];
        ctx->LFSR[i - 1] = ctx->LFSR[i];
    }
    ctx->NFSR[(ctx->keysize) - 1] = NBit;
    ctx->LFSR[(ctx->keysize) - 1] = LBit;
    return outbit;
}

/* Functions for the ECRYPT API */

void ECRYPT_Grain::ECRYPT_keysetup(const u8* key,
                                   u32 keysize, /* Key size in bits. */
                                   u32 ivsize)  /* IV size in bits. */
{
    GRAIN_ctx* ctx = &_ctx;
    ctx->p_key = key;
    ctx->keysize = keysize;
    ctx->ivsize = ivsize;
}

/*
 * Function: ECRYPT_ivsetup
 *
 * Synopsis
 *  Load the key and perform initial clockings.
 *
 * Assumptions
 *  The key is 10 bytes and the IV is 8 bytes. The
 *  registers are loaded in the following way:
 *
 *  NFSR[0] = lsb of key[0]
 *  ...
 *  NFSR[7] = msb of key[0]
 *  ...
 *  ...
 *  NFSR[72] = lsb of key[9]
 *  ...
 *  NFSR[79] = msb of key[9]
 *  LFSR[0] = lsb of IV[0]
 *  ...
 *  LFSR[7] = msb of IV[0]
 *  ...
 *  ...
 *  LFSR[56] = lsb of IV[7]
 *  ...
 *  LFSR[63] = msb of IV[7]
 */
void ECRYPT_Grain::ECRYPT_ivsetup(const u8* iv) {
    GRAIN_ctx* ctx = &_ctx;
    u32 i, j;
    u8 outbit;
    /* load registers */
    for (i = 0; i < (ctx->ivsize) / 8; ++i) {
        for (j = 0; j < 8; ++j) {
            ctx->NFSR[i * 8 + j] = ((ctx->p_key[i] >> j) & 1);
            ctx->LFSR[i * 8 + j] = ((iv[i] >> j) & 1);
        }
    }
    for (i = (ctx->ivsize) / 8; i < (ctx->keysize) / 8; ++i) {
        for (j = 0; j < 8; ++j) {
            ctx->NFSR[i * 8 + j] = ((ctx->p_key[i] >> j) & 1);
            ctx->LFSR[i * 8 + j] = 1;
        }
    }
    /* do initial clockings */
    for (i = 0; i < 256; ++i) {
        outbit = grain_keystream(ctx, _rounds);
        ctx->LFSR[127] ^= outbit;
        ctx->NFSR[127] ^= outbit;
    }
}

/*
 * Function: ECRYPT_keystream_bytes
 *
 * Synopsis
 *  Generate keystream in bytes.
 *
 * Assumptions
 *  Bits are generated in order z0,z1,z2,...
 *  The bits are stored in a byte in order:
 *
 *  lsb of keystream[0] = z0
 *  ...
 *  msb of keystream[0] = z7
 *  ...
 *  lsb of keystream[1] = z8
 *  ...
 *  msb of keystream[1] = z15
 *  ...
 *  ...
 *  ...
 *  Example: The bit keystream: 10011100 10110011 ..
 *  corresponds to the byte keystream: 39 cd ..
 */
void ECRYPT_Grain::GRAIN_keystream_bytes(GRAIN_ctx* ctx, u8* keystream, u32 msglen) {
    u32 i, j;
    for (i = 0; i < msglen; ++i) {
        keystream[i] = 0;
        for (j = 0; j < 8; ++j) {
            keystream[i] |= (grain_keystream(ctx, _rounds) << j);
        }
    }
}
void ECRYPT_Grain::ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) {
    GRAIN_ctx* ctx = &_ctx;
    u32 i, j;
    u8 k = 0;
    for (i = 0; i < msglen; ++i) {
        k = 0;
        for (j = 0; j < 8; ++j) {
            k |= (grain_keystream(ctx, _rounds) << j);
        }
        ciphertext[i] = plaintext[i] ^ k;
    }
}

void ECRYPT_Grain::ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) {
    GRAIN_ctx* ctx = &_ctx;
    u32 i, j;
    u8 k = 0;
    for (i = 0; i < msglen; ++i) {
        k = 0;
        for (j = 0; j < 8; ++j) {
            k |= (grain_keystream(ctx, _rounds) << j);
        }
        plaintext[i] = ciphertext[i] ^ k;
    }
}
