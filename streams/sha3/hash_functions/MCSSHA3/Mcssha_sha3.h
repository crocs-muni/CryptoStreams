#ifndef MCSSHA3_SHA3_H
#define MCSSHA3_SHA3_H

#include "../../sha3_interface.h"

class Mscsha : public sha3_interface {

// This is maximum hash length in bytes for 512 hash length in bits
#define MCSSHA_LBLOCK       	64

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;

typedef struct MCSSHAstate_st
	{
	DataLength hashbitlen;
	BitSequence x[7];
	BitSequence data[MCSSHA_LBLOCK];
	} MCSSHA_CTX,hashState;

private:
hashState mscShaState;

public:
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *md);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif