#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//#include "nist.h"
#include "tables.h"

#define SIMD_PRINT_STEPS 0
#define SIMD_PRINT_SOME 0

char* SIMD_VERSION(void) {
  return "OPTIMIZED, NON-VECTORIZED";
}

int SimdSupportedLength(int hashbitlen) {
  if (hashbitlen <= 0 || hashbitlen > 512)
    return 0;
  else
    return 1;
}

int SimdRequiredAlignment(void) {
#ifdef SIMD_LITTLE_ENDIAN
  return 4;
#else
  return 1;
#endif
}

/************* the round function ****************/

#define  SIMD_IF(x, y, z)  ((((y) ^ (z)) & (x)) ^ (z))

#define SIMD_MAJ(x, y, z)  (((z) & (y)) | (((z) | (y)) & (x)))

#define SIMD_STEP4(state, w, i, r, s, A, B, C, D, F)				\
  {                                                                     \
    int j;                                                              \
    unsigned long R[4];                                                           \
    if (SIMD_PRINT_SOME) {                                                   \
      for (j=0; j < 4; j++) {                                           \
        printf ("%08x/%2i/%2i: %08x %08x %08x %08x\n",                  \
                w[j], r, s,                                             \
                state->A[j], state->B[j], state->C[j], state->D[j]);    \
      }                                                                 \
      printf("\n");                                                     \
    }									\
    for(j=0; j<4; j++) {                                                \
      R[j] = SIMD_ROTL32(state->A[j], r);                                    \
    }                                                                   \
    for(j=0; j<4; j++) {                                                \
      state->D[j] = state->D[j] + w[j] +                                \
        F(state->A[j], state->B[j], state->C[j]);                       \
      state->D[j] = SIMD_T32(SIMD_ROTL32(SIMD_T32(state->D[j]), s) + R[(j+(i&1)+1)&3]); \
      state->A[j] = R[j];                                               \
    }                                                                   \
  }
  
const int simd_p8[4][8] = {
  {1,0, 3,2, 5,4, 7,6},
  {2,3, 0,1, 6,7, 4,5},
  {7,6, 5,4, 3,2, 1,0},
  {4,5, 6,7, 0,1, 2,3}};

#define SIMD_STEP8(state, w, i, r, s, A, B, C, D, F)				\
  {                                                                     \
    int j;                                                              \
    unsigned long R[8];                                                           \
    if (SIMD_PRINT_SOME) {                                                   \
      for (j=0; j < 8; j++) {                                           \
        printf ("%08x/%2i/%2i: %08x %08x %08x %08x\n",                  \
                w[j], r, s,                                             \
                state->A[j], state->B[j], state->C[j], state->D[j]);    \
      }                                                                 \
      printf("\n");                                                     \
    }									\
    for(j=0; j<8; j++) {                                                \
      R[j] = SIMD_ROTL32(state->A[j], r);                                    \
    }                                                                   \
    for(j=0; j<8; j++) {                                                \
      state->D[j] = state->D[j] + w[j] +                                \
        F(state->A[j], state->B[j], state->C[j]);                       \
      state->D[j] = SIMD_T32(SIMD_ROTL32(SIMD_T32(state->D[j]), s) + R[simd_p8[i & 3][j]]);	\
      state->A[j] = R[j];                                               \
    }                                                                   \
  }

void SimdRound4(simdHashState * state, int y[128], int i,
           int r, int s, int t, int u) {
  int code = i<2? 185: 233;
  unsigned long w[8][4];
  int a, b;

  /*
   * The FFT output y is in Simdrevbin permuted order,
   * but this is included in the tables P and Q
   */

  for(a=0; a<8; a++)
    for(b=0; b<4; b++)
      w[a][b] = (((unsigned long) (y[SIMD_Q4[8*i+a][b]] * code)) << 16) |
                (((unsigned long) (y[SIMD_P4[8*i+a][b]] * code)) & 0xffff);

  SIMD_STEP4(state, w[0], 0, r, s, A, B, C, D, SIMD_IF);
  SIMD_STEP4(state, w[1], 1, s, t, D, A, B, C, SIMD_IF);
  SIMD_STEP4(state, w[2], 2, t, u, C, D, A, B, SIMD_IF);
  SIMD_STEP4(state, w[3], 3, u, r, B, C, D, A, SIMD_IF);

  SIMD_STEP4(state, w[4], 4, r, s, A, B, C, D, SIMD_MAJ);
  SIMD_STEP4(state, w[5], 5, s, t, D, A, B, C, SIMD_MAJ);
  SIMD_STEP4(state, w[6], 6, t, u, C, D, A, B, SIMD_MAJ);
  SIMD_STEP4(state, w[7], 7, u, r, B, C, D, A, SIMD_MAJ);
}

