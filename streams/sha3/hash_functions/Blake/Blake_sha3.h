#ifndef BLAKE_SHA3_H
#define BLAKE_SHA3_H

#include "../../sha3_interface.h"

class Blake : public sha3_interface {

//NASTAVENIE RUND:
#define BLAKE_NB_ROUNDS32 14
#define BLAKE_NB_ROUNDS64 16

/*typedef unsigned char BitSequence;
typedef unsigned long long DataLength;*/


#define BLAKE_U8TO32_BE(p) \
  (((unsigned int)((p)[0]) << 24) | \
   ((unsigned int)((p)[1]) << 16) | \
   ((unsigned int)((p)[2]) <<  8) | \
   ((unsigned int)((p)[3])      ))

#define BLAKE_U8TO64_BE(p) \
  (((unsigned long long)BLAKE_U8TO32_BE(p) << 32) | (unsigned long long)BLAKE_U8TO32_BE((p) + 4))

#define BLAKE_U32TO8_BE(p, v) \
  do { \
    (p)[0] = (BitSequence)((v) >> 24);  \
    (p)[1] = (BitSequence)((v) >> 16); \
    (p)[2] = (BitSequence)((v) >>  8); \
    (p)[3] = (BitSequence)((v)      ); \
  } while (0)

#define BLAKE_U64TO8_BE(p, v) \
  do { \
    BLAKE_U32TO8_BE((p),     (unsigned int)((v) >> 32));	\
    BLAKE_U32TO8_BE((p) + 4, (unsigned int)((v)      ));	\
  } while (0)

/*
  error codes
*/
typedef enum { SUCCESS=0, FAIL=1, BAD_HASHBITLEN=2  } HashReturn;

/* 
   hash structure
*/
typedef struct  { 
  int hashbitlen;  /* length of the hash value (bits) */
  int datalen;     /* amount of remaining data to hash (bits) */
  int init;        /* set to 1 when initialized */
  int nullt;       /* Boolean value for special case \ell_i=0 */
  /*
    variables for the 32-bit version  
  */
  unsigned int h32[8];         /* current chain value (initialized to the IV) */
  unsigned int t32[2];         /* number of bits hashed so far */
  BitSequence data32[64];     /* remaining data to hash (less than a block) */
  unsigned int salt32[4];      /* salt (null by default) */
  /*
    variables for the 64-bit version  
  */
  unsigned long long h64[8];      /* current chain value (initialized to the IV) */
  unsigned long long t64[2];      /* number of bits hashed so far */
  BitSequence data64[128];  /* remaining data to hash (less than a block) */
  unsigned long long salt64[4];   /* salt (null by default) */
} hashState;

private:
int blakeNumRounds32;
int blakeNumRounds64;
hashState blakeState;

public:
Blake( const int numRounds );
int Init( int hashbitlen );
int Update( const BitSequence * data, DataLength databitlen );
int Final( BitSequence * hashval );
int Hash( int hashbitlen, const BitSequence * data, DataLength databitlen, 
		 BitSequence * hashval );

private:
int AddSalt( const BitSequence * salt );
int compress32( const BitSequence * datablock );
int compress64( const BitSequence * datablock );
int Update32( const BitSequence * data, DataLength databitlen );
int Update64( const BitSequence * data, DataLength databitlen );
int Final32( BitSequence * hashval );
int Final64( BitSequence * hashval );

};

#endif