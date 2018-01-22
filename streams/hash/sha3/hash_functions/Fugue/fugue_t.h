#ifndef _FUGUE_T_H
#define _FUGUE_T_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#ifdef __unix__
#define INLINE inline
#else
#define INLINE __inline
#endif

#if defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN
#define FUGUE_HO2BE_8(_x)  (_x)
#define FUGUE_HO2BE_4(_x)  (_x)
#else
#define FUGUE_HO2BE_8(_x)  ((_x<<56)|((_x<<40)&0xff000000000000ull)|((_x<<24)&0xff0000000000ull)|((_x<<8)&0xff00000000ull)|\
                     ((_x>>8)&0xff000000ull)|((_x>>24)&0xff0000ull)|((_x>>40)&0xff00ull)|(_x>>56))
#define FUGUE_HO2BE_4(_x)  ((_x<<24)|((_x<<8)&0xff0000)|((_x>>8)&0xff00)|(_x>>24))
#endif

#if defined(__WORDSIZE) && __WORDSIZE == 64
typedef unsigned long       fugue_uint64;
typedef unsigned int        fugue_uint32;
#else
typedef unsigned long long  fugue_uint64;
typedef unsigned long       fugue_uint32;
#endif
typedef unsigned short      fugue_uint16;
typedef unsigned char       fugue_uint8;

typedef union {
    unsigned long    d;
    unsigned char     b[4];
    }         fugue_hash32_s;
typedef fugue_hash32_s* fugue_hash32_p;

typedef struct {
    int        n;   /* columns in output */
    int        s;   /* columns in state */
    int        k;   /* number of smix's per TIX or round */
    int        r;   /* number of G1 rounds in final part */
    int        t;   /* number of G2 rounds in final part */
    }          fugueHashCfg;

typedef struct {
    int        hashbitlen;
    fugueHashCfg*   Cfg;
    int        Base;
    fugue_hash32_s   State[36];
    unsigned long     Partial[1];
    unsigned long long     TotalBits;
    }          fugueHashState;

#endif
