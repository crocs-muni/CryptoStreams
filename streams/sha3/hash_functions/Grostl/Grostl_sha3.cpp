#include "Grostl_sha3.h"
#include "tables.h"

/* compute one new state column */
#define GROSTL_COLUMN(x,y,i,c0,c1,c2,c3,c4,c5,c6,c7)				\
  y[i] =								\
    GROSTL_T[0*256+GROSTL_EXT_BYTE(x[c0], 0)]^					\
    GROSTL_T[1*256+GROSTL_EXT_BYTE(x[c1], 1)]^					\
    GROSTL_T[2*256+GROSTL_EXT_BYTE(x[c2], 2)]^					\
    GROSTL_T[3*256+GROSTL_EXT_BYTE(x[c3], 3)]^					\
    GROSTL_T[4*256+GROSTL_EXT_BYTE(x[c4], 0)]^					\
    GROSTL_T[5*256+GROSTL_EXT_BYTE(x[c5], 1)]^					\
    GROSTL_T[6*256+GROSTL_EXT_BYTE(x[c6], 2)]^					\
    GROSTL_T[7*256+GROSTL_EXT_BYTE(x[c7], 3)]

/* compute one round of P (short variants) */
void GROSTL_RND512P(grostl_u32 *x, grostl_u32 *y, grostl_u32 r) {
  x[ 0] ^= GROSTL_U32BIG((grostl_u32)0x00000000u)^r;
  x[ 2] ^= GROSTL_U32BIG((grostl_u32)0x10000000u)^r;
  x[ 4] ^= GROSTL_U32BIG((grostl_u32)0x20000000u)^r;
  x[ 6] ^= GROSTL_U32BIG((grostl_u32)0x30000000u)^r;
  x[ 8] ^= GROSTL_U32BIG((grostl_u32)0x40000000u)^r;
  x[10] ^= GROSTL_U32BIG((grostl_u32)0x50000000u)^r;
  x[12] ^= GROSTL_U32BIG((grostl_u32)0x60000000u)^r;
  x[14] ^= GROSTL_U32BIG((grostl_u32)0x70000000u)^r;
  GROSTL_COLUMN(x,y, 0,  0,  2,  4,  6,  9, 11, 13, 15);
  GROSTL_COLUMN(x,y, 1,  9, 11, 13, 15,  0,  2,  4,  6);
  GROSTL_COLUMN(x,y, 2,  2,  4,  6,  8, 11, 13, 15,  1);
  GROSTL_COLUMN(x,y, 3, 11, 13, 15,  1,  2,  4,  6,  8);
  GROSTL_COLUMN(x,y, 4,  4,  6,  8, 10, 13, 15,  1,  3);
  GROSTL_COLUMN(x,y, 5, 13, 15,  1,  3,  4,  6,  8, 10);
  GROSTL_COLUMN(x,y, 6,  6,  8, 10, 12, 15,  1,  3,  5);
  GROSTL_COLUMN(x,y, 7, 15,  1,  3,  5,  6,  8, 10, 12);
  GROSTL_COLUMN(x,y, 8,  8, 10, 12, 14,  1,  3,  5,  7);
  GROSTL_COLUMN(x,y, 9,  1,  3,  5,  7,  8, 10, 12, 14);
  GROSTL_COLUMN(x,y,10, 10, 12, 14,  0,  3,  5,  7,  9);
  GROSTL_COLUMN(x,y,11,  3,  5,  7,  9, 10, 12, 14,  0);
  GROSTL_COLUMN(x,y,12, 12, 14,  0,  2,  5,  7,  9, 11);
  GROSTL_COLUMN(x,y,13,  5,  7,  9, 11, 12, 14,  0,  2);
  GROSTL_COLUMN(x,y,14, 14,  0,  2,  4,  7,  9, 11, 13);
  GROSTL_COLUMN(x,y,15,  7,  9, 11, 13, 14,  0,  2,  4);
}

/* compute one round of Q (short variants) */
void GROSTL_RND512Q(grostl_u32 *x, grostl_u32 *y, grostl_u32 r) {
  x[ 0] = ~x[ 0];
  x[ 1] ^= GROSTL_U32BIG((grostl_u32)0xffffffffu)^r;
  x[ 2] = ~x[ 2];
  x[ 3] ^= GROSTL_U32BIG((grostl_u32)0xffffffefu)^r;
  x[ 4] = ~x[ 4];
  x[ 5] ^= GROSTL_U32BIG((grostl_u32)0xffffffdfu)^r;
  x[ 6] = ~x[ 6];
  x[ 7] ^= GROSTL_U32BIG((grostl_u32)0xffffffcfu)^r;
  x[ 8] = ~x[ 8];
  x[ 9] ^= GROSTL_U32BIG((grostl_u32)0xffffffbfu)^r;
  x[10] = ~x[10];
  x[11] ^= GROSTL_U32BIG((grostl_u32)0xffffffafu)^r;
  x[12] = ~x[12];
  x[13] ^= GROSTL_U32BIG((grostl_u32)0xffffff9fu)^r;
  x[14] = ~x[14];
  x[15] ^= GROSTL_U32BIG((grostl_u32)0xffffff8fu)^r;
  GROSTL_COLUMN(x,y, 0,  2,  6, 10, 14,  1,  5,  9, 13);
  GROSTL_COLUMN(x,y, 1,  1,  5,  9, 13,  2,  6, 10, 14);
  GROSTL_COLUMN(x,y, 2,  4,  8, 12,  0,  3,  7, 11, 15);
  GROSTL_COLUMN(x,y, 3,  3,  7, 11, 15,  4,  8, 12,  0);
  GROSTL_COLUMN(x,y, 4,  6, 10, 14,  2,  5,  9, 13,  1);
  GROSTL_COLUMN(x,y, 5,  5,  9, 13,  1,  6, 10, 14,  2);
  GROSTL_COLUMN(x,y, 6,  8, 12,  0,  4,  7, 11, 15,  3);
  GROSTL_COLUMN(x,y, 7,  7, 11, 15,  3,  8, 12,  0,  4);
  GROSTL_COLUMN(x,y, 8, 10, 14,  2,  6,  9, 13,  1,  5);
  GROSTL_COLUMN(x,y, 9,  9, 13,  1,  5, 10, 14,  2,  6);
  GROSTL_COLUMN(x,y,10, 12,  0,  4,  8, 11, 15,  3,  7);
  GROSTL_COLUMN(x,y,11, 11, 15,  3,  7, 12,  0,  4,  8);
  GROSTL_COLUMN(x,y,12, 14,  2,  6, 10, 13,  1,  5,  9);
  GROSTL_COLUMN(x,y,13, 13,  1,  5,  9, 14,  2,  6, 10);
  GROSTL_COLUMN(x,y,14,  0,  4,  8, 12, 15,  3,  7, 11);
  GROSTL_COLUMN(x,y,15, 15,  3,  7, 11,  0,  4,  8, 12);
}

/* compute one round of P (short variants) */
void GROSTL_RND1024P(grostl_u32 *x, grostl_u32 *y, grostl_u32 r) {
  x[ 0] ^= GROSTL_U32BIG((grostl_u32)0x00000000u)^r;
  x[ 2] ^= GROSTL_U32BIG((grostl_u32)0x10000000u)^r;
  x[ 4] ^= GROSTL_U32BIG((grostl_u32)0x20000000u)^r;
  x[ 6] ^= GROSTL_U32BIG((grostl_u32)0x30000000u)^r;
  x[ 8] ^= GROSTL_U32BIG((grostl_u32)0x40000000u)^r;
  x[10] ^= GROSTL_U32BIG((grostl_u32)0x50000000u)^r;
  x[12] ^= GROSTL_U32BIG((grostl_u32)0x60000000u)^r;
  x[14] ^= GROSTL_U32BIG((grostl_u32)0x70000000u)^r;
  x[16] ^= GROSTL_U32BIG((grostl_u32)0x80000000u)^r;
  x[18] ^= GROSTL_U32BIG((grostl_u32)0x90000000u)^r;
  x[20] ^= GROSTL_U32BIG((grostl_u32)0xa0000000u)^r;
  x[22] ^= GROSTL_U32BIG((grostl_u32)0xb0000000u)^r;
  x[24] ^= GROSTL_U32BIG((grostl_u32)0xc0000000u)^r;
  x[26] ^= GROSTL_U32BIG((grostl_u32)0xd0000000u)^r;
  x[28] ^= GROSTL_U32BIG((grostl_u32)0xe0000000u)^r;
  x[30] ^= GROSTL_U32BIG((grostl_u32)0xf0000000u)^r;

  GROSTL_COLUMN(x,y, 0, 0, 2, 4, 6, 9,11,13,23);
  GROSTL_COLUMN(x,y, 2, 2, 4, 6, 8,11,13,15,25);
  GROSTL_COLUMN(x,y, 4, 4, 6, 8,10,13,15,17,27);
  GROSTL_COLUMN(x,y, 6, 6, 8,10,12,15,17,19,29);
  GROSTL_COLUMN(x,y, 8, 8,10,12,14,17,19,21,31);
  GROSTL_COLUMN(x,y,10,10,12,14,16,19,21,23, 1);
  GROSTL_COLUMN(x,y,12,12,14,16,18,21,23,25, 3);
  GROSTL_COLUMN(x,y,14,14,16,18,20,23,25,27, 5);
  GROSTL_COLUMN(x,y,16,16,18,20,22,25,27,29, 7);
  GROSTL_COLUMN(x,y,18,18,20,22,24,27,29,31, 9);
  GROSTL_COLUMN(x,y,20,20,22,24,26,29,31, 1,11);
  GROSTL_COLUMN(x,y,22,22,24,26,28,31, 1, 3,13);
  GROSTL_COLUMN(x,y,24,24,26,28,30, 1, 3, 5,15);
  GROSTL_COLUMN(x,y,26,26,28,30, 0, 3, 5, 7,17);
  GROSTL_COLUMN(x,y,28,28,30, 0, 2, 5, 7, 9,19);
  GROSTL_COLUMN(x,y,30,30, 0, 2, 4, 7, 9,11,21);

  GROSTL_COLUMN(x,y, 1, 9,11,13,23, 0, 2, 4, 6);
  GROSTL_COLUMN(x,y, 3,11,13,15,25, 2, 4, 6, 8);
  GROSTL_COLUMN(x,y, 5,13,15,17,27, 4, 6, 8,10);
  GROSTL_COLUMN(x,y, 7,15,17,19,29, 6, 8,10,12);
  GROSTL_COLUMN(x,y, 9,17,19,21,31, 8,10,12,14);
  GROSTL_COLUMN(x,y,11,19,21,23, 1,10,12,14,16);
  GROSTL_COLUMN(x,y,13,21,23,25, 3,12,14,16,18);
  GROSTL_COLUMN(x,y,15,23,25,27, 5,14,16,18,20);
  GROSTL_COLUMN(x,y,17,25,27,29, 7,16,18,20,22);
  GROSTL_COLUMN(x,y,19,27,29,31, 9,18,20,22,24);
  GROSTL_COLUMN(x,y,21,29,31, 1,11,20,22,24,26);
  GROSTL_COLUMN(x,y,23,31, 1, 3,13,22,24,26,28);
  GROSTL_COLUMN(x,y,25, 1, 3, 5,15,24,26,28,30);
  GROSTL_COLUMN(x,y,27, 3, 5, 7,17,26,28,30, 0);
  GROSTL_COLUMN(x,y,29, 5, 7, 9,19,28,30, 0, 2);
  GROSTL_COLUMN(x,y,31, 7, 9,11,21,30, 0, 2, 4);
}

/* compute one round of Q (short variants) */
void GROSTL_RND1024Q(grostl_u32 *x, grostl_u32 *y, grostl_u32 r) {
  x[ 0] = ~x[ 0];
  x[ 1] ^= GROSTL_U32BIG((grostl_u32)0xffffffffu)^r;
  x[ 2] = ~x[ 2];
  x[ 3] ^= GROSTL_U32BIG((grostl_u32)0xffffffefu)^r;
  x[ 4] = ~x[ 4];
  x[ 5] ^= GROSTL_U32BIG((grostl_u32)0xffffffdfu)^r;
  x[ 6] = ~x[ 6];
  x[ 7] ^= GROSTL_U32BIG((grostl_u32)0xffffffcfu)^r;
  x[ 8] = ~x[ 8];
  x[ 9] ^= GROSTL_U32BIG((grostl_u32)0xffffffbfu)^r;
  x[10] = ~x[10];
  x[11] ^= GROSTL_U32BIG((grostl_u32)0xffffffafu)^r;
  x[12] = ~x[12];
  x[13] ^= GROSTL_U32BIG((grostl_u32)0xffffff9fu)^r;
  x[14] = ~x[14];
  x[15] ^= GROSTL_U32BIG((grostl_u32)0xffffff8fu)^r;
  x[16] = ~x[16];
  x[17] ^= GROSTL_U32BIG((grostl_u32)0xffffff7fu)^r;
  x[18] = ~x[18];
  x[19] ^= GROSTL_U32BIG((grostl_u32)0xffffff6fu)^r;
  x[20] = ~x[20];
  x[21] ^= GROSTL_U32BIG((grostl_u32)0xffffff5fu)^r;
  x[22] = ~x[22];
  x[23] ^= GROSTL_U32BIG((grostl_u32)0xffffff4fu)^r;
  x[24] = ~x[24];
  x[25] ^= GROSTL_U32BIG((grostl_u32)0xffffff3fu)^r;
  x[26] = ~x[26];
  x[27] ^= GROSTL_U32BIG((grostl_u32)0xffffff2fu)^r;
  x[28] = ~x[28];
  x[29] ^= GROSTL_U32BIG((grostl_u32)0xffffff1fu)^r;
  x[30] = ~x[30];
  x[31] ^= GROSTL_U32BIG((grostl_u32)0xffffff0fu)^r;

  GROSTL_COLUMN(x,y, 0,  2,  6, 10, 22,  1,  5,  9, 13);
  GROSTL_COLUMN(x,y, 1,  1,  5,  9, 13,  2,  6, 10, 22);
  GROSTL_COLUMN(x,y, 2,  4,  8, 12, 24,  3,  7, 11, 15);
  GROSTL_COLUMN(x,y, 3,  3,  7, 11, 15,  4,  8, 12, 24);
  GROSTL_COLUMN(x,y, 4,  6, 10, 14, 26,  5,  9, 13, 17);
  GROSTL_COLUMN(x,y, 5,  5,  9, 13, 17,  6, 10, 14, 26);
  GROSTL_COLUMN(x,y, 6,  8, 12, 16, 28,  7, 11, 15, 19);
  GROSTL_COLUMN(x,y, 7,  7, 11, 15, 19,  8, 12, 16, 28);
  GROSTL_COLUMN(x,y, 8, 10, 14, 18, 30,  9, 13, 17, 21);
  GROSTL_COLUMN(x,y, 9,  9, 13, 17, 21, 10, 14, 18, 30);
  GROSTL_COLUMN(x,y,10, 12, 16, 20,  0, 11, 15, 19, 23);
  GROSTL_COLUMN(x,y,11, 11, 15, 19, 23, 12, 16, 20,  0);
  GROSTL_COLUMN(x,y,12, 14, 18, 22,  2, 13, 17, 21, 25);
  GROSTL_COLUMN(x,y,13, 13, 17, 21, 25, 14, 18, 22,  2);
  GROSTL_COLUMN(x,y,14, 16, 20, 24,  4, 15, 19, 23, 27);
  GROSTL_COLUMN(x,y,15, 15, 19, 23, 27, 16, 20, 24,  4);

  GROSTL_COLUMN(x,y,16, 18, 22, 26,  6, 17, 21, 25, 29);
  GROSTL_COLUMN(x,y,17, 17, 21, 25, 29, 18, 22, 26,  6);
  GROSTL_COLUMN(x,y,18, 20, 24, 28,  8, 19, 23, 27, 31);
  GROSTL_COLUMN(x,y,19, 19, 23, 27, 31, 20, 24, 28,  8);
  GROSTL_COLUMN(x,y,20, 22, 26, 30, 10, 21, 25, 29,  1);
  GROSTL_COLUMN(x,y,21, 21, 25, 29,  1, 22, 26, 30, 10);
  GROSTL_COLUMN(x,y,22, 24, 28,  0, 12, 23, 27, 31,  3);
  GROSTL_COLUMN(x,y,23, 23, 27, 31,  3, 24, 28,  0, 12);
  GROSTL_COLUMN(x,y,24, 26, 30,  2, 14, 25, 29,  1,  5);
  GROSTL_COLUMN(x,y,25, 25, 29,  1,  5, 26, 30,  2, 14);
  GROSTL_COLUMN(x,y,26, 28,  0,  4, 16, 27, 31,  3,  7);
  GROSTL_COLUMN(x,y,27, 27, 31,  3,  7, 28,  0,  4, 16);
  GROSTL_COLUMN(x,y,28, 30,  2,  6, 18, 29,  1,  5,  9);
  GROSTL_COLUMN(x,y,29, 29,  1,  5,  9, 30,  2,  6, 18);
  GROSTL_COLUMN(x,y,30,  0,  4,  8, 20, 31,  3,  7, 11);
  GROSTL_COLUMN(x,y,31, 31,  3,  7, 11,  0,  4,  8, 20);
}


/* compute compression function (short variants) */
void GROSTL_F512(grostl_u32 *h, const grostl_u32 *m, const int rounds512) {
  int i;
  grostl_u32 Ptmp[2*GROSTL_COLS512];
  grostl_u32 Qtmp[2*GROSTL_COLS512];
  grostl_u32 y[2*GROSTL_COLS512];
  grostl_u32 z[2*GROSTL_COLS512];

  for (i = 0; i < 2*GROSTL_COLS512; i++) {
    z[i] = m[i];
    Ptmp[i] = h[i]^m[i];
  }

  /* compute Q(m) */
  if (rounds512 >= 1) GROSTL_RND512Q(z, y, GROSTL_U32BIG((grostl_u32)0x00000000u));
  if (rounds512 >= 2) GROSTL_RND512Q(y, z, GROSTL_U32BIG((grostl_u32)0x00000001u));
  if (rounds512 >= 3) GROSTL_RND512Q(z, y, GROSTL_U32BIG((grostl_u32)0x00000002u));
  if (rounds512 >= 4) GROSTL_RND512Q(y, z, GROSTL_U32BIG((grostl_u32)0x00000003u));
  if (rounds512 >= 5) GROSTL_RND512Q(z, y, GROSTL_U32BIG((grostl_u32)0x00000004u));
  if (rounds512 >= 6) GROSTL_RND512Q(y, z, GROSTL_U32BIG((grostl_u32)0x00000005u));
  if (rounds512 >= 7) GROSTL_RND512Q(z, y, GROSTL_U32BIG((grostl_u32)0x00000006u));
  if (rounds512 >= 8) GROSTL_RND512Q(y, z, GROSTL_U32BIG((grostl_u32)0x00000007u));
  if (rounds512 >= 9) GROSTL_RND512Q(z, y, GROSTL_U32BIG((grostl_u32)0x00000008u));
  if (rounds512 >= 10) GROSTL_RND512Q(y, Qtmp, GROSTL_U32BIG((grostl_u32)0x00000009u));

  /* compute P(h+m) */
  if (rounds512 >= 1) GROSTL_RND512P(Ptmp, y, GROSTL_U32BIG((grostl_u32)0x00000000u));
  if (rounds512 >= 2) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x01000000u));
  if (rounds512 >= 3) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x02000000u));
  if (rounds512 >= 4) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x03000000u));
  if (rounds512 >= 5) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x04000000u));
  if (rounds512 >= 6) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x05000000u));
  if (rounds512 >= 7) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x06000000u));
  if (rounds512 >= 8) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x07000000u));
  if (rounds512 >= 9) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x08000000u));
  if (rounds512 >= 10) GROSTL_RND512P(y, Ptmp, GROSTL_U32BIG((grostl_u32)0x09000000u));

  /* compute P(h+m) + Q(m) + h */
  for (i = 0; i < 2*GROSTL_COLS512; i++) {
    h[i] ^= Ptmp[i]^Qtmp[i];
  }
}

/* compute compression function (long variants) */
void GROSTL_F1024(grostl_u32 *h, const grostl_u32 *m, const int rounds1024) {
  int i;
  grostl_u32 Ptmp[2*GROSTL_COLS1024];
  grostl_u32 Qtmp[2*GROSTL_COLS1024];
  grostl_u32 y[2*GROSTL_COLS1024];
  grostl_u32 z[2*GROSTL_COLS1024];

  for (i = 0; i < 2*GROSTL_COLS1024; i++) {
    z[i] = m[i];
    Ptmp[i] = h[i]^m[i];
  }

  /* compute Q(m) */
  GROSTL_RND1024Q(z, y, GROSTL_U32BIG((grostl_u32)0x00000000u));
  for (i = 1; i < rounds1024-1; i += 2) {
    GROSTL_RND1024Q(y, z, GROSTL_U32BIG((grostl_u32)i));
    GROSTL_RND1024Q(z, y, GROSTL_U32BIG((grostl_u32)i+1));
  }
  GROSTL_RND1024Q(y, Qtmp, GROSTL_U32BIG((grostl_u32)0x0000000du));

  /* compute P(h+m) */
  GROSTL_RND1024P(Ptmp, y, GROSTL_U32BIG((grostl_u32)0x00000000u));
  for (i = 1; i < rounds1024-1; i += 2) {
    GROSTL_RND1024P(y, z, GROSTL_U32BIG((grostl_u32)i<<24));
    GROSTL_RND1024P(z, y, GROSTL_U32BIG(((grostl_u32)i+1)<<24));
  }
  GROSTL_RND1024P(y, Ptmp, GROSTL_U32BIG(((grostl_u32)rounds1024-1)<<24));

  /* compute P(h+m) + Q(m) + h */
  for (i = 0; i < 2*GROSTL_COLS1024; i++) {
    h[i] ^= Ptmp[i]^Qtmp[i];
  }
}


/* digest up to msglen bytes of input (full blocks only) */
void Grostl::Transform(hashState *ctx, 
	       const grostl_u8 *input, 
	       int msglen, const int rounds512, const int rounds1024) {
  /* determine variant, SHORT or LONG, and select underlying
     compression function based on the variant */
  void (*F)(grostl_u32*,const grostl_u32*, const int);
  int selectedRounds;
  switch ( ctx->v ) {
  case SHORT : { F = &GROSTL_F512; selectedRounds = rounds512; break; }
  case LONG  : 
  default    : { F = &GROSTL_F1024; selectedRounds = rounds1024; break; }
  }

  /* digest message, one block at a time */
  for (; msglen >= ctx->statesize; 
       msglen -= ctx->statesize, input += ctx->statesize) {
    F(ctx->chaining,(grostl_u32*)input, selectedRounds);

    /* increment block counter */
    ctx->block_counter1++;
    if (ctx->block_counter1 == 0) ctx->block_counter2++;
  }
}

/* given state h, do h <- P(h)+h */
void Grostl::OutputTransformation(hashState *ctx, const int rounds512, const int rounds1024) {
  int j;
  grostl_u32 *temp, *y, *z;
  temp = (grostl_uint_32t *) malloc(2*ctx->columns*sizeof(grostl_u32));
  y    = (grostl_uint_32t *) malloc(2*ctx->columns*sizeof(grostl_u32));
  z    = (grostl_uint_32t *) malloc(2*ctx->columns*sizeof(grostl_u32));

  /* determine variant */
  switch (ctx->v) {
  case SHORT :
    for (j = 0; j < 2*GROSTL_COLS512; j++) {
      temp[j] = ctx->chaining[j];
    }
	if (rounds512 >= 1) GROSTL_RND512P(temp, y, GROSTL_U32BIG((grostl_u32)0x00000000u));
    if (rounds512 >= 2) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x01000000u));
    if (rounds512 >= 3) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x02000000u));
    if (rounds512 >= 4) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x03000000u));
    if (rounds512 >= 5) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x04000000u));
    if (rounds512 >= 6) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x05000000u));
    if (rounds512 >= 7) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x06000000u));
    if (rounds512 >= 8) GROSTL_RND512P(y, z, GROSTL_U32BIG((grostl_u32)0x07000000u));
    if (rounds512 >= 9) GROSTL_RND512P(z, y, GROSTL_U32BIG((grostl_u32)0x08000000u));
    /*if (rounds512 >= 10)*/ GROSTL_RND512P(y, temp, GROSTL_U32BIG((grostl_u32)0x09000000u));
    for (j = 0; j < 2*GROSTL_COLS512; j++) {
      ctx->chaining[j] ^= temp[j];
    }
    break;
  case LONG  :
    for (j = 0; j < 2*GROSTL_COLS1024; j++) {
      temp[j] = ctx->chaining[j];
    }
    GROSTL_RND1024P(temp,y,GROSTL_U32BIG((grostl_u32)0x00000000u));
    for (j = 1; j < rounds1024-1; j += 2) {
      GROSTL_RND1024P(y,z,GROSTL_U32BIG((grostl_u32)j<<24));
      GROSTL_RND1024P(z,y,GROSTL_U32BIG(((grostl_u32)j+1)<<24));
    }
    GROSTL_RND1024P(y,temp,GROSTL_U32BIG(((grostl_u32)rounds1024-1)<<24));
    for (j = 0; j < 2*GROSTL_COLS1024; j++) {
      ctx->chaining[j] ^= temp[j];
    }
    break;
  }

  free(temp);
  free(y);
  free(z);
}

/* initialise context */
int Grostl::Init(int hashbitlen) {
  /* output size (in bits) must be a positive integer less than or
     equal to 512, and divisible by 8 */
  if (hashbitlen <= 0 || (hashbitlen%8) || hashbitlen > 512)
    return BAD_HASHLEN;

  /* set number of state columns and state size depending on
     variant */
  if (hashbitlen <= 256) {
    grostlState.columns = GROSTL_COLS512;
    grostlState.statesize = GROSTL_SIZE512;
    grostlState.v = SHORT;
  }
  else {
    grostlState.columns = GROSTL_COLS1024;
    grostlState.statesize = GROSTL_SIZE1024;
    grostlState.v = LONG;
  }

  /* allocate memory for state and data buffer */
  grostlState.chaining = (grostl_uint_32t *) calloc(grostlState.statesize,1);
  grostlState.buffer = (BitSequence *) malloc(grostlState.statesize);
  if (grostlState.chaining == NULL || grostlState.buffer == NULL)
    return FAIL;

  /* set initial value */
  grostlState.chaining[2*grostlState.columns-1] = GROSTL_U32BIG((grostl_u32)hashbitlen);

  /* set other variables */
  grostlState.hashbitlen = hashbitlen;
  grostlState.buf_ptr = 0;
  grostlState.block_counter1 = 0;
  grostlState.block_counter2 = 0;
  grostlState.bits_in_last_byte = 0;

  return SUCCESS;
}

/* update state with databitlen bits of input */
int Grostl::Update(const BitSequence* input,
		  DataLength databitlen) {
  int index = 0;
  int msglen = (int)(databitlen/8);
  int rem = (int)(databitlen%8);

  /* non-integral number of message bytes can only be supplied in the
     last call to this function */
  if (grostlState.bits_in_last_byte) return FAIL;

  /* if the buffer contains data that has not yet been digested, first
     add data to buffer until full */
  if (grostlState.buf_ptr) {
    while (grostlState.buf_ptr < grostlState.statesize && index < msglen) {
      grostlState.buffer[(int)grostlState.buf_ptr++] = input[index++];
    }
    if (grostlState.buf_ptr < grostlState.statesize) {
      /* buffer still not full, return */
      if (rem) {
	grostlState.bits_in_last_byte = rem;
	grostlState.buffer[(int)grostlState.buf_ptr++] = input[index];
      }
      return SUCCESS;
    }

    /* digest buffer */
    grostlState.buf_ptr = 0;
    Transform(&grostlState, grostlState.buffer, grostlState.statesize, grostlNumRounds512, grostlNumRounds1024);
  }

  /* digest bulk of message */
  Transform(&grostlState, input+index, msglen-index, grostlNumRounds512, grostlNumRounds1024);
  index += ((msglen-index)/grostlState.statesize)*grostlState.statesize;

  /* store remaining data in buffer */
  while (index < msglen) {
    grostlState.buffer[(int)grostlState.buf_ptr++] = input[index++];
  }

  /* if non-integral number of bytes have been supplied, store
     remaining bits in last byte, together with information about
     number of bits */
  if (rem) {
    grostlState.bits_in_last_byte = rem;
    grostlState.buffer[(int)grostlState.buf_ptr++] = input[index];
  }
  return SUCCESS;
}

