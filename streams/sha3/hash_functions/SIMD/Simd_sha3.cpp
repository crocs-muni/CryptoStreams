#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Simd_sha3.h"
extern "C" {
#include "tables.h"
}

// #define NO_PRECOMPUTED_IV


/* 
 * Increase the counter.
 */
void Simd::IncreaseCounter(simdHashState *state, SimdDataLength databitlen) {
#ifdef SIMD_HAS_64
      state->count += databitlen;
#else
      unsigned long old_count = state->count_low;
      state->count_low += databitlen;
      if (state->count_low < old_count)
        state->count_high++;
#endif
}


/* 
 * Initialize the hashState with a given IV.
 * If the IV is NULL, initialize with zeros.
 */
int Simd::InitIV(simdHashState *state, int hashbitlen, const unsigned long *IV) {

  int n;

  if (!SimdSupportedLength(hashbitlen))
    return SIMD_BAD_HASHBITLEN;

  n = (hashbitlen <= 256) ? 4 : 8;

  state->hashbitlen = hashbitlen;
  state->n_feistels = n;
  state->blocksize = 128*n;
  
#ifdef SIMD_HAS_64
  state->count = 0;
#else
  state->count_low  = 0;
  state->count_high = 0;
#endif  

  state->buffer = (unsigned char*) malloc(16*n + 16);
  /*
   * Align the buffer to a 128 bit boundary.
   */
  state->buffer += ((unsigned char*)NULL - state->buffer)&15;

  state->A = (unsigned long*) malloc((4*n+4)*sizeof(unsigned long));
  /*
   * Align the buffer to a 128 bit boundary.
   */
  state->A += ((unsigned long*)NULL - state->A)&3;

  state->B = state->A+n;
  state->C = state->B+n;
  state->D = state->C+n;

  if (IV)
    memcpy(state->A, IV, 4*n*sizeof(unsigned long));
  else
    memset(state->A, 0, 4*n*sizeof(unsigned long));

  return SIMD_SUCCESS;
}

/* 
 * Initialize the hashState.
 */
int Simd::Init(int hashbitlen) {
  int r;
  char *init;

#ifndef NO_PRECOMPUTED_IV
  if (hashbitlen == 224)
    r=InitIV(&simdState, hashbitlen, SIMD_IV_224);
  else if (hashbitlen == 256)
    r=InitIV(&simdState, hashbitlen, SIMD_IV_256);
  else if (hashbitlen == 384)
    r=InitIV(&simdState, hashbitlen, SIMD_IV_384);
  else if (hashbitlen == 512)
    r=InitIV(&simdState, hashbitlen, SIMD_IV_512);
  else
#endif
    {
      /* 
       * Nonstandart length: IV is not precomputed.
       */
      r=InitIV(&simdState, hashbitlen, NULL);
      if (r != SIMD_SUCCESS)
        return r;
      
      init = (char*) malloc(simdState.blocksize);
      memset(init, 0, simdState.blocksize);
#if defined __STDC__ && __STDC_VERSION__ >= 199901L
      snprintf(init, simdState.blocksize, "SIMD-%i v1.0", hashbitlen);
#else
      sprintf(init, "SIMD-%i v1.0", hashbitlen);
#endif
      SIMD_Compress(&simdState, (unsigned char*) init, 0, simdNumRounds);
      free(init);
    }
  return r;
}



int Simd::Update(const BitSequence *data, SimdDataLength databitlen) {
  unsigned current;
  unsigned int bs = simdState.blocksize;
  static int align = -1;

  if (align == -1)
    align = SimdRequiredAlignment();

#ifdef SIMD_HAS_64
  current = simdState.count & (bs - 1);
#else
  current = simdState.count_low & (bs - 1);
#endif
  
  if (current & 7) {
    /*
     * The number of hashed bits is not a multiple of 8.
     * Very painfull to implement and not required by the NIST API.
     */
    return SIMD_FAIL;
  }

  while (databitlen > 0) {
    if (SIMD_IS_ALIGNED(data,align) && current == 0 && databitlen >= bs) {
      /* 
       * We can hash the data directly from the input buffer.
       */
      SIMD_Compress(&simdState, data, 0, simdNumRounds);
      databitlen -= bs;
      data += bs/8;
      IncreaseCounter(&simdState, bs);
    } else {
      /* 
       * Copy a chunk of data to the buffer
       */
      unsigned int len = bs - current;
      if (databitlen < len) {
        memcpy(simdState.buffer+current/8, data, (databitlen+7)/8);
        IncreaseCounter(&simdState, databitlen);        
        return SIMD_SUCCESS;
      } else {
        memcpy(simdState.buffer+current/8, data, len/8);
        IncreaseCounter(&simdState,len);
        databitlen -= len;
        data += len/8;
        current = 0;
        SIMD_Compress(&simdState, simdState.buffer, 0, simdNumRounds);
      }
    }
  }
  return SIMD_SUCCESS;
}

int Simd::Final(BitSequence *hashval) {
#ifdef SIMD_HAS_64
  unsigned long long l;
#else
  unsigned long l;
#endif
  unsigned int i;
  BitSequence bs[64];

  /* 
   * If there is still some data in the buffer, hash it
   */
  if (simdState.count & (simdState.blocksize - 1)) {
    /* 
     * We first need to zero out the end of the buffer.
     */
    int current = simdState.count & (simdState.blocksize - 1);
    if (current & 7) {
      BitSequence mask = 0xff >> (current&7);
      simdState.buffer[current/8] &= ~mask;
    }
    current = (current+7)/8;
    memset(simdState.buffer+current, 0, simdState.blocksize/8 - current);
    SIMD_Compress(&simdState, simdState.buffer, 0, simdNumRounds);
  }

  /* 
   * Input the message length as the last block
   */
  memset(simdState.buffer, 0, simdState.blocksize/8);
#ifdef SIMD_HAS_64
  l = simdState.count;
  for (i=0; i<8; i++) {
    simdState.buffer[i] = l & 0xff;
    l >>= 8;
  }
#else
  l = simdState.count_low;
  for (i=0; i<4; i++) {
    simdState.buffer[i] = l & 0xff;
    l >>= 8;
  }
  l = simdState.count_high;
  for (i=0; i<4; i++) {
    simdState.buffer[4+i] = l & 0xff;
    l >>= 8;
  }
#endif

  SIMD_Compress(&simdState, simdState.buffer, 1, simdNumRounds);
    

  /*
   * Decode the 32-bit words into a BitSequence
   */
  for (i=0; i<2*simdState.n_feistels; i++) {
    unsigned long x = simdState.A[i];
    bs[4*i  ] = x&0xff;
    x >>= 8;
    bs[4*i+1] = x&0xff;
    x >>= 8;
    bs[4*i+2] = x&0xff;
    x >>= 8;
    bs[4*i+3] = x&0xff;
  }

  memcpy(hashval, bs, simdState.hashbitlen/8);
  if (simdState.hashbitlen%8) {
    BitSequence mask = 0xff << (8 - (simdState.hashbitlen%8));
    hashval[simdState.hashbitlen/8 + 1] = bs[simdState.hashbitlen/8 + 1] & mask;
  }

  return SIMD_SUCCESS;
}



int Simd::Hash(int hashbitlen, const BitSequence *data, SimdDataLength databitlen,
                BitSequence *hashval) {
  //hashState s;
  int r;
  r = Simd::Init(hashbitlen);
  if (r != SIMD_SUCCESS)
    return r;
  r = Simd::Update(data, databitlen);
  if (r != SIMD_SUCCESS)
    return r;
  r = Simd::Final(hashval);
  return r;
}

Simd::Simd(const int numRounds) {
	if (numRounds == -1) {
		simdNumRounds = SIMD_DEFAULT_ROUNDS;
	} else {
		simdNumRounds = numRounds;
	}
}