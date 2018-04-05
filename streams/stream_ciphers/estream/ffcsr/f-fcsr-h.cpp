/*
 * F-FCSR-H reference implementation.
 *
 * (c) 2005 FCSR project. This software is provided 'as-is', without
 * any express or implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to no restriction.
 *
 * Technical remarks and questions can be addressed to
 * <cedric.lauradoux@inria.fr>
 */

#ifndef FFCSR_API
#define FFCSRH_EVALUATE
#endif

#include "ffcsrh-sync.h"
#ifdef FFCSRH_EVALUATE
#include <time.h>
#endif

void ECRYPT_FFCSR::ECRYPT_init(void) {}

void ECRYPT_FFCSR::ECRYPT_keysetup(const u8* key,
                                   u32 keyse, /* Key se in bits. */
                                   u32 ivse)  /* IV se in bits. */
{
    FFCSR_ctx* ctx = &_ctx;
    ctx->filter[0] = d0;
    ctx->filter[1] = d1;
    ctx->filter[2] = d2;
    ctx->filter[3] = d3;
    ctx->filter[4] = d4;

    ctx->init[0] = key[9] | key[8] << 8 | key[7] << 16 | key[6] << 24;
    ctx->init[1] = key[5] | key[4] << 8 | key[3] << 16 | key[2] << 24;
    ctx->init[2] = key[1] | key[0] << 8 | 0x00 << 16 | 0x00 << 24;
}

void ECRYPT_FFCSR::ECRYPT_ivsetup(const u8* iv) {
    FFCSR_ctx* ctx = &_ctx;
    u32 i = 0;

    ctx->state[0] = ctx->init[0];
    ctx->state[1] = ctx->init[1];
    ctx->state[2] = ctx->init[2];

    ctx->state[2] ^= iv[9] << 16 | iv[8] << 24;
    ctx->state[3] = iv[7] | iv[6] << 8 | iv[5] << 16 | iv[4] << 24;
    ctx->state[4] = iv[3] | iv[2] << 8 | iv[1] << 16 | iv[0] << 24;

    ctx->carry[0] = 0;
    ctx->carry[1] = 0;
    ctx->carry[2] = 0;
    ctx->carry[3] = 0;
    ctx->carry[4] = 0;

    for (i = 0; i < 160 * (float(_rounds) / 5); i++) {
        FFCSR_clock(ctx, _rounds);
    }
}

void ECRYPT_FFCSR::FFCSR_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
                                       void* ctxa,
                                       const u8* input,
                                       u8* output,
                                       u32 msglen) /* Message length in bytes. */
{
    FFCSR_ctx* ctx = (FFCSR_ctx*)ctxa;
    u32 i;
    for (i = 0; i < msglen; i++) {
        FFCSR_clock(ctx, _rounds);
        output[i] = input[i] ^ FFCSR_filter(ctx, _rounds);
    }
}

/* Update the shift register and the carry register of the FCSR */
void FFCSR_clock(FFCSR_ctx* ctx, int numRounds = 5) {
    u32 feedback;
    u32 buffer[5];

    /* expand the feedback bit */

    feedback = ((int)(ctx->state[0] << (MAXSHIFT))) >> (MAXSHIFT);
    /* shift the state */

    ctx->state[0] = ctx->state[0] >> 1;
    ctx->state[0] |= (ctx->state[1] & 0x00000001) << (MAXSHIFT);

    if (numRounds > 1) {
        ctx->state[1] = ctx->state[1] >> 1;
        ctx->state[1] |= (ctx->state[2] & 0x01) << (MAXSHIFT);
    }
    if (numRounds > 2) {
        ctx->state[2] = ctx->state[2] >> 1;
        ctx->state[2] |= (ctx->state[3] & 0x01) << (MAXSHIFT);
    }
    if (numRounds > 3) {
        ctx->state[3] >>= 1;
        ctx->state[3] |= (ctx->state[4] & 0x01) << (MAXSHIFT);
    }
    if (numRounds > 4) {
        ctx->state[4] >>= 1;
    }
    /* update the register */
    buffer[0] = ctx->state[0] ^ ctx->carry[0];

    if (numRounds > 1)
        buffer[1] = ctx->state[1] ^ ctx->carry[1];
    if (numRounds > 2)
        buffer[2] = ctx->state[2] ^ ctx->carry[2];
    if (numRounds > 3)
        buffer[3] = ctx->state[3] ^ ctx->carry[3];
    if (numRounds > 4)
        buffer[4] = ctx->state[4] ^ ctx->carry[4];

    ctx->carry[0] &= ctx->state[0];
    if (numRounds > 1)
        ctx->carry[1] &= ctx->state[1];
    if (numRounds > 2)
        ctx->carry[2] &= ctx->state[2];
    if (numRounds > 3)
        ctx->carry[3] &= ctx->state[3];
    if (numRounds > 4)
        ctx->carry[4] &= ctx->state[4];

    ctx->carry[0] ^= buffer[0] & (feedback & d0);
    if (numRounds > 1)
        ctx->carry[1] ^= buffer[1] & (feedback & d1);
    if (numRounds > 2)
        ctx->carry[2] ^= buffer[2] & (feedback & d2);
    if (numRounds > 3)
        ctx->carry[3] ^= buffer[3] & (feedback & d3);
    if (numRounds > 4)
        ctx->carry[4] ^= buffer[4] & (feedback & d4);

    buffer[0] ^= feedback & d0;
    if (numRounds > 1)
        buffer[1] ^= feedback & d1;
    if (numRounds > 2)
        buffer[2] ^= feedback & d2;
    if (numRounds > 3)
        buffer[3] ^= feedback & d3;
    if (numRounds > 4)
        buffer[4] ^= feedback & d4;

    ctx->state[0] = buffer[0];
    if (numRounds > 1)
        ctx->state[1] = buffer[1];
    if (numRounds > 2)
        ctx->state[2] = buffer[2];
    if (numRounds > 3)
        ctx->state[3] = buffer[3];
    if (numRounds > 4)
        ctx->state[4] = buffer[4];
}

/* Produce one byte of keystream from the internal state of the register */
u8 FFCSR_filter(FFCSR_ctx* ctx, int numRounds = 5) {
    u32 buffer[5];

    buffer[0] = ctx->filter[0] & ctx->state[0];
    if (numRounds > 1)
        buffer[1] = ctx->filter[1] & ctx->state[1];
    if (numRounds > 2)
        buffer[2] = ctx->filter[2] & ctx->state[2];
    if (numRounds > 3)
        buffer[3] = ctx->filter[3] & ctx->state[3];
    if (numRounds > 4)
        buffer[4] = ctx->filter[4] & ctx->state[4];

    if (numRounds > 1)
        buffer[0] ^= buffer[1];
    if (numRounds > 3)
        buffer[2] ^= buffer[3];
    if (numRounds > 4)
        buffer[0] ^= buffer[2] ^ buffer[4];

    buffer[0] ^= (buffer[0] >> 16);
    buffer[0] ^= (buffer[0] >> 8);
    return (u8)buffer[0];
}

void ECRYPT_FFCSR::ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) {
    FFCSR_process_bytes(0, &_ctx, plaintext, ciphertext, msglen);
}
void ECRYPT_FFCSR::ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) {
    FFCSR_process_bytes(1, &_ctx, ciphertext, plaintext, msglen);
}
