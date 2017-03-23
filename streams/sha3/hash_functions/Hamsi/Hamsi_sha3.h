#ifndef HAMSI_SHA3_H
#define HAMSI_SHA3_H

#include "../../sha3_interface.h"

class Hamsi : public sha3_interface {

#define HAMSI_ROUNDS_256		3 /* 224 AND 256 VERSION */
#define HAMSI_ROUNDS_384		6 /* 384 AND 512 VERSION */
#define HAMSI_PF_ROUNDS_256		6
#define HAMSI_PF_ROUNDS_384		12

typedef enum { SUCCESS=0, FAIL=1, BAD_HASHBITLEN=2 } HashReturn;

typedef struct {
    int hashbitlen;
    int leftbits;
    unsigned char leftdata[8];
    unsigned int state[16];     // chain value
    unsigned int counter;

    int cvsize;
    // int blocksize;
    int ROUNDS;
    int PFROUNDS;
} hashState;

private:
int hamsiNumRounds256;
int hamsiNumRounds384;
int hamsiPFNumRounds256;
int hamsiPFNumRounds384;
hashState hamsiState;

public:
Hamsi(const int numRounds);
int	  Init(int hashbitlen);
int Update(const BitSequence* data, DataLength databitlen);
int  Final(BitSequence* hashval);
int   Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif