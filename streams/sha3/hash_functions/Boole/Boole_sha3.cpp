#include <stdlib.h>
#include <string.h>
#include "Boole_sha3.h"

#define BOOLE_INITSUM 0x6996c53a	/* initial value of sums */

/* some useful macros -- machine independent little-endian */
#define Boole_Byte(x,i) ((unsigned char)(((x) >> (8*(i))) & 0xFF))

#if BOOLE_WORDSIZE == 64
#define BOOLE_BYTE2WORD(b) ( \
	(((BOOLE_WORD)(b)[7] & 0xFF)<<56) | \
	(((BOOLE_WORD)(b)[6] & 0xFF)<<48) | \
	(((BOOLE_WORD)(b)[5] & 0xFF)<<40) | \
	(((BOOLE_WORD)(b)[4] & 0xFF)<<32) | \
	(((BOOLE_WORD)(b)[3] & 0xFF)<<24) | \
	(((BOOLE_WORD)(b)[2] & 0xFF)<<16) | \
	(((BOOLE_WORD)(b)[1] & 0xFF)<<8) | \
	(((BOOLE_WORD)(b)[0] & 0xFF)))
#elif BOOLE_WORDSIZE == 32
#define BOOLE_BYTE2WORD(b) ( \
	(((BOOLE_WORD)(b)[3] & 0xFF)<<24) | \
	(((BOOLE_WORD)(b)[2] & 0xFF)<<16) | \
	(((BOOLE_WORD)(b)[1] & 0xFF)<<8) | \
	(((BOOLE_WORD)(b)[0] & 0xFF)))
#else
#define BOOLE_BYTE2WORD(b) ( \
	(((BOOLE_WORD)(b)[1] & 0xFF)<<8) | \
	(((BOOLE_WORD)(b)[0] & 0xFF)))
#endif

#if BOOLE_WORDSIZE == 64
#define BOOLE_XORWORD(w, b) { \
	(b)[7] ^= Boole_Byte(w,7); \
	(b)[6] ^= Boole_Byte(w,6); \
	(b)[5] ^= Boole_Byte(w,5); \
	(b)[4] ^= Boole_Byte(w,4); \
	(b)[3] ^= Boole_Byte(w,3); \
	(b)[2] ^= Boole_Byte(w,2); \
	(b)[1] ^= Boole_Byte(w,1); \
	(b)[0] ^= Boole_Byte(w,0); \
    }
#elif BOOLE_WORDSIZE == 32
#define BOOLE_XORWORD(w, b) { \
	(b)[3] ^= Boole_Byte(w,3); \
	(b)[2] ^= Boole_Byte(w,2); \
	(b)[1] ^= Boole_Byte(w,1); \
	(b)[0] ^= Boole_Byte(w,0); \
    }
#else
#define BOOLE_XORWORD(w, b) { \
	(b)[1] ^= Boole_Byte(w,1); \
	(b)[0] ^= Boole_Byte(w,0); \
    }
#endif

/* Nonlinear transform (sbox) of a word.
 * There are two slightly different combinations.
 */
#if BOOLE_WORDSIZE == 16
inline static BOOLE_WORD
boole_sbox1(BOOLE_WORD w)
{
    w ^= BOOLE_ROTL(w, 9)  | BOOLE_ROTL(w, 13);
    w ^= ~BOOLE_ROTL(w, 10) | BOOLE_ROTL(w, 15);
    return w;
}

inline static BOOLE_WORD
boole_sbox2(BOOLE_WORD w)
{
    w ^= BOOLE_ROTL(w, 3)  | BOOLE_ROTL(w, 14);
    w ^= ~BOOLE_ROTL(w, 9) | BOOLE_ROTL(w, 10);
    return w;
}

#elif BOOLE_WORDSIZE == 32
inline static BOOLE_WORD
boole_sbox1(BOOLE_WORD w)
{
    w ^= BOOLE_ROTL(w, 5)  | BOOLE_ROTL(w, 7);
    w ^= BOOLE_ROTL(w, 19) | BOOLE_ROTL(w, 22);
    return w;
}

inline static BOOLE_WORD
boole_sbox2(BOOLE_WORD w)
{
    w ^= BOOLE_ROTL(w, 7)  | BOOLE_ROTL(w, 22);
    w ^= BOOLE_ROTL(w, 5) | BOOLE_ROTL(w, 19);
    return w;
}

#elif BOOLE_WORDSIZE == 64
inline static BOOLE_WORD
boole_sbox1(BOOLE_WORD w)
{
    w ^= BOOLE_INITSUM;
    w ^= BOOLE_ROTL(w, 34)  | BOOLE_ROTL(w, 42);
    w ^= BOOLE_ROTL(w, 20)  | BOOLE_ROTL(w, 55);
    w ^= (w << 3) | BOOLE_ROTL(w, 60);
    return w;
}

inline static BOOLE_WORD
boole_sbox2(BOOLE_WORD w)
{
    w ^= BOOLE_INITSUM;
    w ^= BOOLE_ROTR(w, 35)  | BOOLE_ROTR(w, 46);
    w ^= BOOLE_ROTR(w, 27)  | BOOLE_ROTR(w, 52);
    w ^= (w >> 5) | BOOLE_ROTR(w, 55);
    return w;
}
#endif

/* cycle the contents of the register
 */
void
Boole::cycle(Boole::hashState *c, const int rounds)
{
    BOOLE_WORD	t;
    int		i;

    /* nonlinear feedback function */
    t = c->R[12] ^ c->R[13];
    t = boole_sbox1(t) ^ BOOLE_ROTL(c->R[0], 1);
    /* shift register */
    for (i = 1; i < rounds; ++i)
	c->R[i-1] = c->R[i];
    c->R[rounds-1] = t;
    /* feed forward */
    t = boole_sbox2(c->R[2] ^ c->R[15]);
    c->R[0] ^= t;
}

/* Incorporate data word into the state
 */
void
Boole::datacycle(Boole::hashState *c, BOOLE_WORD i, const int rounds)
{
    c->xsum ^= i;
    c->lsum = boole_sbox1(c->lsum) ^ i;
    c->rsum ^= c->lsum;
    c->lsum = BOOLE_ROTL(c->lsum, 1);
    c->rsum = BOOLE_ROTR(c->rsum, 1);
    c->R[3] ^= c->lsum; /* becomes input to sbox1 before shift */
    c->R[13] ^= c->rsum; /* becomes input to sbox2 after register shift */
	Boole::cycle(c, rounds);
}

/* Return a stream word from the state
 */
BOOLE_WORD
Boole::streamcycle(Boole::hashState *c, const int rounds)
{
	Boole::cycle(c, rounds);
    return c->R[0] ^ c->R[8] ^ c->R[12];
}

/* "soft reset" -- used whenever transitioning between input/output modes.
 * Basically sets the ancillary variables to a sane state.
 */
void
Boole::ble_softreset(Boole::hashState *c)
{
    c->nbits = 0;
    c->xsum = 0;
    c->lsum = (BOOLE_WORD) BOOLE_INITSUM; /* truncates in 16-bit case */
    c->rsum = (BOOLE_WORD) BOOLE_ROTL((BOOLE_WORD)BOOLE_INITSUM, 8); /* truncates in 16-bit case */
    c->nbuf = 0; /* no bits buffered for input or output */
    c->bbuf = 0; /* don't care */
}

/* initialise to known state for hash or pre-keying
 */
void
Boole::ble_initstate(Boole::hashState *c, const int rounds)
{
    int		i;

    c->R[0] = boole_sbox1((BOOLE_WORD)1);
    for (i = 1; i < rounds; ++i)
	c->R[i] = boole_sbox1(c->R[i-1]);
    /* reasonable values for everything else */
	Boole::ble_softreset(c);
}

/* Diffuse changes through the register
 */
void
Boole::ble_diffuse(Boole::hashState *c, const int rounds)
{
    int		i;

    for (i = 0; i < rounds; ++i)
	Boole::cycle(c, rounds);
}

/* Having accumulated data, finish absorbing it.
 */
void
Boole::ble_finish(Boole::hashState *c, const int rounds)
{
    int		i;
    DataLength	nbits;

    /* handle any previously buffered input bits or bytes */
    if (c->nbuf != 0) {
	Boole::datacycle(c, c->bbuf, rounds);
	/* hash finalization calls this twice... only do it once. */
	c->nbuf = 0;
    }
    
    /* Perturb the state to mark end of input.
     * c->nbits is exactly how many bits of input there were.
     * No further padding or disambiguation is necessary, so
     * long as this goes into the state.
     * The accumulated data is incorporated in a way that
     * can't otherwise be duplicated, frustrating extension
     * attacks.
     * Note also that since the register and the other data
     * words are dependent on the same data, this disturbance
     * is not easily invertible.
     */
    nbits = c->nbits;
    for (i = 0; i < sizeof(DataLength)/BOOLE_BPW; ++i) {
	c->R[i] ^= (BOOLE_WORD)nbits;
	/* shifting twice below is a crock to avoid a compiler warning
	 * in the 64-bit case.
	 */
	nbits >>= BOOLE_WORDSIZE/2;
	nbits >>= BOOLE_WORDSIZE/2;
    }
    /* everything else is already WORD sized. */
    c->R[4] ^= c->hashbitlen;
    for (i = 4; i < rounds; i += 3) {
	c->R[i] ^= c->lsum;
	c->R[i+1] ^= c->xsum;
	c->R[i+2] ^= c->rsum;
    }

	Boole::ble_diffuse(c, rounds);
}

/* XOR pseudo-random bytes into buffer (stream cipher).
 * c->nbuf is how many unused bits remain in c->bbuf.
 */
int
Boole::ble_gen(Boole::hashState *c, unsigned char *buf, int nbits, const int rounds)
{
    unsigned char       *endbuf;

    /* handle any previously buffered bits */
    if ((c->nbuf & 0x7) != 0) {
	/* attempt to generate more stream after partial bytes */
	return BAD_TERMINATION;
    }
    while (c->nbuf != 0 && nbits >= 8) {
	*buf++ ^= c->bbuf & 0xFF;
	c->bbuf >>= 8;
	c->nbuf -= 8;
	nbits -= 8;
    }

    /* handle whole words */
    endbuf = &buf[(nbits & ~BOOLE_WORDMASK) >> 3];
    while (buf < endbuf)
    {
	c->bbuf = streamcycle(c, rounds);
	BOOLE_XORWORD(c->bbuf, buf);
	buf += BOOLE_BPW;
    }

    /* handle any trailing bits */
    nbits &= BOOLE_WORDMASK;
    if (nbits != 0) {
	c->bbuf = Boole::streamcycle(c, rounds);
	c->nbuf = BOOLE_WORDSIZE;
	/* whole bytes */
	while (nbits >= 8) {
	    *buf++ ^= c->bbuf & 0xFF;
	    c->bbuf >>= 8;
	    c->nbuf -= 8;
	    nbits -= 8;
	}
	if (nbits != 0) {
	    /* partial byte left -- clobber bbuf, it can't be used again */
	    c->bbuf &= (0xFF & (0xFF00 >> nbits));
	    *buf ^= c->bbuf & 0xFF;
	    c->nbuf -= nbits;
	}
    }
    return SUCCESS;
}

/* accumulate words into hash/MAC
 * c->nbuf is how much space remains in c->bbuf.
 */
