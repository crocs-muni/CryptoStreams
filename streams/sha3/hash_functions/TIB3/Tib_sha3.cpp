#include "Tib_sha3.h"

int Tib::Init(int hashbitlen){
	switch (hashbitlen){
	case 256: 	tibState.hashbitlen = 256;
				return TibInit256(tibState.uu->state256);
	case 224: 	tibState.hashbitlen = 224;
				return TibInit224(tibState.uu->state256);
	case 512: 	tibState.hashbitlen = 512;
				return TibInit512(tibState.uu->state512);
	case 384: 	tibState.hashbitlen = 384;
				return TibInit384(tibState.uu->state512);
	default: return TIB_BAD_HASHBITLEN;
	}
}

int Tib::Update (const BitSequence *data, DataLength databitlen){
	switch (tibState.hashbitlen){
	case 224:
	case 256: return TibUpdate256(tibState.uu->state256, data, databitlen, tibNumRounds);
	case 384:
	case 512: return TibUpdate512(tibState.uu->state512, data, databitlen, tibNumRounds);
	default: return TIB_BAD_HASHBITLEN;
	}
}

int Tib::Final (BitSequence *hashval){
	switch (tibState.hashbitlen){
	case 256: return TibFinal256(tibState.uu->state256, hashval, tibNumRounds);
	case 224: return TibFinal224(tibState.uu->state256, hashval, tibNumRounds);
	case 512: return TibFinal512(tibState.uu->state512, hashval, tibNumRounds);
	case 384: return TibFinal384(tibState.uu->state512, hashval, tibNumRounds);
	default: return TIB_BAD_HASHBITLEN;
	}
}

int Tib::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval){
	//tibhashState state;
	int status;
	status = Tib::Init(hashbitlen);
	if (status != TIB_SUCCESS) return status;
	status = Tib::Update(data, databitlen);
	if (status != TIB_SUCCESS) return status;
	return Tib::Final(hashval);
}

Tib::Tib(const int numRounds) {
	if (numRounds == -1) {
		tibNumRounds = TIB_DEFAULT_ROUNDS;
	} else {
		tibNumRounds = numRounds;
	}
}