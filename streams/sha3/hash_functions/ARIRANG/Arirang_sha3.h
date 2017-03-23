#ifndef ARIRANG_SHA3_H
#define ARIRANG_SHA3_H

extern "C" {
#include "Arirang_OP32.h"
}
#include "../../sha3_interface.h"

class Arirang : public sha3_interface {

typedef enum {SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2} HashReturn;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Definition

#define ARIRANG_DEFAULT_ROUNDS 4

/*#define ARIRANG224_BLOCK_LEN	64
#define ARIRANG224_DIGEST_LEN	28

#define ARIRANG256_BLOCK_LEN	64
#define ARIRANG256_DIGEST_LEN	32

#define ARIRANG384_BLOCK_LEN	128
#define ARIRANG384_DIGEST_LEN	48

#define ARIRANG512_BLOCK_LEN	128
#define ARIRANG512_DIGEST_LEN	64*/


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Datatype

//typedef unsigned char BYTE;				//  8-bit data type

//typedef unsigned char BitSequence;		//  8-bit data type

//typedef unsigned short int WORD;		// 16-bit data type

//typedef unsigned int DWORD;				// 32-bit data type

//typedef unsigned long DataLength;	    // 32-bit data type


private:
int arirangNumRounds;
arirangHashState arirangState;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function

public:
Arirang(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(unsigned char *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif
