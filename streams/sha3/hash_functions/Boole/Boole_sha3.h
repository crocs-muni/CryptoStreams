#ifndef BOOLE_SHA3_H
#define BOOLE_SHA3_H

#include "../../sha3_interface.h"

class Boole : public sha3_interface {

private:
#ifdef _WIN32
#define inline __inline
#endif

#ifndef _BLE_DEFINED
#define _BLE_DEFINED 1

/* Get nice, portable integer size constants */
#if __STDC_VERSION__ >= 199901
#include <stdint.h>
#endif

/*
 * Definitions for Boole, and the SHA-3 interface.
 * WORDSIZE defines the variant of Boole.
 */
#ifndef BOOLE_WORDSIZE
#define BOOLE_WORDSIZE 64
#endif

/*
 * The following section is intended to define "WORD" to be an
 * appropriate unsigned integer type with WORDSIZE bits, and to define
 * some macros that work properly with WORDs even on machines where
 * the native integer type is larger. This is really hard stuff to get right.
 */
#if BOOLE_WORDSIZE == 16
#   define BOOLE_WORDSHIFT 4
#   if __STDC_VERSION__ >= 199901
#       define BOOLE_WORD uint16_t
#       define BOOLE_WORD_MAX UINT16_MAX
#   elif defined(_WIN32) || defined(_WIN64)
#       define BOOLE_WORD unsigned __int16
#       define BOOLE_WORD_MAX _UI16_MAX
#   else
#       define BOOLE_WORD unsigned short
#       define BOOLE_WORD_MAX USHORT_MAX
#   endif

#   if BOOLE_WORD_MAX == 0xFFFF
#       define BOOLE_ROTL(w,x) (((w) << (x))|((w) >> (16 - (x))))
#       define BOOLE_ROTR(w,x) (((w) >> (x))|((w) << (16 - (x))))
#   else
#       define BOOLE_ROTL(w,x) (((w) << (x))|(((w) & 0xFFFF) >> (16 - (x))))
#       define BOOLE_ROTR(w,x) ((((w) & 0xFFFF) >> (x))|((w) << (16 - (x))))
#   endif
/* end BOOLE_WORDSIZE 16 */

#elif BOOLE_WORDSIZE == 32
#   define BOOLE_WORDSHIFT 5
#   if __STDC_VERSION__ >= 199901 
#       define BOOLE_WORD uint32_t 
#       define BOOLE_WORD_MAX UINT32_MAX
#   elif defined(_WIN32) || defined(_WIN64)
#       define BOOLE_WORD unsigned __int32
#       define BOOLE_WORD_MAX _UI32_MAX
#   elif UINT_MAX >= 0xFFFFFFFF 
#       define BOOLE_WORD unsigned int
#       define BOOLE_WORD_MAX UINT_MAX
#   else
#       define BOOLE_WORD unsigned long
#       define BOOLE_WORD_MAX ULONG_MAX
#   endif

#   if BOOLE_WORD_MAX == 0xFFFFFFFF
#       define BOOLE_ROTL(w,x) (((w) << (x))|((w) >> (32 - (x))))
#       define BOOLE_ROTR(w,x) (((w) >> (x))|((w) << (32 - (x))))
#   else
#       define BOOLE_ROTL(w,x) (((w) << (x))|(((w) & 0xFFFFFFFF) >> (32 - (x))))
#       define BOOLE_ROTR(w,x) ((((w) & 0xFFFFFFFF) >> (x))|((w) << (32 - (x))))
#   endif
/* end BOOLE_WORDSIZE 32 */

#elif BOOLE_WORDSIZE == 64
#   define BOOLE_WORDSHIFT 6
#   if __STDC_VERSION__ >= 199901 
#       define BOOLE_WORD uint64_t 
#       define BOOLE_WORD_MAX UINT64_MAX
#   elif defined(_WIN32) || defined(_WIN64)
#       define BOOLE_WORD unsigned __int64
#       define BOOLE_WORD_MAX _UI64_MAX
#   elif UINT_MAX >= 0xFFFFFFFFFFFFFFFF
#       define BOOLE_WORD unsigned int
#       define BOOLE_WORD_MAX UINT_MAX
#   elif ULONG_MAX >= 0xFFFFFFFFFFFFFFFF
#       define BOOLE_WORD unsigned long
#       define BOOLE_WORD_MAX ULONG_MAX
#   else
#       define BOOLE_WORD unsigned long long
#       define BOOLE_WORD_MAX ULLONG_MAX
#   endif

#   if BOOLE_WORD_MAX == 0xFFFFFFFFFFFFFFFF
#     define BOOLE_ROTL(w,x) (((w) << (x))|((w) >> (64 - (x))))
#     define BOOLE_ROTR(w,x) (((w) >> (x))|((w) << (64 - (x))))
#   else
#     define BOOLE_ROTL(w,x) (((w) << (x))|(((w) & 0xFFFFFFFFFFFFFFFF) >> (64 - (x))))
#     define BOOLE_ROTR(w,x) ((((w) & 0xFFFFFFFFFFFFFFFF) >> (x))|((w) << (64 - (x))))
#   endif
/* end BOOLE_WORDSIZE 64 */

#else
#   error invalid BOOLE_WORDSIZE specified
#endif

/*
 * important definitions
 */
#define BOOLE_N	16 /* number of register elements, and diffusion iterations */

typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	WARN_HASHBITLEN = 2, /* warning when length greater than expected security */
	BAD_TERMINATION = 3, /* update called after odd-sized call to update */
	BAD_NEEDNONCE = 4, /* attempt to use MAC/stream without nonce */
} HashReturn;

/*
 * state structure for hash/MAC
 */
typedef struct {
    int		hashbitlen;	/* number of bits of output -- 0 for stream */
    DataLength	nbits;		/* bits of input seen so far */
    BOOLE_WORD	R[BOOLE_N];		/* Working storage for the shift register */
    BOOLE_WORD	xsum;		/* XOR sum of input words */
    BOOLE_WORD	lsum;		/* rotating addition sum of input words */
    BOOLE_WORD	rsum;		/* rotating addition sum of input words */
    /* the following handle non-whole-word input/output */
    BOOLE_WORD	bbuf;		/* partial word buffer */
    int		nbuf;		/* number of part-word bits buffered */
    /* the following used for circular buffer fast implementation only */
    int		z;		/* current zero position */
} hashState;

/*
 * state structure for stream cipher (including hash/MAC)
 */
typedef struct {
    hashState	h;		/* includes state for hash/MAC */
    hashState	s;		/* state for stream cipher only */
    BOOLE_WORD	initR[BOOLE_N];	/* copy of post-key register to avoid rekey */
    int		neednonce;	/* nonce must be called */
} ble_ctx;

/*
 * some other useful definitions for WORDs
 */
#define BOOLE_BPW	(BOOLE_WORDSIZE/8)	/* Bytes per WORD */
#define BOOLE_BYTESIN(x) (((x)+7)>>3) /* how many bytes are needed for x bits */
#define BOOLE_WORDMASK ((BOOLE_WORD)(BOOLE_WORDSIZE-1)) /* mask for low order bits */

private:
int booleNumRounds;
hashState booleState;

/*
 * external interface definitions for SHA-3
 */
public:
Boole(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data,	DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

/*
 * external non SHA-3 interface definitions (for stream and MAC)
 */
private:
int ble_key(ble_ctx *c, const unsigned char key[], int keylen, int maclen); /* set key */
int ble_nonce(ble_ctx *c, const unsigned char nonce[], int nlen); /* set Init Vector */
int ble_stream(ble_ctx *c, unsigned char *buf, int nbits); /* stream cipher */
int ble_macdata(ble_ctx *c, unsigned char *buf, int nbits); /* accumulate MAC */
int ble_encrypt(ble_ctx *c, unsigned char *buf, int nbits); /* enc+MAC */
int ble_decrypt(ble_ctx *c, unsigned char *buf, int nbits); /* dec+MAC */
int ble_mac(ble_ctx *c, BitSequence *hashval); /* finalize MAC */

static void cycle(hashState *c, const int rounds);
static void datacycle(hashState *c, BOOLE_WORD i, const int rounds);
static BOOLE_WORD streamcycle(hashState *c, const int rounds);
static void ble_softreset(hashState *c);
static void ble_initstate(hashState *c, const int rounds);
static void ble_diffuse(hashState *c, const int rounds);
static void ble_finish(hashState *c, const int rounds);
int ble_gen(hashState *c, unsigned char *buf, int nbits, const int rounds);
static int ble_update(hashState *c, const unsigned char *buf, DataLength nbits, const int rounds);

#endif
};

#endif