void SimdRound8(simdHashState * state, int y[128], int i,
           int r, int s, int t, int u) {
  int code = i<2? 185: 233;
  unsigned long w[8][8];
  int a, b;

  /*
   * The FFT output y is in Simdrevbin permuted order,
   * but this is included in the tables P and Q
   */

  for(a=0; a<8; a++)
    for(b=0; b<8; b++)
      w[a][b] = (((unsigned long) (y[SIMD_Q8[8*i+a][b]] * code)) << 16) |
                (((unsigned long) (y[SIMD_P8[8*i+a][b]] * code)) & 0xffff);

  SIMD_STEP8(state, w[0], 0, r, s, A, B, C, D, SIMD_IF);
  SIMD_STEP8(state, w[1], 1, s, t, D, A, B, C, SIMD_IF);
  SIMD_STEP8(state, w[2], 2, t, u, C, D, A, B, SIMD_IF);
  SIMD_STEP8(state, w[3], 3, u, r, B, C, D, A, SIMD_IF);

  SIMD_STEP8(state, w[4], 4, r, s, A, B, C, D, SIMD_MAJ);
  SIMD_STEP8(state, w[5], 5, s, t, D, A, B, C, SIMD_MAJ);
  SIMD_STEP8(state, w[6], 6, t, u, C, D, A, B, SIMD_MAJ);
  SIMD_STEP8(state, w[7], 7, u, r, B, C, D, A, SIMD_MAJ);
}


/********************* Message expansion ************************/

/*
 * Reduce modulo 257; result is in [-127; 383]
 * SIMD_REDUCE(x) := (x&255) - (x>>8)
 */
#define SIMD_REDUCE(x) (((x)&255) - ((x)>>8))

/*
 * Reduce from [-127; 383] to [-128; 128]
 * SIMD_EXTRA_REDUCE_S(x) := x<=128 ? x : x-257
 */
#define SIMD_EXTRA_REDUCE_S(x)                       \
  ((x)<=128 ? (x) : (x)-257)
 
/*
 * Reduce modulo 257; result is in [-128; 128]
 */
#define SIMD_REDUCE_FULL_S(x)                        \
  SIMD_EXTRA_REDUCE_S(SIMD_REDUCE(x))

MAYBE_INLINE void SimdFFT_8(int *y, int stripe) {

  /*
   * SimdFFT_8 using w=4 as 8th root of unity
   *  Unrolled decimation in frequency (DIF) radix-2 NTT.
   *  Output data is in revbin_permuted order.
   */



#define SIMD_X(i) y[stripe*i]

#define SIMD_DO_REDUCE(i)                            \
  SIMD_X(i) = SIMD_REDUCE(SIMD_X(i))

#define SIMD_DO_REDUCE_FULL_S(i)                     \
  do {                                          \
    SIMD_X(i) = SIMD_REDUCE(SIMD_X(i));                        \
    SIMD_X(i) = SIMD_EXTRA_REDUCE_S(SIMD_X(i));                \
  } while(0)

#define SIMD_BUTTERFLY(i,j,n)                        \
  do {                                          \
    int u= SIMD_X(i);                              \
    int v= SIMD_X(j);                              \
    SIMD_X(i) =  u+v;				\
    SIMD_X(j) = (u-v) << (2*n);			\
  } while(0)

    SIMD_BUTTERFLY(0, 4, 0);
    SIMD_BUTTERFLY(1, 5, 1);
    SIMD_BUTTERFLY(2, 6, 2);
    SIMD_BUTTERFLY(3, 7, 3);

    SIMD_DO_REDUCE(6);
    SIMD_DO_REDUCE(7);

    SIMD_BUTTERFLY(0, 2, 0);
    SIMD_BUTTERFLY(4, 6, 0);
    SIMD_BUTTERFLY(1, 3, 2);
    SIMD_BUTTERFLY(5, 7, 2);

    SIMD_DO_REDUCE(7);
    
    SIMD_BUTTERFLY(0, 1, 0);
    SIMD_BUTTERFLY(2, 3, 0);
    SIMD_BUTTERFLY(4, 5, 0);
    SIMD_BUTTERFLY(6, 7, 0);

    SIMD_DO_REDUCE_FULL_S(0);
    SIMD_DO_REDUCE_FULL_S(1);
    SIMD_DO_REDUCE_FULL_S(2);
    SIMD_DO_REDUCE_FULL_S(3);
    SIMD_DO_REDUCE_FULL_S(4);
    SIMD_DO_REDUCE_FULL_S(5);
    SIMD_DO_REDUCE_FULL_S(6);
    SIMD_DO_REDUCE_FULL_S(7);

#undef SIMD_X
#undef SIMD_DO_REDUCE
#undef SIMD_DO_REDUCE_FULL_S
#undef SIMD_BUTTERFLY
}

