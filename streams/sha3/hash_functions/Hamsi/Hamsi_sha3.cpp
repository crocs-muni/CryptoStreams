#include <string.h>
#include <stdlib.h>
extern "C" {
#include "hamsi.h"
#include "hamsi-tables.h"
}
#include "Hamsi_sha3.h"

Hamsi::Hamsi(const int numRounds) {
	if (numRounds == -1) {
		hamsiNumRounds256 = HAMSI_ROUNDS_256;
		hamsiNumRounds384 = HAMSI_ROUNDS_384;
		hamsiPFNumRounds256 = HAMSI_PF_ROUNDS_256;
		hamsiPFNumRounds384 = HAMSI_PF_ROUNDS_384;
	} else {
		hamsiNumRounds256 = numRounds;
		hamsiNumRounds384 = numRounds;
		hamsiPFNumRounds256 = numRounds;
		hamsiPFNumRounds384 = numRounds;
	}
}

int Hamsi::Init(int hashbitlen) {
    int size=(hashbitlen<=256)?1:2;
    hamsiState.leftbits    = 0;
    hamsiState.counter     = 0;
    hamsiState.hashbitlen  = hashbitlen;
	if (hashbitlen <= 256) {
		hamsiState.ROUNDS  = hamsiNumRounds256;
	} else {
		hamsiState.ROUNDS  = hamsiNumRounds384;
	}
	if (hashbitlen <= 256) {
		hamsiState.PFROUNDS = hamsiPFNumRounds256;
	} else {
		hamsiState.PFROUNDS = hamsiPFNumRounds384;
	}
    hamsiState.cvsize      = 256*size;

    if (hashbitlen==224)      { memcpy(hamsiState.state,hamsi_iv224,32); }
    else if (hashbitlen==256) { memcpy(hamsiState.state,hamsi_iv256,32); }
    else if (hashbitlen==384) { memcpy(hamsiState.state,hamsi_iv384,64); }
    else if (hashbitlen==512) { memcpy(hamsiState.state,hamsi_iv512,64); }
    else return BAD_HASHBITLEN;

    memset(hamsiState.leftdata,'\0',8);

    return SUCCESS;
}

int Hamsi::Update(const BitSequence *data, DataLength databitlen) {
    HashReturn ret=SUCCESS;
    int bits2hash=databitlen;
    int i;
    const BitSequence* block=data;
    const int s_blocksize=(hamsiState.cvsize/8);
    // Do first imcomplete block
    if (hamsiState.leftbits) {
        if ((hamsiState.leftbits+bits2hash) < s_blocksize) {
            // just copy, update leftbits
            for (i=0; i<((bits2hash+7)/8); ++i) hamsiState.leftdata[(hamsiState.leftbits/8)+i]=block[i];
            if (bits2hash%8) hamsiState.leftdata[(hamsiState.leftbits+bits2hash)/8] &= (0xff << (8-(bits2hash%8)));
            hamsiState.leftbits += bits2hash;
        } else {
            // copy and do first block from leftdata
            block += (s_blocksize - hamsiState.leftbits)/8;
            bits2hash -= (s_blocksize - hamsiState.leftbits);
            if (hamsiState.cvsize==256) {
                hamsi_hash256(hamsiState.ROUNDS,hamsiState.state,hamsiState.leftdata,0);
            } else if (hamsiState.cvsize==512) {
                hamsi_hash512(hamsiState.ROUNDS,hamsiState.state,hamsiState.leftdata,0);
            }
            hamsiState.leftbits=0;
        }
    }

    // do all complete blocks
    for (; bits2hash >= s_blocksize; bits2hash-=s_blocksize) {
        if (hamsiState.cvsize==256) {
            hamsi_hash256(hamsiState.ROUNDS,hamsiState.state,block,0);
        } else if (hamsiState.cvsize==512) {
            hamsi_hash512(hamsiState.ROUNDS,hamsiState.state,block,0);
        }
        block += (s_blocksize/8);
        ++(hamsiState.counter);
    }

    // copy leftover data, update leftbits
    hamsiState.leftbits = bits2hash;
    for (i=0; i<((bits2hash+7)/8); ++i) hamsiState.leftdata[i]=block[i];

    return ret;
}

inline void HAMSI_HV(BitSequence *h,int i,unsigned int w) {
    h[4*(i)]=((w)>>24)&0xff;
    h[4*(i)+1]=((w)>>16)&0xff;
  
    h[4*(i)+2]=((w)>>8)&0xff;
    h[4*(i)+3]=w&0xff;
}

int Hamsi::Final(BitSequence *hashval) {
  const int block_bytes=(hamsiState.cvsize/8)/8;
  int i;
  unsigned long long int length = ((unsigned long long int)(hamsiState.cvsize)*hamsiState.counter)+hamsiState.leftbits;
  BitSequence lenbytes[8];

  HashReturn ret=SUCCESS;

  // Padding
  hamsiState.leftdata[hamsiState.leftbits/8] &= (0xff<<(8-(hamsiState.leftbits%8)));
  hamsiState.leftdata[hamsiState.leftbits/8] |= (1<<(7-(hamsiState.leftbits%8)));
  for (i=(hamsiState.leftbits/8)+1; i<block_bytes; ++i) hamsiState.leftdata[i]=0;

  // position data length in lenbytes, BIG endian
  for (i=0; i<8; ++i) lenbytes[i] = (length>>(8*(7-i)))&0xff;

  // Processing padding and length
  if (hamsiState.cvsize==256) {
      hamsi_hash256(hamsiState.ROUNDS,hamsiState.state,hamsiState.leftdata,0);
      hamsi_hash256(hamsiState.ROUNDS,hamsiState.state,lenbytes,0);
      hamsi_hash256(hamsiState.PFROUNDS,hamsiState.state,lenbytes+4,1);
  } else if (hamsiState.cvsize==512) {
      hamsi_hash512(hamsiState.ROUNDS,hamsiState.state,hamsiState.leftdata,0);
      hamsi_hash512(hamsiState.PFROUNDS,hamsiState.state,lenbytes,1);
  }

  // Truncation
  if (hamsiState.hashbitlen==384) {
      int T_384[12] = { 0, 1, 3, 4, 5, 6, 8, 9, 10, 12, 13, 15 };
      for (i=0; i<12; ++i) HAMSI_HV(hashval,i,hamsiState.state[T_384[i]]);
          // hashval[i] = hamsiState.state[T_384[i]];
  } else
      //memcpy(hashval,hamsiState.state,hamsiState.hashbitlen/8);
      for (i=0; i<(hamsiState.hashbitlen/32); ++i)
          HAMSI_HV(hashval,i,hamsiState.state[i]);
          // hashval[i] = hamsiState.state[i];

  return ret;
}

int Hamsi::Hash(int hashbitlen, const BitSequence* data, DataLength databitlen, BitSequence *hashval) {
    //static hashState MyState;
    //HashReturn ret;
	int ret;
	ret = Hamsi::Init(hashbitlen);
    if (ret) return ret;
    ret = Hamsi::Update(data,databitlen);
    if (ret) return ret;
    else return Hamsi::Final(hashval);
}