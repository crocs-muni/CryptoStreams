//
// Created by Dusan Klinec on 23/03/2018.
// https://github.com/rhash/RHash/blob/master/librhash/whirlpool.h
// f("") = 19FA61D75522A4669B44E39C1D2E1726C530232130D407F89AFEE0964997F7A7 3E83BE698B288FEBCF88E3E03C4F0757EA8964E59B63D93708B138CC42A66EB3
// f("The quick brown fox jumps over the lazy dog") =  B97DE512E91E3828B40D2B0FDCE9CEB3C4A71F9BEA8D88E75C4FA854DF36725FD2B52EB6544EDCACD6F8BEDDFEA403CB55AE31F03AD62A5EF54E42EE82C3FB35
//

#ifndef EACIRC_STREAMS_WHIRLPOOL_H
#define EACIRC_STREAMS_WHIRLPOOL_H

#include <cstdint>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

#define whirlpool_block_size 64
#define WHIRPOOL_ROUNDS 10

/* algorithm context */
typedef struct whirlpool_ctx
{
    uint64_t hash[8];    /* 512-bit algorithm internal hashing state */
    unsigned char message[whirlpool_block_size]; /* 512-bit buffer to hash */

    /* Note: original algorith uses 256-bit counter, allowing to hash up to
       2^256 bits sized message. For optimization we use here 64-bit counter,
       thus reducing maximal message size to 2^64 bits = 2 Exbibytes = 2^21 TiB) */
    uint64_t length;     /* number of processed bytes */
} whirlpool_ctx;

/* hash functions */

void rhash_whirlpool_init(whirlpool_ctx* ctx);
void rhash_whirlpool_update(whirlpool_ctx* ctx, const unsigned char* msg, size_t size, unsigned nr);
void rhash_whirlpool_final(whirlpool_ctx* ctx, unsigned char* result, unsigned nr);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif //EACIRC_STREAMS_WHIRLPOOL_H
