#include <string.h>
#include "hamsi.h"
//#include "words.h"
#include "hamsi-tables.h"
#include "hamsi-internals.h"

#define MOV(r,y) for (i=0; i<arrlen; ++i) s[r][i] =  s[y][i];
#define XOR(r,y) for (i=0; i<arrlen; ++i) s[r][i] ^= s[y][i];
#define  OR(r,y) for (i=0; i<arrlen; ++i) s[r][i] |= s[y][i];
#define AND(r,y) for (i=0; i<arrlen; ++i) s[r][i] &= s[y][i];
#define MOT(r,y) for (i=0; i<arrlen; ++i) s[r][i] = ~s[y][i];
#define NOT(r)   for (i=0; i<arrlen; ++i) s[r][i] = ~s[r][i];

// From "Speeding up Serpent", by Dag Arne Osvik, University of Bergen
#define SUBST(size,s)                   \
    do {                                \
        int i;                          \
        const int arrlen=(size/64);     \
        MOV(4,0);     AND(0,2);         \
        XOR(0,3);     XOR(2,1);         \
        XOR(2,0);      OR(3,4);         \
        XOR(3,1);     XOR(4,2);         \
\
        MOV(1,3);      OR(3,4);         \
        XOR(3,0);     AND(0,1);         \
        XOR(4,0);     XOR(1,3);         \
        XOR(1,4);     MOT(4,4);         \
\
        MOV(0,2);                       \
        MOV(2,1);                       \
        MOV(1,3);                       \
        MOV(3,4);                       \
    } while (0)

#define INLINE

INLINE void subst256(unsigned int s[5][4]) {
    SUBST(256,s);
}

INLINE void subst512(unsigned int s[5][8]) {
    SUBST(512,s);
}

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))
INLINE void Mix(unsigned int* a,unsigned int* b,unsigned int* c,unsigned int* d) {
    *a = ROTL32(*a,13);
    *c = ROTL32(*c,3);
    *b ^= *a ^ *c;
    *d ^= *c ^ (*a<<3);

    *b = ROTL32(*b,1);
    *d = ROTL32(*d,7);
    *a ^= *b ^ *d;
    *c ^= *d ^ (*b<<7);

    *a = ROTL32(*a,5);
    *c = ROTL32(*c,22);
}

#define L Mix
#define S(r,c) &s[r][c]
INLINE void diffuse256(unsigned int s[4][4]) {
    // const int line=4;
    L(S(0,0),S(1,1),S(2,2),S(3,3));
    L(S(0,1),S(1,2),S(2,3),S(3,0));
    L(S(0,2),S(1,3),S(2,0),S(3,1));
    L(S(0,3),S(1,0),S(2,1),S(3,2));
}

INLINE void diffuse512(unsigned int s[4][8]) {
    // const int line=8;
    L(S(0,0),S(1,1),S(2,2),S(3,3));
    L(S(0,1),S(1,2),S(2,3),S(3,4));
    L(S(0,2),S(1,3),S(2,4),S(3,5));
    L(S(0,3),S(1,4),S(2,5),S(3,6));
    L(S(0,4),S(1,5),S(2,6),S(3,7));
    L(S(0,5),S(1,6),S(2,7),S(3,0));
    L(S(0,6),S(1,7),S(2,0),S(3,1));
    L(S(0,7),S(1,0),S(2,1),S(3,2));

    L(S(0,0),S(0,2),S(0,5),S(0,7));
    L(S(1,1),S(1,3),S(1,4),S(1,6));
    L(S(2,0),S(2,3),S(2,5),S(2,6));
    L(S(3,1),S(3,2),S(3,4),S(3,7));
}

// #define T1(j)       hamsi_T256[4096*j+8*d[j]+i]
// #define T5(j)       hamsi_T512[16384*j+16*d[j]+i]
#define T1(j)       hamsi_T256[j][d[j]][i]
#define T5(j)       hamsi_T512[j][d[j]][i]
#define Exp256(i) (T1(0)^T1(1)^T1(2)^T1(3))
#define Exp512(i) (T5(0)^T5(1)^T5(2)^T5(3))

int hamsi_hash256(const int ROUNDS, unsigned int*cv, const unsigned char*d, int lastiter) {
    const int arrlen=4;
    static unsigned int s[5][4];
    int i;

    // Concatenation
    for (i=0; i<8; ++i) {
        s[i/2][i%4] = Exp256(i);    // from message expansion
        s[i/2][(i+2)%4] = cv[i];    // from chain value
    }

    for (i=0; i<ROUNDS; ++i) {
        int r,c;
        // Add Constants
        for (r=0; r<4; ++r) for (c=0; c<arrlen; ++c) s[r][c] ^= hamsi_alpha[lastiter][r][c];
        s[0][1] ^= i;
        subst256(s);
        diffuse256(s);
    }

    // truncation
    for (i=0; i<arrlen; ++i) { cv[i] ^= s[0][i]; cv[i+arrlen] ^= s[2][i]; }

    return 0;
}

int hamsi_hash512(const int ROUNDS, unsigned int*cv, const unsigned char*d, int lastiter) {
    const int arrlen=8;
    static unsigned int s[5][8];
    int i;

    // Concatenation
    for (i=0; i<16; ++i) {
        s[i/4][(i%4)+2*((i/2)%2)+2*((i/4)%2)] = Exp512(i);    // from message expansion
        s[i/4][(i%4)+2*((i/2)%2)+2*(((i/4)+1)%2)] = cv[i];    // from chain value
    }

    for (i=0; i<ROUNDS; ++i) {
        int r,c;
        // Add Constants
        for (r=0; r<4; ++r) for (c=0; c<arrlen; ++c) s[r][c] ^= hamsi_alpha[lastiter][r][c];
        s[0][1] ^= i;
        subst512(s);
        diffuse512(s);
    }

    // truncation
    for (i=0; i<arrlen; ++i) { cv[i] ^= s[0][i]; cv[i+arrlen] ^= s[2][i]; }

    return 0;
}