MAYBE_INLINE void SimdFFT_16(int *y, int stripe) {

  /*
   * SimdFFT_16 using w=2 as 16th root of unity
   *  Unrolled decimation in frequency (DIF) radix-2 NTT.
   *  Output data is in revbin_permuted order.
   */

#define SIMD_X(i) y[stripe*i]

#define SIMD_DO_REDUCE(i)                            \
  SIMD_X(i) = SIMD_REDUCE(SIMD_X(i))

#define SIMD_DO_REDUCE_FULL_S(i)                     \
  do {                                          \
    SIMD_X(i) = SIMD_REDUCE(SIMD_X(i));                        \
    SIMD_X(i) = SIMD_EXTRA_REDUCE_S(SIMD_X(i));                \
  } while(0)

#define SIMD_BUTTERFLY(i,j,n)                        \
  do {                                          \
    int u= SIMD_X(i);                              \
    int v= SIMD_X(j);                              \
    SIMD_X(i) =  u+v;				\
    SIMD_X(j) = (u-v) << n;                          \
  } while(0)

    SIMD_BUTTERFLY(0,  8, 0);
    SIMD_BUTTERFLY(1,  9, 1);
    SIMD_BUTTERFLY(2, 10, 2);
    SIMD_BUTTERFLY(3, 11, 3);
    SIMD_BUTTERFLY(4, 12, 4);
    SIMD_BUTTERFLY(5, 13, 5);
    SIMD_BUTTERFLY(6, 14, 6);
    SIMD_BUTTERFLY(7, 15, 7);

    SIMD_DO_REDUCE(11);
    SIMD_DO_REDUCE(12);
    SIMD_DO_REDUCE(13);
    SIMD_DO_REDUCE(14);
    SIMD_DO_REDUCE(15);

    SIMD_BUTTERFLY( 0,  4, 0);
    SIMD_BUTTERFLY( 8, 12, 0);
    SIMD_BUTTERFLY( 1,  5, 2);
    SIMD_BUTTERFLY( 9, 13, 2);
    SIMD_BUTTERFLY( 2,  6, 4);
    SIMD_BUTTERFLY(10, 14, 4);
    SIMD_BUTTERFLY( 3,  7, 6);
    SIMD_BUTTERFLY(11, 15, 6);

    SIMD_DO_REDUCE(5);
    SIMD_DO_REDUCE(7);
    SIMD_DO_REDUCE(13);
    SIMD_DO_REDUCE(15);
    
    SIMD_BUTTERFLY( 0,  2, 0);
    SIMD_BUTTERFLY( 4,  6, 0);
    SIMD_BUTTERFLY( 8, 10, 0);
    SIMD_BUTTERFLY(12, 14, 0);
    SIMD_BUTTERFLY( 1,  3, 4);
    SIMD_BUTTERFLY( 5,  7, 4);
    SIMD_BUTTERFLY( 9, 11, 4);
    SIMD_BUTTERFLY(13, 15, 4);

    SIMD_BUTTERFLY( 0,  1, 0);
    SIMD_BUTTERFLY( 2,  3, 0);
    SIMD_BUTTERFLY( 4,  5, 0);
    SIMD_BUTTERFLY( 6,  7, 0);
    SIMD_BUTTERFLY( 8,  9, 0);
    SIMD_BUTTERFLY(10, 11, 0);
    SIMD_BUTTERFLY(12, 13, 0);
    SIMD_BUTTERFLY(14, 15, 0);

    SIMD_DO_REDUCE_FULL_S( 0);
    SIMD_DO_REDUCE_FULL_S( 1);
    SIMD_DO_REDUCE_FULL_S( 2);
    SIMD_DO_REDUCE_FULL_S( 3);
    SIMD_DO_REDUCE_FULL_S( 4);
    SIMD_DO_REDUCE_FULL_S( 5);
    SIMD_DO_REDUCE_FULL_S( 6);
    SIMD_DO_REDUCE_FULL_S( 7);
    SIMD_DO_REDUCE_FULL_S( 8);
    SIMD_DO_REDUCE_FULL_S( 9);
    SIMD_DO_REDUCE_FULL_S(10);
    SIMD_DO_REDUCE_FULL_S(11);
    SIMD_DO_REDUCE_FULL_S(12);
    SIMD_DO_REDUCE_FULL_S(13);
    SIMD_DO_REDUCE_FULL_S(14);
    SIMD_DO_REDUCE_FULL_S(15);

#undef SIMD_X
#undef SIMD_DO_REDUCE
#undef SIMD_DO_REDUCE_FULL_S
#undef SIMD_BUTTERFLY
}

