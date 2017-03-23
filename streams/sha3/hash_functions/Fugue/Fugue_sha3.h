#ifndef FUGUE_SHA3_H
#define FUGUE_SHA3_H

#include "../../sha3_interface.h"
#include "fugue_t.h"

class Fugue : public sha3_interface {

#define FUGUE_ROUNDS_PARAM_R_224_256	5
#define FUGUE_ROUNDS_PARAM_R_384		6
#define FUGUE_ROUNDS_PARAM_R_512		8
#define FUGUE_ROUNDS_PARAM_T			13

typedef enum   { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;

private:
//MULTIPLE PARAMS USED IN FUGUE.C
int fugueNumRoundsParam1;
int fugueNumRoundsParam2;
int fugueNumRoundsParam3;
int fugueNumRoundsParam4;
fugueHashState fugueState;

public:
Fugue(const int numRounds);
int Init (int hashbitlen);
int Update (const BitSequence *data, DataLength databitlen);
int Final (BitSequence *hashval);
int Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif
