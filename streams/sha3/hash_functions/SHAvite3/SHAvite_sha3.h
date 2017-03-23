#ifndef SHAVITE_SHA3_H
#define SHAVITE_SHA3_H

#include "../../sha3_interface.h"

class SHAvite : public sha3_interface {

#define SHAVITE_NUM_ROUNDS_256	12
#define SHAVITE_NUM_ROUNDS_512	14

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum {SUCCESS=0, FAIL=1, BAD_HASHBITLEN=2} HashReturn;

/* SHAvite-3 definition */

typedef struct {
   DataLength bitcount;            /* The number of bits compressed so far   */
   BitSequence chaining_value[64]; /* An array containing the chaining value */
   BitSequence buffer[128];        /* A buffer storing bytes until they are  */
				   /* compressed			     */
   BitSequence partial_byte;       /* A byte to store a fraction of a byte   */
				   /* in case the input is not fully byte    */
				   /* alligned				     */
   BitSequence salt[64];           /* The salt used in the hash function     */ 
   int DigestSize;		   /* The requested digest size              */
   int BlockSize;		   /* The message block size                 */
} hashState;

private:
int shaViteNumRounds256;
int shaViteNumRounds512;
hashState shaviteState;

public:
SHAvite(const int numRounds);
/* Function calls imposed by the API */
int Init (int hashbitlen);
int Update (const BitSequence *data, DataLength databitlen);
int Final (BitSequence *hashval);
int Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif