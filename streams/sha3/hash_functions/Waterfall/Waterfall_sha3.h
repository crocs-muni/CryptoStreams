#ifndef WATERFALL_SHA3_H
#define WATERFALL_SHA3_H

#include "../../sha3_interface.h"

class Waterfall : public sha3_interface {

/*typedef unsigned char BitSequence;		// Data type of input data
typedef unsigned long long DataLength;*/	// Data type for input data length in bits
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;	// Returns from functions

#define WATERFALL_STREAM1 16					// Stream lengths (32-bit words)
#define WATERFALL_STREAM2 7
#define WATERFALL_STREAM3 6
#define WATERFALL_POOLFACTOR 2				// Length of each Pool as a multiple of Stream 1
#define WATERFALL_POOL (WATERFALL_STREAM1*WATERFALL_POOLFACTOR)	// Pool lengths (32-bit words)

// Structure to hold Waterfall Hash state data
typedef struct {
	unsigned int hashbitlen;		// Requested number of bits in message digest

	unsigned int inputbits;			// Number of bits currently in input buffer
	unsigned char inputbuffer[64];	// Input buffer of 512 bits
	unsigned int inputblocks;		// Number of blocks of input (512 bits) currently processed
	unsigned int inputslabs;		// Number of slabs of input (2^41 bits) currently processed

	unsigned int *pstream1;			// Pointer into Stream 1
	unsigned int *pstream2;			// Pointer into Stream 2
	unsigned int *pstream3;			// Pointer into Stream 3
	unsigned int *ppool2;			// Pointer into Pool 2
	unsigned int *ppool3;			// Pointer into Pool 3

	unsigned int stream1[WATERFALL_STREAM1];	// Stream 1
	unsigned int stream2[WATERFALL_STREAM2];	// Stream 2
	unsigned int stream3[WATERFALL_STREAM3];	// Stream 3
	unsigned int pool2[WATERFALL_POOL];		// Pool 2 - NB there is no Pool 1
	unsigned int pool3[WATERFALL_POOL];		// Pool 3
} hashState;

private:
unsigned int waterfallFinalUpdates;
unsigned int waterfallFinalSteps;
hashState waterfallState;

public:
Waterfall(const int NumRounds);
// Function prototypes

// EACIRC: manual edit: next 5 lines added
#ifdef __GNUC__
#ifndef __MINGW32__
#define __forceinline __attribute__((always_inline))
#endif
#endif

__forceinline int Init(int hashbitlen);									// Initialisation
__forceinline int Update(const BitSequence *data, DataLength databitlen);	// Update state data with input
__forceinline int Final(BitSequence *hashval);								// Return message digest
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen,
				BitSequence *hashval);													// Do it all at once

private:
void Xbox ( unsigned int *pword );
void UpdateStep ( unsigned int input );
void FinalStep ( unsigned int *pworkin, unsigned int *ppool, unsigned int *pworkout );

};

#endif
