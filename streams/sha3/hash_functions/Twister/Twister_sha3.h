#ifndef TWISTER_SHA3_H
#define TWISTER_SHA3_H

#ifdef __GNUC__
#include<stdint.h>
#endif
#include "../../sha3_interface.h"

#ifndef __GNUC__
typedef unsigned __int64  uint64_t;
typedef unsigned __int8 uint8_t;
#endif


class Twister : public sha3_interface {

#define TWISTER_DEFAULT_MINI_ROUNDS_256	9
#define TWISTER_DEFAULT_MINI_ROUNDS_384	10

#define TWISTER_MIN(a,b) ((a)>(b)?(b):(a))

#define TWISTER_ROWS 8
#define TWISTER_MESSAGE_BYTELEN 64

/****************************************************************************/
/************************** Type Definitions ********************************/
/****************************************************************************/


/* Data to be hased and the resulting hash value */
//typedef unsigned char BitSequence;

/* Data length of the message to be hashed */
//typedef unsigned long long DataLength;

/* Return value for the API Hash function */
typedef enum { SUCCESS = 0, FAIL=1, BAD_HASHBITLEN=2 } HashReturn;

/* Twister specific state */

typedef struct {
  /* state of the transposed matrix (s0 = 1st column, s1 = 2nd colummn...) */ 
  uint64_t hs_s[TWISTER_ROWS];

  /* checksum  */
  uint64_t hs_checksum;
  

  /* transposed checksum matrix 256-512 bit version   */
  uint64_t hs_ck0;
  uint64_t hs_ck1;
  uint64_t hs_ck2;
  uint64_t hs_ck3;
  uint64_t hs_ck4;
  uint64_t hs_ck5;
  uint64_t hs_ck6;
  uint64_t hs_ck7;

  /* Processed message bit */
  DataLength hs_cmesglen;

  /* Unprossesed message block */
  BitSequence hs_data[64];    
  unsigned long int hs_databitlen;

  /* Twist counter, prevents slide attacks */
  uint64_t hs_counter;

  /* hashbitlength in bits */
  unsigned long int hs_hashbitlen;
  
  /* Current state and temp state */
} hashState;

private:
int twisterNumRounds256;
int twisterNumRounds384;
hashState twisterState;


/****************************************************************************/
/****************************** NIST API ************************************/
/****************************************************************************/

public:
Twister(const int numRounds);
/* Initiate Twister */
int Init(int hashbitlen);

/* Process the supplied data */
int Update(const BitSequence *data, 
		  DataLength databitlen);

/* Perform any post processing and output filtering */
int Final(BitSequence *hashval);

/* Hash the supplied data and provie the resulting hash value */
int Hash(int hashbitlen, const BitSequence *data,
		DataLength databitlen, BitSequence *hashval); 




/****************************************************************************/
/***************************** TWISTER AP ***********************************/
/****************************************************************************/
private:
void twister_twist(uint64_t *data);
void twister_print_state();
  
void twister_mini_round();
void twister_output(uint64_t *hashval);

};

#endif