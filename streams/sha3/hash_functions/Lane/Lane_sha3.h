#ifndef LANE_SHA3_H
#define LANE_SHA3_H

#include <string.h>
#include "../../sha3_interface.h"

class Lane : public sha3_interface {

#define LANE_224_P_DEFAULT	6
#define LANE_224_Q_DEFAULT	3
#define LANE_384_P_DEFAULT	8
#define LANE_384_Q_DEFAULT	4

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2, BAD_DATABITLEN = 3 } HashReturn;

typedef struct {
  int hashbitlen;
  unsigned long long ctr;
  unsigned int h[16];
  unsigned char buffer[128];
} hashState;

private:
int numRoundsLane224P;
int numRoundsLane224Q;
int numRoundsLane384P;
int numRoundsLane384Q;
hashState laneState;

public:
Lane(const int numRounds);
int Init (int hashbitlen);
int Update (const BitSequence *data, DataLength databitlen);
int Final (BitSequence *hashval);
int Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void lane256_compress(const unsigned char m[64], unsigned int h[8], const unsigned int ctrh, const unsigned int ctrl);
void lane512_compress(const unsigned char m[128], unsigned int h[16], const unsigned int ctrh, const unsigned int ctrl);

};

#endif