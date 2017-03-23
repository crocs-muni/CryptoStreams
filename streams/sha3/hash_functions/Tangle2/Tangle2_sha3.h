#ifndef TANGLE2_SHA3_H
#define TANGLE2_SHA3_H

#include "../../sha3_interface.h"

class Tangle2 : public sha3_interface {

#define TANGLE_DEFAULT_ROUNDS_224	72
#define TANGLE_DEFAULT_ROUNDS_256	80
#define TANGLE_DEFAULT_ROUNDS_384	96
#define TANGLE_DEFAULT_ROUNDS_512	112
#define TANGLE_DEFAULT_ROUNDS_768	128
#define TANGLE_DEFAULT_ROUNDS_1024	144

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;


typedef struct {
	int hashbitlen;				/* digest size in bits */
	unsigned char rounds;					/* number of rounds */
	unsigned int H[32];					/* current digest */
	unsigned char  rdata[1024];			/* remaining data */
	unsigned int rlen;					/* remaining length in bits */
	unsigned long long total;					/* total length in bits */
} hashState;

private:
int tangleNumRounds224;
int	tangleNumRounds256;
int	tangleNumRounds384;
int	tangleNumRounds512;
int	tangleNumRounds768;
int	tangleNumRounds1024;
hashState tangleState;

public:
Tangle2(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void FuncTangle(hashState *state,const BitSequence *message); /* Tangle transform function */

};

#endif