int
Boole::ble_update(Boole::hashState *c, const unsigned char *buf, DataLength nbits, const int rounds)
{
    const unsigned char *endbuf;
    BOOLE_WORD	t; /* used for partial byte tailings */

    /* handle any previously buffered bits */
    if ((c->nbuf & 0x7) != 0) {
	/* attempt to accumulate more data after partial bytes */
	return BAD_TERMINATION;
    }

    /* account for the bits */
    c->nbits += nbits;

    /* handle any previously buffered bytes */
    if (c->nbuf != 0) {
	while (c->nbuf != 0 && nbits >= 8) {
	    c->bbuf ^= (BOOLE_WORD)(*buf++) << (BOOLE_WORDSIZE - c->nbuf);
	    c->nbuf -= 8;
	    nbits -= 8;
	}
	if (c->nbuf != 0) {
	    /* not a whole word yet */
	    if (nbits != 0) {
		t = *buf & (0xFF00 >> nbits);
		c->bbuf ^= t << (BOOLE_WORDSIZE - c->nbuf);
		c->nbuf -= nbits;
	    }
	    return SUCCESS;
	}
	/* whole word gathered now */
	Boole::datacycle(c, c->bbuf, rounds);
    }

    /* handle whole words */
    endbuf = &buf[(nbits & ~BOOLE_WORDMASK) >> 3];
    while (buf < endbuf)
    {
	Boole::datacycle(c, BOOLE_BYTE2WORD(buf), rounds);
	buf += BOOLE_BPW;
    }

    /* handle any trailing bits */
    nbits &= BOOLE_WORDMASK;
    if (nbits != 0) {
	c->bbuf = 0;
	c->nbuf = BOOLE_WORDSIZE;
	while (nbits >= 8) {
	    /* buffer whole bytes */
	    c->bbuf ^= (BOOLE_WORD)(*buf++) << (BOOLE_WORDSIZE - c->nbuf);
	    c->nbuf -= 8;
	    nbits -= 8;
	}
	if (nbits != 0) {
	    /* partial byte to buffer still */
	    t = *buf & (0xFF00 >> nbits);
	    c->bbuf ^= t << (BOOLE_WORDSIZE - c->nbuf);
	    c->nbuf -= nbits;
	}
    }

    return SUCCESS;
}

/* ===== S T R E A M / M A C ===== */

/* Published "key" interface for stream/MAC
 */
int
Boole::ble_key(ble_ctx *c, const unsigned char key[], int keylen, int maclen)
{
    int		i;

	Boole::ble_initstate(&c->h, booleNumRounds);
    c->h.hashbitlen = maclen;
    c->s.hashbitlen = 0;
    Boole::ble_update(&c->h, key, (DataLength)keylen, booleNumRounds);
    Boole::ble_finish(&c->h, booleNumRounds); 
    Boole::ble_softreset(&c->h);
    /* save state of register */
    for (i = 0; i < booleNumRounds; ++i)
	c->initR[i] = c->h.R[i];
    c->neednonce = 1;
    if (maclen <= 0 || maclen > 8*BOOLE_WORDSIZE)
	return WARN_HASHBITLEN;
    return SUCCESS;
}

/* Published "nonce" interface
 * Set up the two sets of registers identically, but with
 * the initial register reversed for the stream cipher.
 * This leads to significantly different initial states.
 */
int
Boole::ble_nonce(ble_ctx *c, const unsigned char nonce[], int noncelen)
{
    int		i;

    for (i = 0; i < booleNumRounds; ++i)
	c->s.R[15 - i] = c->h.R[i] = c->initR[i];
    Boole::ble_softreset(&c->h);
    Boole::ble_softreset(&c->s);
    Boole::ble_update(&c->h, nonce, (DataLength)noncelen, booleNumRounds);
    Boole::ble_finish(&c->h, booleNumRounds); 
    Boole::ble_softreset(&c->h);
    Boole::ble_update(&c->s, nonce, (DataLength)noncelen, booleNumRounds);
    Boole::ble_finish(&c->s, booleNumRounds); 
    Boole::ble_softreset(&c->s);
    c->neednonce = 0;
    return SUCCESS;
}

/* published stream interface
 */
