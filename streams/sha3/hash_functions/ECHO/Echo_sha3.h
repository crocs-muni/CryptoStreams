#ifndef ECHO_SHA3_H
#define ECHO_SHA3_H

#include <string.h>
#include "../../sha3_interface.h"

class Echo : public sha3_interface {

#define ECHO_NUM_ROUNDS_LOWER 8
#define ECHO_NUM_ROUNDS_HIGHER 10

typedef enum { 
  SUCCESS=0
 ,FAIL=1
 ,BAD_HASHBITLEN=2
 ,UPDATE_WBITS_TWICE=4
} HashReturn;

/*		These are the timers for c/b measurements				*/

typedef struct {
  /* the big state of the compression function						*/
  /* (chaining variable + message)							*/
  unsigned int state[4*16];
  /*  current data buffer								*/
  BitSequence data[1536 >> 3];
  /* the size of the output hash							*/
  unsigned int hashbitlen;
  /* the size of the message bit length							*/
  unsigned long long messbitlen;
  /* the current counter value								*/
  unsigned long long CNT;
  /* additional data									*/
  unsigned int m_blocksize;
  unsigned char end_cv;
  unsigned char padding;
  unsigned char r;
#ifdef ECHO_SALT_OPTION
  unsigned char ECHO_SALT[16];
#endif
} hashState;

private:
int echoNumRoundsLower;
int echoNumRoundsHigher;
hashState echoState;

/*		Set the SALT									*/
#ifdef ECHO_SALT_OPTION
int SetSalt(const BitSequence* ECHO_SALT);
#endif

public:
Echo(const int numRounds);
int Init(int hashbitlen);//WAS UNSIGNED INT HASHBITLEN BEFORE, HAD TO CHANGE CAUSE OF ABSTRACT CLASS
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int Compress();
unsigned char Endianess();
void PushString(unsigned long long num, unsigned char* string, unsigned char type);
unsigned char flip_bits(unsigned char c);


/*		AES Tables taken from OpenSSL					*/
/*
T0[x] = S [x].[02, 01, 01, 03];
T1[x] = S [x].[03, 02, 01, 01];
T2[x] = S [x].[01, 03, 02, 01];
T3[x] = S [x].[01, 01, 03, 02];
T4[x] = S [x].[01, 01, 01, 01];
*/

};

#endif