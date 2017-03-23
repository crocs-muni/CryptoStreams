#ifndef NASHA_SHA3_H
#define NASHA_SHA3_H

#include <stdlib.h>
#include "../../sha3_interface.h"
#include "brg_types.h"

class Nasha : public sha3_interface {

#define Nasha_64BIT

/* define the hash functions that you need  */
#define Nasha_   /* for dynamic hash length  */
#define Nasha_224
#define Nasha_256
#ifdef Nasha_64BIT
#define Nasha_384
#define Nasha_512
#endif

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum {SUCCESS=0, FAIL=1, BAD_HASHLEN=2} HashReturn;

#define Nasha224_DIGEST_SIZE  28
#define Nasha224_BLOCK_SIZE   64
#define Nasha256_DIGEST_SIZE  32
#define Nasha256_BLOCK_SIZE   64

typedef union {
        nasha_uint_8t b8[8];
		nasha_uint_16t bit16[4];
		nasha_uint_32t bit32[2];
        nasha_uint_64t bit64;
} B64;

typedef struct {
	nasha_uint_64t count[2];
	nasha_uint_64t M[8];
    nasha_uint_64t H[4];
	nasha_uint_64t hash[4];
} hashState256;


#ifndef Nasha_64BIT
typedef struct
{   union
    { hashState256  hs256[1];
    } uu[1];
	int hashbitlen;
} hashState;
#define Nasha_MAX_DIGEST_SIZE    Nasha256_DIGEST_SIZE

#else

#define Nasha384_DIGEST_SIZE  48
#define Nasha384_BLOCK_SIZE  128
#define Nasha512_DIGEST_SIZE  64
#define Nasha512_BLOCK_SIZE  128
#define Nasha_MAX_DIGEST_SIZE    Nasha512_DIGEST_SIZE

typedef struct {
	nasha_uint_64t count[2];
	nasha_uint_64t M[16];
    nasha_uint_64t H[8];
	nasha_uint_64t hash[8];
} hashState512;

typedef struct
{   union
    { hashState256  hs256[1];
      hashState512  hs512[1];
    } uu[1];
	int hashbitlen;
} hashState;

private:
hashState nashaState;

public:
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void Nasha256_compile(hashState256 *state);
int Nasha224_Init(hashState *state);
int Nasha256_Init(hashState *state);
int Nasha256_Update(hashState256 *state, const BitSequence *data, DataLength databitlen);
int Nasha_Final1(hashState256 *state, BitSequence *hashval, const unsigned int hlen);
int Nasha224_Final(hashState256 *state, BitSequence *hashval);
int Nasha256_Final(hashState256 *state, BitSequence *hashval);
int Nasha224_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);
int Nasha256_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

void Nasha512_compile(hashState512 *state);
int Nasha384_Init(hashState *state);
int Nasha512_Init(hashState *state);
int Nasha512_Update(hashState512 *state, const BitSequence *data, DataLength databitlen);
int Nasha_Final2(hashState512 *state, BitSequence *hashval,const unsigned int hlen);
int Nasha384_Final(hashState512 *state, BitSequence *hashval);
int Nasha512_Final(hashState512 *state, BitSequence *hashval);
int Nasha384_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);
int Nasha512_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

#endif

};

#endif