#ifndef BLENDER_SHA3_H
#define BLENDER_SHA3_H

#include "../../sha3_interface.h"

class Blender : public sha3_interface {

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;		// needs 64-bit variables to operate properly

#ifndef NO_LONG_LONG
//#define VERBOSE		// define to get state output per round
#endif					// needs 64-bit variables to operate properly

// maximum number of words in message digest
#define BLENDER_MAXWORDS 10
// maximum number of bytes of initial data to be saved for filling
#define BLENDER_FBSIZE 13

#define BLENDER_DEFAULT_ROUNDS 32

#ifndef Uint64
#ifndef NO_LONG_LONG
#define Uint64 unsigned long long
#else
typedef struct {		// equivalent structure on 8-bit systems
	unsigned long llw;	// low order longword
	unsigned long hlw;	// high order longword
} Uint64;
#endif
#endif

#ifndef Uint32
#define Uint32 unsigned long
#endif
#ifndef Uint16
#define Uint16 unsigned short
#endif
#ifndef Uint8
#define Uint8 unsigned char
#endif

typedef union {		// various forms of checksums, words and sums
	Uint64 ui64;
	Uint32 ui32;
	Uint16 ui16;
	Uint8 b[8];
} Mint;

typedef struct {
	// add all the static storage your algorithm needs here
	DataLength sourceDataLength;	// low order part of data length
								// put first to force memory alignment
	Uint8 member;				// algorithm variant (encoded digest size)
	Uint8 wordSizeBytes;		// word size in bytes
	Uint8 fragSizeBytes;		// fragment size in bytes
	Uint8 wordCount;			// number of words in digest
	Uint8 blockSizeFrags;		// block size in fragments
	Uint8 minimumRounds;		// minimum number of rounds to be done
	Uint8 roundsToGo;			// rounds to go in the current block
	Uint8 bitEndianity;			// endianity of bits in data bytes
								// 0=little endian, 1=big endian (default)

	Uint8 messageResidue [8];	// unprocessed bytes of data
	//Uint8 firstBlock [16*8];	// first block memory - maximum size possible
	Uint8 firstBlock [BLENDER_FBSIZE];	// first block memory - reduced size (13 bytes)
	Uint8 carries;				// algorithm carry bits (packed)
	Uint8 firstByteCount;		// number of bytes held in firstBlock
	Uint8 residueLength;		// number of bytes held in messageResidue

	Mint Checksum1, Checksum2;	// workspace for checksums
	Mint Word [BLENDER_MAXWORDS];		// state variables
	Mint Sum [BLENDER_MAXWORDS];		// final accumulators

	DataLength sourceDataLength2[3];	// high order parts of data length
		// note: the array size determines the maximum length supported
	//Uint16 hashBitLen;		// size of the message digest in bits - not used
} hashState;

private:
int blenderNumRounds;
hashState blenderState;
hashState ss;		// local workspace

public:
Blender(const int numRounds);
int Init (int hashbitlen);
int Update (const BitSequence *dta,
					DataLength databitlen);
int Final (BitSequence *hashval);
int Hash (int hashbitlen, const BitSequence *dta,
					DataLength databitlen, BitSequence *hashval);

private:
void doHash32x2 (const BitSequence *dta, Uint32 byteCount);
void doHash16x2 (const BitSequence *dta, Uint32 byteCount);
void doHash64x2 (const BitSequence *dta, Uint32 byteCount);
void doHash32x3 (const BitSequence *dta, Uint32 byteCount);
void doHash16x3 (const BitSequence *dta, Uint32 byteCount);
void doHash64x3 (const BitSequence *dta, Uint32 byteCount);
void doHashGen (const BitSequence *dta, Uint32 byteCount);
void doUpdate (const BitSequence *dta, Uint32 byteCount);
void doFinal32 (BitSequence *hashval);
void doFinal16 (BitSequence *hashval);
void doFinal64 (BitSequence *hashval);
void doFinalGen (BitSequence *hashval);

};

#endif