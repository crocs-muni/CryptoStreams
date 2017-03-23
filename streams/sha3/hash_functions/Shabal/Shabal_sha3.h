#ifndef SHABAL_SHA3_H
#define SHABAL_SHA3_H

#include <limits.h>
#include "../../sha3_interface.h"

#if defined __STDC__ && __STDC_VERSION__ >= 199901L
#include <stdint.h>
#ifdef UINT32_MAX
typedef uint32_t shabal_word32;
#else
typedef uint_fast32_t shabal_word32;
#endif
#else
#if ((UINT_MAX >> 11) >> 11) >= 0x3FF
typedef unsigned int shabal_word32;
#else
typedef unsigned long shabal_word32;
#endif
#endif

class Shabal : public sha3_interface {

#define SHABAL_BLOCK_SIZE   16
#ifndef SHABAL_PARAM_R
#define SHABAL_PARAM_R      12
#endif

/*
 * Input messages and hash outputs are sequences of bits, stored
 * in arrays of C bytes.
 */

/*typedef unsigned char BitSequence;

#ifdef ULLONG_MAX
typedef unsigned long long DataLength;
#else
typedef unsigned long DataLength;
#endif*/

/*
 * Status codes.
 */
typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	BAD_HASHBITLEN = 2
} HashReturn;

/*
 * State structure for Shabal. A structure instance holds the running
 * state for a hash computation. The function implementation is
 * thread-safe and reentrant, as long as the different has computations
 * use distinct instances of this structure. State structures are
 * meant to be allocated by the caller; allocation can be performed
 * in any suitable memory area, including local variables, global
 * variables, and the heap. The structure contains no pointer value
 * and thus can be moved, duplicated (with memcpy()) or serialized
 * at will. In particular, making a copy of a structure instance takes
 * a "snapshot" of the hash computation at that point, which can be
 * resumed later on with alternate subsequence input data.
 *
 * The "hashbitlen" field contains the intended output length, in bits.
 * It is initialized in the Init() function. The other fields are private
 * and should not be accessed externally.
 */
typedef struct {
	BitSequence buffer[SHABAL_BLOCK_SIZE * 4];
	size_t buffer_ptr;
	unsigned last_byte_significant_bits;
	int hashbitlen;
	shabal_word32 A[SHABAL_PARAM_R];
	shabal_word32 B[SHABAL_BLOCK_SIZE];
	shabal_word32 C[SHABAL_BLOCK_SIZE];
	shabal_word32 Whigh, Wlow;
} hashState;

private:
hashState shabalState;

public:
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data,
	DataLength databitlen, BitSequence *hashval);

};

#endif