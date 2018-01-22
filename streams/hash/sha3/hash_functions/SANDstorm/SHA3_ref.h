#ifndef SANDSTORM_SHA3_REF_H
#define SANDSTORM_SHA3_REF_H

/* Type definitions */
typedef unsigned char BitSequence;
typedef unsigned long long DataLength; //typical 64 bit value
//typedef unsigned long long ULL;
//typedef unsigned int UI;

/* 512 bit hash Type defintions */
//typedef unsigned int u32;
//typedef unsigned long long u64;
typedef struct { unsigned long long hi; unsigned long long lo;} sand_u128;

typedef enum { 
	SAND_SUCCESS = 0, 
	SAND_FAIL = 1, 
	SAND_BAD_HASHBITLEN = 2 
} SandHashReturn;

typedef struct {
	/********* 256 bit hash vars *************/
	DataLength blockIters[3];			// iterators to manage which level we are at in the algorithm
	int hashbitlen;						// length of the resultant hash
	int pipedBits;						// current number of bits in the pipe to be processed
	int initCompressFlag;				// lets program know if we have accomplished the first compress
	BitSequence queuedData[64];			// data that are left over from last call to update
	DataLength prevBlock[3][5][4];		// data to feed into the next compression round

	/********* 512 bit hash vars *************/
	unsigned char msgbytes[128];  // data bytes of the message
	int mbcnt;                    // number of message bytes in the buffer
	sand_u128 msg[8];                  // message repacked into 8 words of 128 bits
	sand_u128 msglen;                  // message length in bits
	sand_u128 blkn1, blkn2;            // superblock numbers for level 1 and 2
	int lvl0flag;                 // level 0 has been processed;
	int lvl1cnt, lvl2cnt;         // number of blocks in current level 1 & 2 superblocks
	int lvl3flag;                 // level 3 has been used
	sand_u128 prev[5*5*4];             // result of hash round function at each level
								  // 5 levels, 5 rounds, 4 128-bit words
	int outsize;                  // output size in bits; must be 384 or 512.
	sand_u128 *lvlc;                   // pointer to level starting constants
} sandHashState;



/* Code control flags: 0 means off, 1 means on */
#define SAND_INTERMEDIATE_VALUES 0	// Print intermediate values
#define SAND_ASM_CODE 1				// In-line assembly code

#define	SAND_MAXBITS		512		/* Maximum number of bits for one run of the compression function */
#define	SAND_MAXBYTES	64		/* Maximum number of bytes for one run of the compression function */
#define	SAND_BYTELENGTH	8
#define SAND_L1SBLENGTH	10
#define SAND_L2SBLENGTH	100

/* Value to add extra security to the algorithm: Should be a value between 0 and 20. */
/*  Default is 0 */
//#define SAND_TUNABLE_SECURITY_PARAMETER	0
#define SAND_DEFAULT_ROUNDS	5

/* A and SAND_B constansts used in G(x) function */
#define SAND_ACONST_256 0xA611186BUL
#define SAND_BCONST_256 0xBEE8390DUL
#define SAND_ACONST_512 0xa611186bae67496bull
#define SAND_BCONST_512 0xbee8390d43955aedull

/* C and D constants for initializing levels 2 and 3 */
#define SAND_CCONST_256 0x6135F68D4C0CBB6FULL
#define SAND_DCONST_256 0x79CC45195CF5B7A4ULL

/* masks used in the bit_mix function */
#define SAND_J3 0x3333333333333333ULL
#define SAND_J5 0x5555555555555555ULL
#define SAND_J6 0x6666666666666666ULL
const sand_u128 SAND_J3_128 = {0x3333333333333333ULL, 0x3333333333333333ULL};
const sand_u128 SAND_J5_128 = {0x5555555555555555ULL, 0x5555555555555555ULL};
const sand_u128 SAND_J6_128 = {0x6666666666666666ULL, 0x6666666666666666ULL};

/* mask used to select low 32 bits of a 64-bit variable, sand_b32 = 2^32 */
#define sand_msk32 0xFFFFFFFFul
#define sand_b32 0x100000000ull

/* Number of bits to rotate in message schedule and in round functions */
#define SAND_MS_ROT_BITS		27	
#define SAND_BITMIX_ROT_BITS	19	
#define SAND_R_ROT_BITS		25

/* 32-bit byte swap macro */
#define SAND_ROT_RIGHT32(x,n) (((x) >> n) | ((x) << (32 - n)))
#define SAND_bswap_32(x)		((SAND_ROT_RIGHT32((x), 24) & 0x00ff00ff) | (SAND_ROT_RIGHT32((x), 8) & 0xff00ff00))

//typedef unsigned long long unsigned long long;
//typedef unsigned int unsigned int;

#if SAND_INTERMEDIATE_VALUES == 1
	unsigned long long SAND_GLOBAL_512_L1_BNUM;
	unsigned long long SAND_GLOBAL_512_L2_BNUM;
	unsigned long long SAND_GLOBAL_512_L3_BNUM;
	int sand_print_flag = 0;
#endif

static const BitSequence SAND_WORDMASK = 0xFF;
static const BitSequence SAND_ORMASK = 0x01; 

#endif