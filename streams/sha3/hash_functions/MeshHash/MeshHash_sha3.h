#ifndef MESHHASH_SHA3_H
#define MESHHASH_SHA3_H

#include "../../sha3_interface.h"

class MeshHash : public sha3_interface {

#define MESHHASH_DEFAULT_ROUNDS	256 /* real number of rounds is 256/8 */

#define MESHHASH_MIN_NUMBER_OF_PIPES 4
/* should be >= 4 */
#define MESHHASH_MAX_NUMBER_OF_PIPES 256
/* must be <= 256 */
#define MESHHASH_NUMBER_OF_EXTRA_PIPES 1
/* for extra security, can be even negative, but should not be to large, should only be changed if flaws in design are detected,
 * can also be an expression depending on number_of_pipes */

#define MESHHASH_COUNTER_LENGTH 4
/* must be of type int, positive, and <= MESHHASH_MIN_NUMBER_OF_PIPES */

typedef unsigned char BitSequence;
typedef unsigned long long DataLength; /* should be a typical 64-bit value (long long) */

typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	BAD_HASHBITLEN = 2,
	BAD_KEY_LENGTH = 3,
	MEMORY_ALLOCATION_FAILED = 4
} HashReturn;

typedef struct {
     int hashbitlen,		/* length of hash value in bits */
	 number_of_pipes,	
	 key_length,		/* length in words */
	 block_round_counter,	/* actual round in block */
	 key_counter,		/* actual offset in key */
	 data_counter,		/* counter for buffering data */
	 squeezing;		/* indicating if the whole message is processed and the computing of the hash value has started */
     unsigned long long *pipe,
	  *key,
	  data_buffer,				/* buffer if message is not given whole words */
	  bit_counter[MESHHASH_COUNTER_LENGTH],		/* counter for message bits */
	  block_counter[MESHHASH_COUNTER_LENGTH];	/* counter for processed blocks */
     } hashState;

private:
int meshHashNumRounds;
hashState meshhashState;

public:
MeshHash(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int Init3(int hashbitlen, int number_of_pipes, int keybytelen, unsigned char *key);
int Init2(int hashbitlen, int keybytelen, unsigned char *key);
int InitSponge2(int number_of_pipes, int keybytelen, unsigned char *key);
int InitSponge(int number_of_pipes);
int SqueezeNBytes(BitSequence *hashval, int rounds);
void Deinit();
int Hash2(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval, int keybytelen, unsigned char *key);
int ComputeNumberOfPipes(int hashbitlen);
static unsigned long long meshhash_sbox(unsigned long long w);
static void meshhash_add_to_counter(unsigned long long counter[], DataLength to_add);
static void normal_round(hashState *state, unsigned long long data);
static void final_block_round(hashState *state);
static void final_rounds(hashState *state);

};

#endif