#ifndef CHI_SHA3_H
#define CHI_SHA3_H

#include "../../sha3_interface.h"
extern "C" {
#include "chi.h"
}

class Chi : public sha3_interface {

private:
int chiNumRounds256;
int chiNumRounds512;
chiHashState chiState;
/*
 * External Interface
 */
public:
Chi(const int numRounds);
int Init(int); 
int Update(const BitSequence*, DataLength); 
int Final(BitSequence*); 
int Hash(int, const BitSequence*, DataLength, BitSequence*);

private:
void hash(int final);
INLINE int inc_total_len();

};

#endif