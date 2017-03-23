#ifndef SARMAL_SHA3_H
#define SARMAL_SHA3_H

#include "../../sha3_interface.h"

class Sarmal : public sha3_interface {

#define SARMAL_DEFAULT_ROUNDS_256	16
#define SARMAL_DEFAULT_ROUNDS_384	20

/*typedef unsigned char BitSequence;
typedef unsigned long long DataLength;*/

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;

typedef struct {
	/* hashbitlen + algorithm specific parameters */

	unsigned long long h[8];	/* chaining values */

	unsigned long long c[2];	/* constant values */
	unsigned long long s[4];	/* salt values */
	unsigned long long t;	/* # of hashed bits so far */
	
	BitSequence messageblock[128];	/* 1024-bit message block */

	int remainingbitlen;	/* remaining data length */
	int hashbitlen;		/* hash size */

} hashState;

private:
int sarmalNumRounds256;
int sarmalNumRounds384;
hashState sarmalState;

public:
Sarmal(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
/* prototypes for helper functions */
void HashBlock(hashState *state);
void HashPad(hashState *state);

};

#endif