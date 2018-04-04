/*\
\ / Unrolled EnRUPT hash implementing irRUPT32-224, irRUPT32-256, irRUPT64-384 and irRUPT64-512 stream hashing modes of operation for P=2 and s=4
/ \ Designed and implemented by Sean O'Neil
\ / NIST SHA-3 submission by VEST Corporation
/ \ Released to the public domain by the author on November 1, 2008.
\*/

#ifndef _EnRUPT_h_
#define _EnRUPT_h_

#include "portEnRUPT.h"

typedef unsigned char					EnruptBitSequence;
typedef unsigned long long				EnruptDataLength;	/* the largest integer type supported by the environment */
typedef enum _EnruptHashReturn
{
	ENRUPT_SUCCESS = 0, ENRUPT_FAIL = 1, ENRUPT_BAD_HASHBITLEN = 2
} EnruptHashReturn;

typedef int enrupt_iru (void * state, const EnruptBitSequence * data, const EnruptDataLength databitlen);
typedef int enrupt_irf (void * state, EnruptBitSequence * hashval);

typedef struct _enruptHashState
{
#if defined(_MSC_VER)||defined(__INTEL_COMPILER)
	__declspec(align(16))
#endif
	unsigned char						x[160]	/* H<=16, two d accumulators and two last r indexes (it's okay if they roll over 2^W) */
#ifdef __GNUC__
	__attribute__ ((aligned (16)))
#endif
	;
	unsigned char						p[80];	/* maximum 8-word input block with two spare words for the padding */
	int						n;		/* bits in the input block */
	int						hashbitlen; /* hash output size in bits */
	enrupt_iru						*u;		/* hash update function optimized for that size */
	enrupt_irf						*f;		/* hash finale function optimized for that size */
} enruptHashState
;

#endif
