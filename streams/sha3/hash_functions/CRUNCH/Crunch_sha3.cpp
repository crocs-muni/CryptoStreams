#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "crunch_type.h"
#include "crunch_224.h"
#include "crunch_256.h"
#include "crunch_384.h"
#include "crunch_512.h"
#include "Crunch_sha3.h"
}

Crunch::Crunch(const int numRounds) {
	if (numRounds == -1) {
		crunchNumRounds224 = CRUNCH224_NBROUND;
		crunchNumRounds256 = CRUNCH256_NBROUND;
		crunchNumRounds384 = CRUNCH384_NBROUND;
		crunchNumRounds512 = CRUNCH512_NBROUND;
	} else {
		crunchNumRounds224 = numRounds;
		crunchNumRounds256 = numRounds;
		crunchNumRounds384 = numRounds;
		crunchNumRounds512 = numRounds;
	}
}

int Crunch::Init(int hashbitlen)
{
	crunchState.hashbitlen=hashbitlen;
	switch(hashbitlen)
	{
		case 224:
			return Crunch_Init_224();
		case 256:
			return Crunch_Init_256();
		case 384:
			return Crunch_Init_384();
		case 512:
			return Crunch_Init_512();
		default:
			return CRUNCH_BAD_HASHBITLEN;
	}
}

int Crunch::Final(BitSequence *hash)
{
	switch(crunchState.hashbitlen)
	{
		case 224:
			return Crunch_Final_224(hash, crunchNumRounds224);
		case 256:
			return Crunch_Final_256(hash, crunchNumRounds256);
		case 384:
			return Crunch_Final_384(hash, crunchNumRounds384);
		case 512:
			return Crunch_Final_512(hash, crunchNumRounds512);
		default:
			return CRUNCH_BAD_HASHBITLEN;
	}
}

int Crunch::Update(const BitSequence *data, DataLength databitlen)
{
	switch(crunchState.hashbitlen)
	{
		case 224:
			return Crunch_Update_224(data,databitlen, crunchNumRounds224);
		case 256:
			return Crunch_Update_256(data,databitlen, crunchNumRounds256);
		case 384:
			return Crunch_Update_384(data,databitlen, crunchNumRounds384);
		case 512:
			return Crunch_Update_512(data,databitlen, crunchNumRounds512);
		default:
			return CRUNCH_BAD_HASHBITLEN;
	}
}

int Crunch::Hash(int hashbitlen,const BitSequence *data,DataLength databitlen,BitSequence *hashval) 
{
	switch(hashbitlen)
	{
		case 224:
			return Crunch_Hash_224(data,databitlen,hashval, crunchNumRounds224);
		case 256:
			return Crunch_Hash_256(data,databitlen,hashval, crunchNumRounds256);
		case 384:
			return Crunch_Hash_384(data,databitlen,hashval, crunchNumRounds384);
		case 512:
			return Crunch_Hash_512(data,databitlen,hashval, crunchNumRounds512);
		default:
			return CRUNCH_BAD_HASHBITLEN;
	}
}