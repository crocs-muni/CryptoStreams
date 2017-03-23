#ifndef LESAMNTA_SHA3_H
#define LESAMNTA_SHA3_H

#include "../../sha3_interface.h"

class Lesamnta : public sha3_interface {

#define LESAMNTA_DEFAULT_ROUNDS	32

/* LESAMNTA specific definitions. */
#define LESAMNTA256_BLOCK_BIT_LEN 256
#define LESAMNTA512_BLOCK_BIT_LEN 512
#define LESAMNTA256_BLOCK_LEN (LESAMNTA256_BLOCK_BIT_LEN >> 3) /* 32 bytes */
#define LESAMNTA512_BLOCK_LEN (LESAMNTA512_BLOCK_BIT_LEN >> 3) /* 64 bytes */

#define LESAMNTA224_DIGEST_BIT_LEN 224
#define LESAMNTA256_DIGEST_BIT_LEN 256
#define LESAMNTA384_DIGEST_BIT_LEN 384
#define LESAMNTA512_DIGEST_BIT_LEN 512

typedef unsigned char BitSequence;
typedef unsigned long long Datalength;

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;


typedef struct {
	unsigned int bitlen[2]; /* Message length in bits */
	unsigned int rembitlen; /* Length of buffer data to be hashed */
	unsigned int buffer[8]; /* Buffer to be hashed */
	unsigned int hash[8];   /* Hash state */
} hashState256;

typedef struct {
	unsigned int bitlen[4];  /* Message length in bits */
	unsigned int rembitlen;  /* Length of buffer data to be hashed */
	unsigned int buffer[16]; /* Buffer to be hashed */
	unsigned int hash[16];   /* Hash state */
} hashState512;

typedef struct {
	int hashbitlen;
	union {
		hashState256 state256;
		hashState512 state512;
	} state;
} hashState;

private:
int lesamntaNumRounds;
hashState lesamntaState;

public:
Lesamnta(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int Init256();
int Update256(const BitSequence *data, Datalength databitlen);
int Final256(BitSequence *hashval);

int Init512();
int Update512(const BitSequence *data, Datalength databitlen);
int Final512(BitSequence *hashval);

int Init224();
int Update224(const BitSequence *data, Datalength databitlen);
int Final224(BitSequence *hashval);

int Init384();
int Update384(const BitSequence *data, Datalength databitlen);
int Final384(BitSequence *hashval);

static void Output256(unsigned int *H, const unsigned int *M, const int rounds);
static void Compression256(unsigned int *H, const unsigned int *M, const int rounds);

static void Output512(unsigned int *H, const unsigned int *M, const int rounds);
static void Compression512(unsigned int *H, const unsigned int *M, const int rounds);

};

#endif