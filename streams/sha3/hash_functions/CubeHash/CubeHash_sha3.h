#ifndef CUBEHASH_SHA3_H
#define CUBEHASH_SHA3_H

#include "../../sha3_interface.h"

class Cubehash : public sha3_interface {

//NASTAVENIE RUND:
#define CUBEHASH_ROUNDS 8
#define CUBEHASH_BLOCKBYTES 1

/*typedef unsigned char BitSequence;
typedef unsigned long long DataLength;*/
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;

typedef unsigned int cubehash_myuint32; /* must be exactly 32 bits */

typedef struct {
  int hashbitlen;
  int pos; /* number of bits read into x from current block */
  cubehash_myuint32 x[32];
} hashState;

private:
int cubehashNumRounds;
hashState cubehashState;

public:
Cubehash(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data,
                  DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data,
                DataLength databitlen, BitSequence *hashval);

private:
void transform();

};

#endif