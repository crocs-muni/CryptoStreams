//
// Created by Dusan Klinec on 23/03/2018.
// https://github.com/rhash/RHash/blob/master/librhash/tiger.h
// https://web.archive.org/web/20160304042512/http://th.informatik.uni-mannheim.de/People/Lucks/papers/Tiger_FSE_v10.pdf
// https://www.esat.kuleuven.be/cosic/publications/article-1138.pdf
// https://en.wikipedia.org/wiki/Tiger_(cryptography)

#ifndef EACIRC_STREAMS_TIGER_H
#define EACIRC_STREAMS_TIGER_H

#include <cstdint>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

#define tiger_block_size 64
#define tiger_hash_length 24
#define tiger_full_rounds 24

/* algorithm context */
typedef struct tiger_ctx
{
    /* the order of the fields slightly influence the algorithm speed */
    uint64_t hash[3]; /* algorithm 192-bit state */
    unsigned char message[tiger_block_size]; /* 512-bit buffer for leftovers */
    uint64_t length;  /* processed message length */
    int tiger2;       /* flag, 1 for Tiger2 algorithm, default is 0 */
} tiger_ctx;

/* hash functions */

void rhash_tiger_init(tiger_ctx *ctx);
void rhash_tiger_update(tiger_ctx *ctx, const unsigned char* msg, size_t size, unsigned nr);
void rhash_tiger_final(tiger_ctx *ctx, unsigned char result[24], unsigned nr);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif //EACIRC_STREAMS_TIGER_H
