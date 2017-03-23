#ifndef CRUNCH_SHA3_H
#define CRUNCH_SHA3_H

#include "../../sha3_interface.h"
#include "crunch_type.h"

class Crunch : public sha3_interface {

private:
int crunchNumRounds224;
int crunchNumRounds256;
int crunchNumRounds384;
int crunchNumRounds512;
crunchHashState crunchState;

public:
Crunch(const int numRounds);
int Init(int hashbitlen);
int Final(BitSequence *hash);
int Update(const BitSequence *data, DataLength databitlen);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif