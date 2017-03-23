#ifndef WAMM_SHA3_H
#define WAMM_SHA3_H

#include "../../sha3_interface.h"
#include "WaMMConstants.h"

class WaMM : public sha3_interface {

private:
int wammNumRounds;
WaMMhashState wammState;

public:
WaMM(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif