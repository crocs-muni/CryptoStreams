//
// Created by Dusan Klinec on 23/03/2018.
// https://homes.esat.kuleuven.be/~bosselae/ripemd160.html
// f("") = 9c1185a5c5e9fc54612808977ee8f548b2258d31
// f("a") = 0bdc9d2d256b3ee9daae347be6f4dc835a467ffe
// f("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = 9b752e45573d4b39f4dbd3323cab82bf63326bfb

/* ripemd-160.h */
#ifndef  RMD160_H
#define  RMD160_H
#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#define ripemd160_block_size 64
#define ripemd160_hash_size  20
#define ripemd160_full_rounds  80

/* algorithm context */
typedef struct ripemd160_ctx
{
    unsigned message[ripemd160_block_size / 4]; /* 512-bit buffer for leftovers */
    uint64_t length;  /* number of processed bytes */
    unsigned hash[5]; /* 160-bit algorithm internal hashing state */
} ripemd160_ctx;

/* hash functions */

void rhash_ripemd160_init(ripemd160_ctx *ctx);
void rhash_ripemd160_update(ripemd160_ctx *ctx, const unsigned char* msg, size_t size, unsigned nr);
void rhash_ripemd160_final(ripemd160_ctx *ctx, unsigned char result[20], unsigned nr);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* RMD160_H */
