#ifndef SIMD_SHA3_H
#define SIMD_SHA3_H

#include "../../sha3_interface.h"
extern "C" {
#include "tables.h"
}

class Simd : public sha3_interface {

#define SIMD_DEFAULT_ROUNDS	4

private:
int simdNumRounds;
simdHashState simdState;

/* 
 * NIST API
 */
public:
Simd(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, SimdDataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, SimdDataLength databitlen,
                BitSequence *hashval);

/* 
 * Internal API
 */
private:
void IncreaseCounter(simdHashState *state, SimdDataLength databitlen);
int InitIV(simdHashState *state, int hashbitlen, const unsigned long *IV);

};

#endif