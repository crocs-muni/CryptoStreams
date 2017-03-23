#ifndef DSHA2_SHA3_H
#define DSHA2_SHA3_H

#include <stdlib.h>
#include "../../sha3_interface.h"

class DSHA2 : public sha3_interface {

#define DSHA2_DEFAULT_ROUNDS 17

#define DSHA2_ROTR64(x,n)   (((x) >> (n)) | ((x) << (64 - (n))))
#define DSHA2_ROTR32(x,n)   (((x) >> (n)) | ((x) << (32 - (n))))

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
typedef struct
{
    DataLength databitlen[2];
    unsigned int hashbitlen;
    unsigned int blocksize;
    BitSequence block[129];
	unsigned long hashval[16];
} hashState;

private:
int dsha2NumRounds;
hashState dsha2State;

public:
DSHA2(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int sha32_compile();
int sha64_compile();

};

#endif