void SimdFFT_64(int *y) {
  
  /*
   * SimdFFT_64 using w=46 as 64th root of unity
   *  decimation in frequency (DIF) radix-8 NTT.
   *  Output data is in revbin_permuted order.
   */

  int i;
 
  /*
   * Begin with 8 parallels DIF SimdFFT_8.
   */
  for (i=0; i<8; i++) {
    SimdFFT_8(y+i,8);
  }

  /*
   * Multiply by twiddle factors
   */
  for (i=8; i<64; i++)
    if (i & 7) y[i] = SIMD_REDUCE(y[i]*SIMD_FFT64_8_8_Twiddle[i]);

  /*
   * Finish with 8 paralles DIF SimdFFT_8.
   */
  for (i=0; i<8; i++) {
    SimdFFT_8(y+8*i,1);
  }

}

void SimdFFT_128_halfzero(int *y, int final) {

  /*
   * FFT_128 using w=139 as 128th root of unity.
   *  Decimation in frequency (DIF) NTT.
   *  Output data is in revbin_permuted order.
   *  In place.
   */

  const int tmp = y[63];
  int i;
 
  for (i=0; i<63; i++)
    y[64+i] = SIMD_REDUCE(y[i] * SIMD_FFT128_2_64_Twiddle[i]);

  if (final) {
  /*   handle SIMD_X^125 with an additionnal butterfly */
    const int tmp2 = y[61];
    y[61]  = SIMD_REDUCE(tmp2 + 1);
    y[125] = SIMD_REDUCE((tmp2 - 1) * SIMD_FFT128_2_64_Twiddle[61]);
  }

  /* handle SIMD_X^127 */
  y[63] = SIMD_REDUCE(tmp + 1);
  y[127] = SIMD_REDUCE((tmp - 1) * SIMD_FFT128_2_64_Twiddle[63]);

  SimdFFT_64(y);
  SimdFFT_64(y+64);
}

void SimdFFT_128_full(int *y) {
  int i;
 
  for (i=0; i<16; i++) {
    SimdFFT_8(y+i,16);
  }

  for (i=0; i<128; i++)
    /*if (i & 7)*/  y[i] = SIMD_REDUCE(y[i]*SIMD_FFT128_8_16_Twiddle[i]);

  for (i=0; i<8; i++) {
    SimdFFT_16(y+16*i,1);
  }
}


void SimdFFT_256_halfzero(int *y, int final) {

  /*
   * FFT_256 using w=41 as 256th root of unity.
   *  Decimation in frequency (DIF) NTT.
   *  Output data is in revbin_permuted order.
   *  In place.
   */

  const int tmp = y[127];
  int i;

  for (i=0; i<127; i++)
    y[128+i] = SIMD_REDUCE(y[i] * SIMD_FFT256_2_128_Twiddle[i]);

  if (final) {
  /*   handle SIMD_X^253 with an additionnal butterfly */
    const int tmp2 = y[125];
    y[125] = SIMD_REDUCE(tmp2 + 1);
    y[253] = SIMD_REDUCE((tmp2 - 1) * SIMD_FFT256_2_128_Twiddle[125]);
  }

  /*   handle SIMD_X^255 with an additionnal butterfly */
  y[127] = SIMD_REDUCE(tmp + 1);
  y[255] = SIMD_REDUCE((tmp - 1) * SIMD_FFT256_2_128_Twiddle[127]);

  SimdFFT_128_full(y);
  SimdFFT_128_full(y+128);
}