#define BILB grostlState.bits_in_last_byte

/* finalise: process remaining data (including padding), perform
   output transformation, and write hash result to 'output' */
int Grostl::Final(BitSequence* output) {
  int i, j = 0, hashbytelen = grostlState.hashbitlen/8;
  grostl_u8 *s = (BitSequence*)grostlState.chaining;

  /* pad with '1'-bit and first few '0'-bits */
  if (BILB) {
    grostlState.buffer[(int)grostlState.buf_ptr-1] &= ((1<<BILB)-1)<<(8-BILB);
    grostlState.buffer[(int)grostlState.buf_ptr-1] ^= 0x1<<(7-BILB);
    BILB = 0;
  }
  else grostlState.buffer[(int)grostlState.buf_ptr++] = 0x80;

  /* pad with '0'-bits */
  if (grostlState.buf_ptr > grostlState.statesize-GROSTL_LENGTHFIELDLEN) {
    /* padding requires two blocks */
    while (grostlState.buf_ptr < grostlState.statesize) {
      grostlState.buffer[(int)grostlState.buf_ptr++] = 0;
    }
    /* digest first padding block */
    Transform(&grostlState, grostlState.buffer, grostlState.statesize, grostlNumRounds512, grostlNumRounds1024);
    grostlState.buf_ptr = 0;
  }
  while (grostlState.buf_ptr < grostlState.statesize-GROSTL_LENGTHFIELDLEN) {
    grostlState.buffer[(int)grostlState.buf_ptr++] = 0;
  }

  /* length padding */
  grostlState.block_counter1++;
  if (grostlState.block_counter1 == 0) grostlState.block_counter2++;
  grostlState.buf_ptr = grostlState.statesize;

  while (grostlState.buf_ptr > grostlState.statesize-(int)sizeof(grostl_u32)) {
    grostlState.buffer[(int)--grostlState.buf_ptr] = (grostl_u8)grostlState.block_counter1;
    grostlState.block_counter1 >>= 8;
  }
  while (grostlState.buf_ptr > grostlState.statesize-GROSTL_LENGTHFIELDLEN) {
    grostlState.buffer[(int)--grostlState.buf_ptr] = (grostl_u8)grostlState.block_counter2;
    grostlState.block_counter2 >>= 8;
  }

  /* digest final padding block */
  Transform(&grostlState, grostlState.buffer, grostlState.statesize, grostlNumRounds512, grostlNumRounds1024);
  /* perform output transformation */
  OutputTransformation(&grostlState, grostlNumRounds512, grostlNumRounds1024);

  /* store hash result in output */
  for (i = grostlState.statesize-hashbytelen; i < grostlState.statesize; i++,j++) {
    output[j] = s[i];
  }

  /* zeroise relevant variables and deallocate memory */
  for (i = 0; i < grostlState.columns; i++) {
    grostlState.chaining[i] = 0;
  }
  for (i = 0; i < grostlState.statesize; i++) {
    grostlState.buffer[i] = 0;
  }
  free(grostlState.chaining);
  free(grostlState.buffer);

  return SUCCESS;
}

/* hash bit sequence */
int Grostl::Hash(int hashbitlen,
		const BitSequence* data, 
		DataLength databitlen,
		BitSequence* hashval) {
  int ret;
  //hashState context;

  /* initialise */
  if ((ret = Grostl::Init(hashbitlen)) != SUCCESS)
    return ret;

  /* process message */
  if ((ret = Grostl::Update(data, databitlen)) != SUCCESS)
    return ret;

  /* finalise */
  ret = Grostl::Final(hashval);

  return ret;
}

void Grostl::PrintHash(const BitSequence* hash,
	       int hashbitlen) {
  int i;
  for (i = 0; i < hashbitlen/8; i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");
}

Grostl::Grostl(const int numRounds) {
	if (numRounds == -1) {
		grostlNumRounds512 = GROSTL_ROUNDS512;
		grostlNumRounds1024 = GROSTL_ROUNDS1024;
	} else {
		grostlNumRounds512 = numRounds;
		grostlNumRounds1024 = numRounds;
	}
}