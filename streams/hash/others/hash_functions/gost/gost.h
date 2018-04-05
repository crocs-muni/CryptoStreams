// https://github.com/rhash/RHash/blob/master/librhash/gost.h
/* gost.h */
// https://tools.ietf.org/html/rfc5831
// https://tools.ietf.org/html/rfc6986
// GOST("The quick brown fox jumps over the lazy dog") = 77b7fa410c9ac58a25f49bca7d0468c9296529315eaca76bd1a10f376d1f4294
// GOST("") = ce85b99cc46752fffee35cab9a7b0278abb4c2d2055cff685af4912c49490f8d
// GOST("a") = d42c539e367c66e9c88a801f6649349c21871b4344c6a573f849fdce62f314dd

#ifndef GOST_H
#define GOST_H
#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#define gost_block_size 32
#define gost_hash_length 32
#define gost_full_rounds 32

/* algorithm context */
typedef struct gost_ctx
{
    unsigned hash[8];  /* algorithm 256-bit state */
    unsigned sum[8];   /* sum of processed message blocks */
    unsigned char message[gost_block_size]; /* 256-bit buffer for leftovers */
    uint64_t length;   /* number of processed bytes */
    unsigned cryptpro; /* boolean flag, the type of sbox to use */
} gost_ctx;

/* hash functions */

void rhash_gost_init(gost_ctx *ctx);
void rhash_gost_cryptopro_init(gost_ctx *ctx);
void rhash_gost_update(gost_ctx *ctx, const unsigned char* msg, size_t size, unsigned nr);
void rhash_gost_final(gost_ctx *ctx, unsigned char result[32], unsigned nr);

#ifdef GENERATE_GOST_LOOKUP_TABLE
void rhash_gost_init_table(void); /* initialize algorithm static data */
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* GOST_H */