int
Boole::ble_stream(ble_ctx *c, unsigned char *buf, int nbits)
{
    if (c->neednonce)
	return BAD_NEEDNONCE;
    return Boole::ble_gen(&c->s, buf, nbits, booleNumRounds);
}

/* published MAC accumulation interface
 */
int
Boole::ble_macdata(ble_ctx *c, unsigned char *buf, int nbits)
{
    if (c->neednonce)
	return BAD_NEEDNONCE;
    return Boole::ble_update(&c->h, buf, (DataLength)nbits, booleNumRounds);
}

/* published encryption interface -- MAC based on ciphertext
 */
int
Boole::ble_encrypt(ble_ctx *c, unsigned char *buf, int nbits)
{
    //HashReturn	ret;
	int ret;

    if (c->neednonce)
	return BAD_NEEDNONCE;
    if ((ret = Boole::ble_gen(&c->s, buf, nbits, booleNumRounds)) != SUCCESS)
	return ret;
    return Boole::ble_update(&c->h, buf, (DataLength)nbits, booleNumRounds);
}

/* published decryption interface -- MAC based on ciphertext
 */
int
Boole::ble_decrypt(ble_ctx *c, unsigned char *buf, int nbits)
{
    //HashReturn	ret;
	int ret;

    if (c->neednonce)
	return BAD_NEEDNONCE;
    if ((ret = Boole::ble_update(&c->h, buf, (DataLength)nbits, booleNumRounds)) != SUCCESS)
	return ret;
    return Boole::ble_gen(&c->s, buf, nbits, booleNumRounds);
}

/* published MAC finalization interface
 */
int
Boole::ble_mac(ble_ctx *c, BitSequence *hashval)
{
    if (c->neednonce)
	return BAD_NEEDNONCE;
    Boole::ble_finish(&c->h, booleNumRounds);
    memset(hashval, 0, BOOLE_BYTESIN(c->h.hashbitlen));
    return Boole::ble_gen(&c->h, hashval, c->h.hashbitlen, booleNumRounds);
}

/* ====== H A S H ====== */

/* Published hash "Init" interface
 */
int
Boole::Init(int hashbitlen)
{
    Boole::ble_initstate(&booleState, booleNumRounds);
    booleState.hashbitlen = hashbitlen;
    if (hashbitlen <= 0 || hashbitlen > 8*BOOLE_WORDSIZE)
	return WARN_HASHBITLEN;
    return SUCCESS;
}

/* Published hash Update interface
 */
int
Boole::Update(const BitSequence *data, DataLength databitlen)
{
    return Boole::ble_update(&booleState, data, databitlen, booleNumRounds);
}

/* Published hash Finalization interface
 */
int
Boole::Final(BitSequence *hashval)
{
    if (booleState.hashbitlen <= 0)
	return WARN_HASHBITLEN;
    Boole::ble_finish(&booleState, booleNumRounds);
    Boole::ble_finish(&booleState, booleNumRounds); /* not a typo, do it twice! */
    memset(hashval, 0, BOOLE_BYTESIN(booleState.hashbitlen));
    return Boole::ble_gen(&booleState, hashval, booleState.hashbitlen, booleNumRounds);
}

/* Published "all in one" interface
 */
int
Boole::Hash(int hashbitlen, const BitSequence *data,
        DataLength databitlen, BitSequence *hashval)
{
    //HashReturn	ret;
	int ret;
    //hashState	s;
    //HashReturn	warn = SUCCESS;
	int warn = SUCCESS;

    if ((ret = Boole::Init(hashbitlen)) != SUCCESS) {
	if (ret == WARN_HASHBITLEN)
	    warn = ret;
	else
	    return ret;
    }
    if ((ret = Boole::Update(data, databitlen)) != SUCCESS)
	return ret;
    if ((ret = Boole::Final(hashval)) != SUCCESS)
	return ret;
    return warn;
}

Boole::Boole(const int numRounds) {
	if (numRounds == -1) {
		booleNumRounds = BOOLE_N;
	} else {
		booleNumRounds = numRounds;
	}
}