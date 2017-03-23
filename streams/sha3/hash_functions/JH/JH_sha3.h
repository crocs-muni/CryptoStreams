#ifndef JH_SHA3_H
#define JH_SHA3_H

#include <string.h>
#include "../../sha3_interface.h"

class JH : public sha3_interface {

#define JH_DEFAULT_NUM_ROUNDS	42

/*for LP64, "int" is 32-bit integer, while "long" is 64-bit integer*/
#if defined(_LP64)
      typedef unsigned int jh_uint32;
#else
      typedef unsigned long jh_uint32;
#endif

/*typedef unsigned char BitSequence;
typedef unsigned long long DataLength;*/
typedef enum {SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2} HashReturn;

/*define data alignment for different C compilers*/
#if defined(__GNUC__)
      #define JH_DATA_ALIGN16(x) x __attribute__ ((aligned(16)))
#else
      #define JH_DATA_ALIGN16(x) __declspec(align(16)) x
#endif

typedef struct {
	int hashbitlen;	   	              /*the message digest size*/
	unsigned long long datasize_in_buffer;           /*the size of the message remained in buffer; assumed to be multiple of 8bits except for the last partial block at the end of the message*/
	unsigned long long databitlen;    /*the message size in bits*/
	JH_DATA_ALIGN16(jh_uint32 x[8][4]);     /*the 1024-bit state, ( x[i][0] || x[i][1] || x[i][2] || x[i][3] ) is the ith row of the state in the pseudocode*/
	unsigned char buffer[64];         /*the 512-bit message block to be hashed;*/
} hashState;

private:
unsigned int jhNumRounds;
hashState jhState;

/*The API functions*/
public:
JH(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data,DataLength databitlen, BitSequence *hashval);

private:
void E8();   /*The bijective function E8, in bitslice form */
void F8();   /*The compression function F8 */

};

#endif