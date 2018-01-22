#ifndef INUPFIN_H_
#define INUPFIN_H_

/* Header file for the reference implementation of the candidate algorithm. 
 * All data structures use 32 bits integers 
 *
 * We call a 32 bit integer a WORD, and a 64 bit integer a DWORD
 */


//#include "global.h"
/* 256 bits */
#define TIB_STATE_WORDS_256 8  /* Size of the internal state, in 32 bits units */
#define TIB_STATE_BYTES_256 32 /* Size of the internal state, in bytes */
#define TIB_BLOCK_WORDS_256 16 /* Size of the block, in 32 bits units */
#define TIB_BLOCK_BYTES_256 64 /* Size of the block, in bytes */
#define TIB_HASH_BYTES_256  32 /* Length of the final hash, in bytes */
#define TIB_HASH_BITLEN_256 256 /* Length of the final hash, in bits */

/* 224 bits */
#define TIB_STATE_WORDS_224 TIB_STATE_WORDS_256
#define TIB_STATE_BYTES_224 TIB_STATE_BYTES_256
#define TIB_BLOCK_WORDS_224 TIB_BLOCK_WORDS_256
#define TIB_BLOCK_BYTES_224 TIB_BLOCK_BYTES_256
#define TIB_HASH_BYTES_224  28
#define TIB_HASH_BITLEN_224 224

/* 512 bits */
#define TIB_STATE_WORDS_512 16
#define TIB_STATE_BYTES_512 64
#define TIB_BLOCK_WORDS_512 32
#define TIB_BLOCK_BYTES_512 128
#define TIB_HASH_BYTES_512  64
#define TIB_HASH_BITLEN_512 512

/* 512 bits */
#define TIB_STATE_WORDS_384 TIB_STATE_WORDS_512
#define TIB_STATE_BYTES_384 TIB_STATE_BYTES_512
#define TIB_BLOCK_WORDS_384 TIB_BLOCK_WORDS_512
#define TIB_BLOCK_BYTES_384 TIB_BLOCK_BYTES_512
#define TIB_HASH_BYTES_384  48
#define TIB_HASH_BITLEN_384 384

/* TibUpdate224 is the same as TibUpdate256, and TibUpdate384 is the same as TibUpdate512 */
#define TibUpdate224 TibUpdate256
#define TibUpdate384 TibUpdate512

/* Data types mandated by the API (see http://csrc.nist.gov/groups/ST/hash/documents/SHA3-C-API.pdf) */
typedef unsigned char TibBitSequence;
typedef enum { TIB_SUCCESS = 0, TIB_FAIL = 1, TIB_BAD_HASHBITLEN = 2 } TibHashReturn;
typedef unsigned long long TibDataLength;


/* hash state for 224 and 256 bits versions */
typedef struct {
  	unsigned int state[TIB_STATE_WORDS_256];		/* internal state */
  	unsigned int bits_processed[2];
  	unsigned int buffer[2*TIB_BLOCK_WORDS_256];    /* buffer for the block and the previous block */
  	unsigned int *previous_block;
  	unsigned int *data_block;
	unsigned int bits_waiting_for_process;		/* bits awaiting for process in the next call to TibUpdate() or TibFinal() */
} tibHashState256;

/* hash state for 384 and 512 bits versions */
typedef struct {
  	unsigned int state[TIB_STATE_WORDS_512];
  	unsigned int bits_processed[2];
  	unsigned int buffer[2*TIB_BLOCK_WORDS_512];     /* input buffer */
  	unsigned int *previous_block;
  	unsigned int *data_block;
	unsigned int bits_waiting_for_process;
} tibHashState512;


/* This is the struct mandated by the the API (see http://csrc.nist.gov/groups/ST/hash/documents/SHA3-C-API.pdf) */
typedef struct {
	int hashbitlen;
	union {
		tibHashState256 state256[1];
		tibHashState512 state512[1];
	} uu[1];
} tibhashState;

/* prototypes of functions for each bit length */
TibHashReturn TibInit256 (tibHashState256 *state);
TibHashReturn TibUpdate256 (tibHashState256 *state , const TibBitSequence *data, TibDataLength databitlen, const int rounds);
TibHashReturn TibFinal256 (tibHashState256 *state, TibBitSequence *hashval, const int rounds);
TibHashReturn TibHash256(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds);

TibHashReturn TibInit224 (tibHashState256 *state);
TibHashReturn TibFinal224 (tibHashState256 *state, TibBitSequence *hashval, const int rounds);
TibHashReturn TibHash224(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds);

TibHashReturn TibInit512 (tibHashState512 *state);
TibHashReturn TibUpdate512 (tibHashState512 *state , const TibBitSequence *data, TibDataLength databitlen, const int rounds);
TibHashReturn TibFinal512 (tibHashState512 *state, TibBitSequence *hashval, const int rounds);
TibHashReturn TibHash512(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds);

TibHashReturn TibInit384 (tibHashState512 *state);
TibHashReturn TibFinal384 (tibHashState512 *state, TibBitSequence *hashval, const int rounds);
TibHashReturn TibHash384(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds);


#endif /*INUPFIN_H_*/