void SIMD_Compress(simdHashState * state, const unsigned char *M, int final, const int rounds) {
  
  int y[256];
  unsigned long IV[4][8];
  int i,j;
  int n = state->n_feistels;
 
  if (n == 4) {
    for(i=0; i<64; i++)
      y[i] = M[i];

    SimdFFT_128_halfzero(y, final);
  } else {
    for(i=0; i<128; i++)
      y[i] = M[i];

    SimdFFT_256_halfzero(y, final);
  }  

  /* Save the chaining value for the feed-forward */
  
  for(i=0; i<n; i++) {
    IV[0][i] = state->A[i];
    IV[1][i] = state->B[i];
    IV[2][i] = state->C[i];
    IV[3][i] = state->D[i];
  }

#if SIMD_PRINT_STEPS
 printf("IV :\n");
  for(i=0; i<n; i++)
    printf("A[%d]=%08x  B[%d]=%08x  C[%d]=%08x  D[%d]=%08x\n", 
           i,state->A[i], i,state->B[i], i,state->C[i], i,state->D[i]);
  printf("\n");
#endif

  /* XOR the message to the chaining value */
#ifdef SIMD_LITTLE_ENDIAN
  /* we can XOR word-by-word */
  
  {
    unsigned long * message = (unsigned long *) M;
    
    for(i=0; i<n; i++) {
      state->A[i] ^= message[i];
      state->B[i] ^= message[n+i];
      state->C[i] ^= message[2*n+i];
      state->D[i] ^= message[3*n+i];
    }
  }
#else
  /* we have to be extra-careful */
#define SIMD_PACK(i)  (((unsigned long) M[i]) ^ (((unsigned long) M[i+1]) << 8) ^ (((unsigned long) M[i+2]) << 16) ^ (((unsigned long) M[i+3]) << 24))
  for(j=0; j<n; j++) {
    state->A[j] ^= SIMD_PACK(4*j);
    state->B[j] ^= SIMD_PACK(4*j+4*n);
    state->C[j] ^= SIMD_PACK(4*j+8*n);
    state->D[j] ^= SIMD_PACK(4*j+12*n);
  }
#undef SIMD_PACK  
#endif

#if SIMD_PRINT_STEPS
  printf("IV XOR M :\n");
  for(i=0; i<n; i++)
    printf("A[%d]=%08x  B[%d]=%08x  C[%d]=%08x  D[%d]=%08x\n", 
           i,state->A[i], i,state->B[i], i,state->C[i], i,state->D[i]);
  printf("\n");
#endif

  /* Run the feistel ladders */
  if (n == 4) {
    if (rounds >= 1) SimdRound4(state, y, 0, 3,  20, 14, 27);
    if (rounds >= 2) SimdRound4(state, y, 1, 26,  4, 23, 11);
    if (rounds >= 3) SimdRound4(state, y, 2, 19, 28,  7, 22);
    if (rounds >= 4) SimdRound4(state, y, 3, 15,  5, 29, 9);

    if (rounds >= 1) SIMD_STEP4(state, IV[0], 0, 15, 5, A, B, C, D, SIMD_IF);
    if (rounds >= 2) SIMD_STEP4(state, IV[1], 1, 5, 29, D, A, B, C, SIMD_IF);
    if (rounds >= 3) SIMD_STEP4(state, IV[2], 2, 29, 9, C, D, A, B, SIMD_IF);
    if (rounds >= 4) SIMD_STEP4(state, IV[3], 3, 9, 15, B, C, D, A, SIMD_IF);
  }
  else
 {
    if (rounds >= 1) SimdRound8(state, y, 0, 3,  20, 14, 27);
    if (rounds >= 2) SimdRound8(state, y, 1, 26,  4, 23, 11);
    if (rounds >= 3) SimdRound8(state, y, 2, 19, 28,  7, 22);
    if (rounds >= 4) SimdRound8(state, y, 3, 15,  5, 29, 9);

    if (rounds >= 1) SIMD_STEP8(state, IV[0], 0, 15, 5, A, B, C, D, SIMD_IF);
    if (rounds >= 2) SIMD_STEP8(state, IV[1], 1, 5, 29, D, A, B, C, SIMD_IF);
    if (rounds >= 3) SIMD_STEP8(state, IV[2], 2, 29, 9, C, D, A, B, SIMD_IF);
    if (rounds >= 4) SIMD_STEP8(state, IV[3], 3, 9, 15, B, C, D, A, SIMD_IF);
  }
}


/* 
 * Gives the FFT output in the regular order for consistency checks
 */
int Simdrevbin(int k, int i) {
  int acc = 0;
  int j;
  for(j=0; j<k; j++) {
    acc = acc << 1;
    acc ^= (i & 1);
    i = i >> 1;
  }
  return acc;
}


void Simdfft128_natural(int *a, unsigned char *x) {
  int A[128];
  int i;

  for (i=0; i<64; i++)
    A[i] = x[i];

  SimdFFT_128_halfzero(A,0);
  
  for (i=0; i<128; i++)
  a[i] = A[Simdrevbin(7,i)];
  
}

void Simdfft256_natural(int *a, unsigned char *x) {
  int A[256];
  int i;

  for (i=0; i<128; i++)
    A[i] = x[i];

  SimdFFT_256_halfzero(A,0);
  
  for (i=0; i<256; i++)
    a[i] = A[Simdrevbin(8,i)];
}
