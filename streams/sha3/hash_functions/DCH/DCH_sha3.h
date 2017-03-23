#ifndef DCH_SHA3_H
#define DCH_SHA3_H

#include "../../sha3_interface.h"

class DCH : public sha3_interface {

//Primitive polynomial over GF(2^8) to use
#define DCH_PRIMITIVE 0x11d

//Block length
#define DCH_BLOCK_LENGTH_BYTES 64
#define DCH_PADDED_BLOCK_LENGTH_BITS 512

//Number of rounds
//NASTAVENIE RUND:
#define DCH_NUM_ROUNDS 4

typedef enum {SUCCESS = 0,  FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;

typedef struct {
  int hashbitlen;
  int numUnprocessed;        //Number of unprocessed bits
  BitSequence *unprocessed;  //Unprocessed data storage
  BitSequence *curr;         //Current result so far.
  DataLength datalen;        //Total length processed so far.

  //the following are used to generate the square-free sequence
  BitSequence *p[3];
  BitSequence *top[3];
  BitSequence parity;
  BitSequence small;
  BitSequence count;
  BitSequence move;

} hashState;

private:
int dchNumRounds;
hashState dchState;

public:
DCH(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void setNextSequenceValue(hashState *state);
void setLastSequenceValue(hashState *state);
void hashOneBlock(hashState *state);
void addInto(BitSequence *to, const BitSequence *from);
void doSBox(BitSequence *data);
void doTransform(BitSequence *data);

};

#endif