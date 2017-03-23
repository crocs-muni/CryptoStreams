#ifndef DSHA_SHA3_H
#define DSHA_SHA3_H

#include <stdlib.h>
#include "../../sha3_interface.h"

class DSHA : public sha3_interface {

#define DSHA_DEFAULT_ROUNDS 16 /* from 0 to 15 */

#define DSHA_ROTR64(x,n)   (((x) >> (n)) | ((x) << (64 - (n))))
#define DSHA_ROTR32(x,n)   (((x) >> (n)) | ((x) << (32 - (n))))

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
typedef struct
{
    DataLength databitlen[2];
    unsigned int hashbitlen;
    unsigned int blocksize;
    BitSequence block[129];
	unsigned long hashval[16];
} hashState;

private:
int dshaNumRounds;
hashState dshaState;

public:
DSHA(const int numRounds);
int Init(int hashbitlen);   
int Update(const BitSequence *data, DataLength databitlen); 
int Final(BitSequence *hashval); 
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int sha32_compile();
int sha64_compile();

};

#endif