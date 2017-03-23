#ifndef SHAMATA_SHA3_H
#define SHAMATA_SHA3_H

#include "../../sha3_interface.h"

class Shamata : public sha3_interface {

/* The followings are for multiplication over GF(2^8) with the primitive polynomial (x^8 + x^7 + x^5 + x + 1) for AES block */
#define SHAMATA_R_f8 0x1b                                                // Primitive polynomial remainder
#define SHAMATA_MULT8_BY_2(x) (((x<<1)&0xff) ^ ((x>>7) == 1 ? SHAMATA_R_f8 : 0)) // Multiplication by x over GF(2^8)
#define SHAMATA_MULT8_BY_3(x) (SHAMATA_MULT8_BY_2(x) ^ x)                        // Multiplication by (x + 1) over GF(2^8)

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum {SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;
typedef enum {MESSAGEBLOCK = 0, PARITYBLOCK = 1} BlockType;

// Data structure
typedef struct {
     unsigned int B[4][4], K[12][4];    // State registers (include chaning value)
	 unsigned int r;                 // Round count of ARF (AES Round Function) for message blocks
	 unsigned int hashbitlen;          // Hash bit length
	 unsigned int remainingdatabitlen; // databitlen%128
	 unsigned int remainingdata[8];    // The last databitlen%128 bits data in the input data of the hash function
	 DataLength databitlen;            // Data bit length
} hashState;

private:
hashState shamataState;

public:
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int Init224(hashState *state, int hashbitlen);
int Init256(hashState *state, int hashbitlen);
int Init384(hashState *state, int hashbitlen);
int Init512(hashState *state, int hashbitlen);

int Update224_256(hashState *state, const BitSequence *data, DataLength databitlen);
int Update384_512(hashState *state, const BitSequence *data, DataLength databitlen);

int Final224_256(hashState *state, BitSequence *hashval);
int Final384_512(hashState *state, BitSequence *hashval);

int FinalUpdate224_256(hashState *state, const BitSequence *data);
int FinalUpdate384_512(hashState *state, const BitSequence *data);

int ProduceOutput(hashState *state, BitSequence *hashval);

};

#endif