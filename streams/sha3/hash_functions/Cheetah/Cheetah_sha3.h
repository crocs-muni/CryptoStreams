#ifndef CHEETAH_SHA3_H
#define CHEETAH_SHA3_H

#include <string.h>
#include <stdio.h>

#include "../../sha3_interface.h"

class Cheetah : public sha3_interface {

//typedef unsigned char u8;
//typedef unsigned int	u32;
//typedef unsigned long long u64;
//typedef unsigned int salt;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;
/*typedef unsigned char BitSequence;
typedef unsigned long long DataLength;*/
typedef struct {
	int hashbitlen;	
	unsigned int core[8];		
	unsigned long long core64[8];	
	DataLength databitlen;
	DataLength doneLength;
	BitSequence rS[128];
	int lRS;
	int nRows;
} hashState;

#define CHEETAH_MBLEN 1024
#define CHEETAH_DEFAULT_ROUNDS_32 16
#define CHEETAH_DEFAULT_ROUNDS_64 12

private:
int cheetahNumRounds32;
int cheetahNumRounds64;
hashState cheetahState;

public:
Cheetah(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void compressCheetah64(const BitSequence *data, unsigned long long blockCounter);
void compressCheetah32(const BitSequence *data, unsigned long long blockCounter);
void lastBlockPermutation();

};

#endif