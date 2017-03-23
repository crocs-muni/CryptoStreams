#ifndef KHICHIDI_SHA3_H
#define KHICHIDI_SHA3_H

#include "../../sha3_interface.h"
#include "common.h"

class Khichidi : public sha3_interface {

	typedef struct
	{
		
		KHICHIDI_WORD bitlength; // The algorithm bit length. Must be 224, 256, 384 OR 512
		BitSequence residue[64]; // Contains the data to be processed.
		KHICHIDI_WORD residuelen; // Length of residue
		BitSequence M[64]; // Message being processed
		BitSequence Mdash[64]; //Message being processed - used in last 5 rounds
		BitSequence count[9]; // total date length in bits max 2^64-1
#ifdef EIGHT_BIT
		BitSequence Z[64]; // Hash value
		BitSequence H[64]; // Initial Hash value
#else
		KHICHIDI_WORD Z[16]; // Hash value
		KHICHIDI_WORD H[17]; // Initial hash value
#endif
#ifdef DEBUG
		BitSequence Round[6][130];
#endif
		
	}hashState;

private:
hashState khichidiState;

public:
	int Init(int hashbitlen);
	int Update(const BitSequence *data, DataLength databitlen);
	int Final(BitSequence *hashval);
	int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
#ifndef EIGHT_BIT
	void wordstobytes(KHICHIDI_WORD *in, int inlen, BitSequence *khichidi_out, int *outlen);
#endif
	void digest();
	void hexify(BitSequence *in, int inlen, BitSequence *khichidi_out);
	int bitcpy(BitSequence *dest, int destoffset, const BitSequence *source, int numbits);
	void convertLength(KHICHIDI_WORD *in, BitSequence *khichidi_out);

};

#endif