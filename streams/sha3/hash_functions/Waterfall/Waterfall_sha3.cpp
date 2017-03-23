#include <stddef.h>		// For NULL
#include "Waterfall_sha3.h"

#define WATERFALL_FINALUPDATES 16	// Number of Update Steps in Final with fixed input
#define WATERFALL_FINALSTEPS 4	// Number of Final Steps in Final after combining with Pools

#define WATERFALL_LITTLEENDIAN 1	// If we can assume little-endian byte order - yes for Intel
#define WATERFALL_UNROLL 1		// If we do best with loops unrolled - yes
#define WATERFALL_DEBUG 0			// 0: no output, 1: API calls, 2: input, main step results, 3: full processing breakdown
#if WATERFALL_DEBUG>0
#include <stdio.h>
#endif

// -------------------------------------------------------------------------------------------------

// API routine to initialise hash algorithm state and check required message digest length

int Waterfall::Init ( int hashbitlen ) {
	int i;

// Check message digest length is valid (whole 32-bit words, at most 16 of them)
	if (hashbitlen<=0 || hashbitlen>32*WATERFALL_STREAM1 || hashbitlen%32) return BAD_HASHBITLEN;
	waterfallState.hashbitlen = hashbitlen;

// Initialise input buffer to empty
	waterfallState.inputbits = 0;
	waterfallState.inputblocks = 0;
	waterfallState.inputslabs = 0;

// Put the digest length into the first word of Stream 2 (Stream 1 may be used for a key)
	waterfallState.stream2[0] = hashbitlen;

// Initialise remaining Stream entries to zeros
	for (i=0; i<WATERFALL_STREAM1; i++) waterfallState.stream1[i] = 0;
	waterfallState.pstream1 = waterfallState.stream1 +  WATERFALL_STREAM1 - 1;
	for (i=1; i<WATERFALL_STREAM2; i++) waterfallState.stream2[i] = 0;
	waterfallState.pstream2 = waterfallState.stream2 +  WATERFALL_STREAM2 - 1;
	for (i=0; i<WATERFALL_STREAM3; i++) waterfallState.stream3[i] = 0;
	waterfallState.pstream3 = waterfallState.stream3 +  WATERFALL_STREAM3 - 1;

// Initialise Pools to zeros
	for (i=0; i<WATERFALL_POOL; i++) waterfallState.pool2[i] = 0;
	waterfallState.ppool2 = waterfallState.pool2 + WATERFALL_POOL-1;
	for (i=0; i<WATERFALL_POOL; i++) waterfallState.pool3[i] = 0;
	waterfallState.ppool3 = waterfallState.pool3 + WATERFALL_POOL-1;

#if WATERFALL_DEBUG>0
	printf("Waterfall initialised: hashbitlen %d\n", hashbitlen);
#endif
	return SUCCESS;
}

// -------------------------------------------------------------------------------------------------

// Perform the X-box substitution on a 32-bit word

void Waterfall::Xbox ( unsigned int *pword ) {

// X-box substitution data, based on Rijndael S-box and 3 affine transforms
	static const unsigned int waterfall_xbox[] = {
		0xbb9a4063, 0x2b668d7d, 0xa50c3275, 0xaf811c78, 0xe638eff6, 0xb793746e, 0xb1176e69, 0x4be946c2,
		0x43a30338, 0xebb4bd08, 0xbd1e5a6d, 0xd5dbd420, 0xecb5c1f2, 0x50b923da, 0x114a95a5, 0x242db479,
		0xc307e3da, 0xad467793, 0x416468db, 0xaa470b6e, 0xea31dbee, 0x9de7414c, 0x8c3a0a51, 0xe57ae2e7,
		0x148c82b5, 0xd2daa8cd, 0x9c6227b8, 0x17ce8fb4, 0xbc9b3c80, 0x99a430b9, 0x22a9ae6c, 0xcc4cdadf,
		0x03d5d397, 0x6ed64adc, 0x347599b1, 0x5e777c05, 0x46651412, 0xcb4da61a, 0x619d73d1, 0xc6c1f4eb,
		0x4527191c, 0x1885b68c, 0x7acd16cf, 0x645b64da, 0xa0ca255d, 0xd85786f5, 0xc282851f, 0xf522cf3a,
		0x6c112134, 0x48ab4bf6, 0xd9d2e011, 0x4e2f51f0, 0x7e8e672c, 0xb3d005a3, 0xed30a733, 0xb95d2bad,
		0xee72aa3f, 0x71c55e2b, 0xae047aba, 0xfe2a87d9, 0x730235d7, 0xdf56fa1a, 0x84704f8c, 0xa64e3f4a,
		0xe7bd8949, 0x2c67f1c2, 0x513c456e, 0x7dcc6a59, 0xfcedec5f, 0x3036e82b, 0x1f84ca1c, 0x9f202ae7,
		0x138dfe1a, 0xcdc9bc72, 0xd198a59c, 0x0551c9f8, 0xd699d965, 0x7f0b01ae, 0xd35fce61, 0xa88060cb,
		0x92ac7803, 0x557f3480, 0x6a953b52, 0x76c422be, 0x5bb16b74, 0xeff7cca9, 0x0613c4e7, 0x9ea54c0c,
		0x36b2f232, 0x42266592, 0x8efd61e4, 0xce8bb162, 0x0796a216, 0x0250b511, 0x1cc6c706, 0x44a27f90,
		0xd45eb2b0, 0x75862f8e, 0x906b13c8, 0x6b105d98, 0x8abe1027, 0x83713328, 0xc1c08855, 0x29a1e6e2,
		0x8f78072d, 0x68525090, 0x69d73668, 0xa9050614, 0x10cff33c, 0x492e2d51, 0x3ef8b7f1, 0x93291ec7,
		0x91ee7521, 0x1d43a1d2, 0x08dd9b32, 0x26eadffc, 0xb5541fe6, 0x3dbabae8, 0x4faa374e, 0x62df7e82,
		0x8dbf6cc4, 0x82f455cf, 0xdb158ba0, 0xda90ed5a, 0x7287536c, 0x6d944782, 0x6719698d, 0xd71cbfad,
		0x47e0724d, 0x6018158d, 0xf0e4d891, 0xf7e5a46f, 0x982156db, 0x32f18312, 0x0e5981c2, 0xf660c290,
		0xcac8c04c, 0x1bc7bb2e, 0x282480f4, 0xc80fabb6, 0x3f7dd1e8, 0x9b635bd0, 0xffafe197, 0xa38828fc,
		0x39f9cbf0, 0x2f25fc10, 0x80333edd, 0xded39c4f, 0x58f366b6, 0x54fa52bf, 0xb6161206, 0xa20d4e1f,
		0x0d1b8cde, 0xf4a7a977, 0x8b3b7622, 0x7403498f, 0xdd919142, 0x1900d0c3, 0xe4ff8495, 0x5a340d44,
		0xfd688a40, 0x40e10e93, 0x4ce83a98, 0x65de02a9, 0x85f529ed, 0x6f532ca3, 0x5d357182, 0x1a42ddfb,
		0xcf0ed76a, 0x563d397a, 0x95ad0406, 0x3abbc6c9, 0x3737943d, 0x31b38e38, 0xfbec904a, 0xacc311d6,
		0x798f1b57, 0xc045ee79, 0xc7449285, 0xb25563de, 0x25a8d239, 0x53fb2e60, 0x0112b8f8, 0x1208981e,
		0x3374e5d4, 0x1509e4ef, 0xe3fef84e, 0xf223b351, 0xbe5c57d9, 0x2ea09ac7, 0x96ef0910, 0x669c0fb7,
		0x88797b7a, 0x2de297b9, 0xdc14f7e7, 0x527e48ed, 0x780a7dd8, 0x9ae63d63, 0x81b65872, 0xc98acd01,
		0xf161be20, 0x5ff21a14, 0x276fb9be, 0xfa69f6d4, 0x86b72487, 0x0c9eea70, 0x206ec545, 0xa14f4345,
		0x21eba3a0, 0x4a6c20ef, 0x0097de67, 0x3c3fdcb5, 0x04d4af9c, 0xe8f6b0d6, 0xe0bcf520, 0x635a18d9,
		0xb8d84db9, 0xc4069fec, 0x9428628d, 0x0a1af062, 0xabc26d5a, 0x4d6d5c1c, 0xf92bfbc3, 0xbfd93141,
		0x7c490c01, 0xe973d619, 0xba1f267a, 0xf3a6d5f2, 0xb4d1798d, 0x5976003c, 0xa7cb5968, 0xb0920873,
		0x387cad73, 0x7b4870f7, 0x2ae3eb6d, 0x70403802, 0xc583f922, 0x976a6fb8, 0x57b85fc6, 0x5cb01730,
		0xa489547c, 0x1e01ac50, 0x232cc87b, 0xe13993fe, 0x0fdce74b, 0xf8ae9d13, 0x0b9f96b4, 0x35f0ff9f,
		0x89fc1db9, 0x3b3ea060, 0xd01dc3d7, 0xe27b9ef4, 0x8732424c, 0x164be9a9, 0x0958fd45, 0x774144e9
	};

#if WATERFALL_DEBUG>2
	printf("  X-box steps: %8.8x ", *pword);
#endif
	*pword ^= waterfall_xbox[*pword & 0xFF];
	*pword = (*pword<<24) | (*pword>>8);
#if DEBUG>2
	printf("%8.8x ", *pword);
#endif
	*pword ^= waterfall_xbox[*pword & 0xFF];
	*pword = (*pword<<24) | (*pword>>8);
#if WATERFALL_DEBUG>2
	printf("%8.8x ", *pword);
#endif
	*pword ^= waterfall_xbox[*pword & 0xFF];
	*pword = (*pword<<24) | (*pword>>8);
#if WATERFALL_DEBUG>2
	printf("%8.8x ", *pword);
#endif
	*pword ^= waterfall_xbox[*pword & 0xFF];
	*pword = (*pword<<24) | (*pword>>8);
#if WATERFALL_DEBUG>2
	printf("%8.8x\n", *pword);
#endif
}

// -------------------------------------------------------------------------------------------------

// Perform a single update step given an input word

// Debug statements to support inline version
#if WATERFALL_DEBUG>2
#define WATERFALL_UPDATEDEBUG1 printf(" Update step input %8.8x\n", input);
#define WATERFALL_UPDATEDEBUG2																								\
	printf("  Current stream positions %d %d %d\n",																	\
		waterfallState.pstream1 - waterfallState.stream1, waterfallState.pstream2 - waterfallState.stream2, waterfallState.pstream3 - waterfallState.stream3);		\
	printf("  Previous stream values %8.8x %8.8x %8.8x\n", word1, word2, word3);									\
	printf("  Current stream values  %8.8x %8.8x %8.8x\n", *waterfallState.pstream1, *waterfallState.pstream2, *waterfallState.pstream3);
#define WATERFALL_UPDATEDEBUG3 printf("  New stream values      %8.8x %8.8x %8.8x\n", word1, word2, word3);
#define WATERFALL_UPDATEDEBUG4 printf("  New pool values at %d: %8.8x %8.8x\n", waterfallState.ppool2 - waterfallState.pool2, *waterfallState.ppool2, *waterfallState.ppool3);
#elif WATERFALL_DEBUG>1
#define WATERFALL_UPDATEDEBUG1 printf(" Update step input %8.8x\n", input);
#define WATERFALL_UPDATEDEBUG2
#define WATERFALL_UPDATEDEBUG3
#define WATERFALL_UPDATEDEBUG4
#else // no debug
#define WATERFALL_UPDATEDEBUG1
#define WATERFALL_UPDATEDEBUG2
#define WATERFALL_UPDATEDEBUG3
#define WATERFALL_UPDATEDEBUG4
#endif

// Extract current stream values if not already there - depends on unrolling
#define WATERFALL_STREAMWORDS																	\
	word1 = *waterfallState.pstream1;															\
	word2 = *waterfallState.pstream2;															\
	word3 = *waterfallState.pstream3;

// Macro version of UpdateStep for inlining
#define WATERFALL_UPDATESTEP																		\
	WATERFALL_UPDATEDEBUG1																		\
	if (++waterfallState.pstream1 == waterfallState.stream1+WATERFALL_STREAM1) waterfallState.pstream1 = waterfallState.stream1;	\
	if (++waterfallState.pstream2 == waterfallState.stream2+WATERFALL_STREAM2) waterfallState.pstream2 = waterfallState.stream2;	\
	if (++waterfallState.pstream3 == waterfallState.stream3+WATERFALL_STREAM3) waterfallState.pstream3 = waterfallState.stream3;	\
	WATERFALL_UPDATEDEBUG2																		\
	word1 ^= *waterfallState.pstream1 ^ input; 													\
	Waterfall::Xbox(&word1);																		\
	*waterfallState.pstream1 = word1;															\
	word2 ^= *waterfallState.pstream2 ^ input ^ 1; 												\
	Waterfall::Xbox(&word2);																		\
	*waterfallState.pstream2 = word2;															\
	word3 ^= *waterfallState.pstream3 ^ input ^ 2; 												\
	Waterfall::Xbox(&word3);																		\
	*waterfallState.pstream3 = word3;															\
	WATERFALL_UPDATEDEBUG3																		\
	waterfallState.ppool2++;																	\
	if(waterfallState.ppool2==waterfallState.pool2+WATERFALL_POOL) {												\
		waterfallState.ppool2 = waterfallState.pool2;													\
		waterfallState.ppool3 = waterfallState.pool3;													\
	} else {																			\
		waterfallState.ppool3++;																\
	}																					\
	*waterfallState.ppool2 ^= word2;															\
	*waterfallState.ppool3 ^= word3;															\
	WATERFALL_UPDATEDEBUG4

// For Final
void Waterfall::UpdateStep ( unsigned int input ) {
	unsigned int word1, word2, word3;
	//WATERFALL_STREAMWORDS
	WATERFALL_STREAMWORDS
	WATERFALL_UPDATESTEP
}

// Get one word out of the buffer or input data, taking care of big-endian data
#if WATERFALL_LITTLEENDIAN
#define WATERFALL_NEXTINPUT input = *pinput++;
#else
#define WATERFALL_NEXTINPUT													\
	input = *pinput++;												\
	input = (input>>16) | (input<<16);								\
	input = ((input & 0xFF00FF00)>>8) | ((input & 0x00FF00FF)<<8);
#endif

// Process one block of input data (16 words) plus the block count, depending on loop unrolling
#if WATERFALL_UNROLL
#define WATERFALL_UPDATEBLOCK						\
	WATERFALL_STREAMWORDS							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	WATERFALL_NEXTINPUT							\
	WATERFALL_UPDATESTEP							\
	input = ++waterfallState.inputblocks;		\
	if (!input) waterfallState.inputslabs++;	\
	WATERFALL_UPDATESTEP
#else // not UNROLL
#define WATERFALL_UPDATEBLOCK						\
	for (i=0; i<16; i++) {				\
		WATERFALL_STREAMWORDS						\
		WATERFALL_NEXTINPUT						\
		WATERFALL_UPDATESTEP						\
	}									\
	WATERFALL_STREAMWORDS							\
	input = ++waterfallState.inputblocks;		\
	if (!input) waterfallState.inputslabs++;	\
	WATERFALL_UPDATESTEP
#endif // UNROLL

// -------------------------------------------------------------------------------------------------

// API routine to process some passed data into the hash state

int Waterfall::Update ( const BitSequence *data, DataLength databitlen ) {
	unsigned long long left;
	int ibuff, ibits, i;
	unsigned int input, *pinput;
	unsigned int word1, word2, word3;
	unsigned char last;

#if WATERFALL_DEBUG>0
	printf("Update: databitlen %d\n", databitlen);
#endif

// Number of bytes left in the input data
	left = databitlen / 8;

// If there is something in the buffer, fill up the buffer first
	ibuff = waterfallState.inputbits / 8;

	if (ibuff) {
		while (ibuff<64 && left>0) {
			waterfallState.inputbuffer[ibuff++] = *data++;
			left--;
		}

// If the buffer is now full, process the buffer, then mark it empty
		if (ibuff==64) {
			pinput = (unsigned int *) waterfallState.inputbuffer;
			WATERFALL_UPDATEBLOCK
			waterfallState.inputbits = 0;
			ibuff = 0;
		}
	}

// Pointer to the rest of the data
	pinput = (unsigned int *) data;

// If there something left in the input data, process as many whole blocks as we can
	while (left>=64) {
		WATERFALL_UPDATEBLOCK
		left -= 64;
	}

// Copy remaining whole bytes into the buffer
	data = (BitSequence *) pinput;
	while (left) {
		waterfallState.inputbuffer[ibuff++] = *data++;
		left--;
	}
	waterfallState.inputbits = ibuff*8;

// Handle a part of a byte (only in the last update) - zero least significant bits before storing
	ibits = (int) (databitlen % 8);
	if (ibits) {
		last = *data;
		last >>= 8-ibits;
		last <<= 8-ibits;
		waterfallState.inputbuffer[ibuff] = last;
		waterfallState.inputbits += ibits;
	}

	return SUCCESS;
}

// -------------------------------------------------------------------------------------------------

// Mangle the copy of Stream 1, optionally mixing with a section of one of the Pools

void Waterfall::FinalStep ( unsigned int *pworkin, unsigned int *ppool, unsigned int *pworkout ) {
	unsigned int i;

// Swap bits rotationally between elements of the array
	for (i=0; i<WATERFALL_STREAM1; i++) {
		pworkout[i] = (pworkin[i] & 0x14141414) | (pworkin[(i+3)%WATERFALL_STREAM1] & 0x28282828) | 
					(pworkin[(i+7)%WATERFALL_STREAM1] & 0x42424242) | (pworkin[(i+12)%WATERFALL_STREAM1] & 0x81818181);
	}

#if WATERFALL_DEBUG>2
	printf("  Bits swapped:\n");
	for(i=0; i<WATERFALL_STREAM1; i++) {
		if((i&7)==0) printf("  ");
		printf(" %8.8x", pworkout[i]);
		if((i&7)==7) printf("\n");
	}
#endif

// Apply the X-box to each member of the copy array
	for(i=0; i<WATERFALL_STREAM1; i++) Waterfall::Xbox(pworkout+i);

#if WATERFALL_DEBUG>2
	printf("  X-box applied:\n");
	for(i=0; i<WATERFALL_STREAM1; i++) {
		if((i&7)==0) printf("  ");
		printf(" %8.8x", pworkout[i]);
		if((i&7)==7) printf("\n");
	}
#endif

// If it is passed, exclusive-or a section of a Pool into the copy of Stream 1
	if (ppool!=NULL) for(i=0; i<WATERFALL_STREAM1; i++) pworkout[i] ^= ppool[i];

#if WATERFALL_DEBUG>2
	printf("  Masked:\n");
	for(i=0; i<WATERFALL_STREAM1; i++) {
		if((i&7)==0) printf("  ");
		printf(" %8.8x", pworkout[i]);
		if((i&7)==7) printf("\n");
	}
#endif

}

// -------------------------------------------------------------------------------------------------

// API routine to complete the algorithm and return the message digest

int Waterfall::Final ( BitSequence *hashval ) {
	unsigned int input, *pinput, *ppool;
	unsigned int word1, word2, word3;
	unsigned int i, pass;
	unsigned int work1[WATERFALL_STREAM1], work2[WATERFALL_STREAM1], *pworkin, *pworkout, *pwork;

#if WATERFALL_DEBUG>0
	printf("Final\n");
#endif

// May need to process the contents of the input buffer
	if (waterfallState.inputbits) {

	// Zero the end of the buffer (we don't want data going in twice)
		i = (waterfallState.inputbits+7) / 8;
		for(; i<64; i++) waterfallState.inputbuffer[i] = 0;

	// Process the buffer
		pinput = (unsigned int *) waterfallState.inputbuffer;
		WATERFALL_UPDATEBLOCK
	}

// Always process the bit count and slab count
	UpdateStep(waterfallState.inputbits);
	UpdateStep(waterfallState.inputslabs);

// Perform multiple steps with fixed input to mix the last entries in
	for(i=0; i<waterfallFinalUpdates; i++) Waterfall::UpdateStep(i);

#if WATERFALL_DEBUG>1
	printf(" Stream 1 after input:\n");
	for(i=0; i<WATERFALL_STREAM1; i++) {
		if((i&7)==0) printf(" ");
		printf(" %8.8x", waterfallState.stream1[i]);
		if((i&7)==7) printf("\n");
	}
	printf(" Stream 2 after input:\n ");
	for(i=0; i<WATERFALL_STREAM2; i++) printf(" %8.8x", waterfallState.stream2[i]);
	printf("\n");
	printf(" Stream 3 after input:\n ");
	for(i=0; i<WATERFALL_STREAM3; i++) printf(" %8.8x", waterfallState.stream3[i]);
	printf("\n");
	printf(" Pool 2 after input:\n");
	for(i=0; i<WATERFALL_POOL; i++) {
		if((i&7)==0) printf(" ");
		printf(" %8.8x", waterfallState.pool2[i]);
		if((i&7)==7) printf("\n");
	}
	printf(" Pool 3 after input:\n");
	for(i=0; i<WATERFALL_POOL; i++) {
		if((i&7)==0) printf(" ");
		printf(" %8.8x", waterfallState.pool3[i]);
		if((i&7)==7) printf("\n");
	}
#endif

// Take a copy of Stream 1
	for(i=0; i<WATERFALL_STREAM1; i++) work1[i] = waterfallState.stream1[i];

// Work arrays are used alternately as input and output
	pworkin = work1;
	pworkout = work2;

// Perform mangling steps, combining with Pool 2
	ppool = waterfallState.pool2;
	for(pass=0; pass<WATERFALL_POOLFACTOR; pass++) {
#if WATERFALL_DEBUG>1
		printf(" Combining with Pool 2 pass %d\n", pass);
#endif
		Waterfall::FinalStep(pworkin, ppool, pworkout);
		pwork = pworkin;
		pworkin = pworkout;
		pworkout = pwork;
		ppool += WATERFALL_STREAM1;
	}

// Same with Pool 3
	ppool = waterfallState.pool3;
	for(pass=0; pass<WATERFALL_POOLFACTOR; pass++) {
#if WATERFALL_DEBUG>1
		printf(" Combining with Pool 3 pass %d\n", pass);
#endif
		Waterfall::FinalStep(pworkin, ppool, pworkout);
		pwork = pworkin;
		pworkin = pworkout;
		pworkout = pwork;
		ppool += WATERFALL_STREAM1;
	}

// Same steps but with no Pool data
	for(pass=0; pass<waterfallFinalSteps; pass++) {
#if WATERFALL_DEBUG>1
		printf(" Mixing without Pools pass %d\n", pass);
#endif
		Waterfall::FinalStep(pworkin, NULL, pworkout);
		pwork = pworkin;
		pworkin = pworkout;
		pworkout = pwork;
	}

#if WATERFALL_DEBUG>1
	printf(" Mangled copy of Stream 1:\n");
	for(i=0; i<WATERFALL_STREAM1; i++) {
		if((i&7)==0) printf(" ");
		printf(" %8.8x", pworkin[i]);
		if((i&7)==7) printf("\n");
	}
#endif

// Exclusive-or the mangled copy of Stream 1 with Stream 1
	for(i=0; i<WATERFALL_STREAM1; i++) pworkin[i] ^= waterfallState.stream1[i];

// And with each section of each Pool to make Final one-way with respect to each of these
	ppool = waterfallState.pool2;
	for(pass=0; pass<WATERFALL_POOLFACTOR; pass++) {
		for(i=0; i<WATERFALL_STREAM1; i++) pworkin[i] ^= ppool[i];
		ppool += WATERFALL_STREAM1;
	}

	ppool = waterfallState.pool3;
	for(pass=0; pass<WATERFALL_POOLFACTOR; pass++) {
		for(i=0; i<WATERFALL_STREAM1; i++) pworkin[i] ^= ppool[i];
		ppool += WATERFALL_STREAM1;
	}
	
#if WATERFALL_DEBUG>1
	printf(" Final digest:\n");
	for(i=0; i<WATERFALL_STREAM1; i++) {
		if((i&7)==0) printf(" ");
		printf(" %8.8x", pworkin[i]);
		if((i&7)==7) printf("\n");
	}
#endif

// Return all the bytes required (little-endian transfer)
	for(i=0; i<(waterfallState.hashbitlen/32); i++) {
		hashval[4*i] = (unsigned char) pworkin[i];
		hashval[4*i+1] = (unsigned char) (pworkin[i]>>8);
		hashval[4*i+2] = (unsigned char) (pworkin[i]>>16);
		hashval[4*i+3] = (unsigned char) (pworkin[i]>>24);
	}
	return SUCCESS;
}

// -------------------------------------------------------------------------------------------------

// Perform the complete hash process in one call

int Waterfall::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen,
				BitSequence *hashval) {
	//HashReturn ret;
	int ret;
	//hashState state;

	ret = Waterfall::Init(hashbitlen);
	if (ret != SUCCESS) return ret;

	ret = Waterfall::Update(data, databitlen);
	if (ret != SUCCESS) return ret;

	ret = Waterfall::Final(hashval);
	return ret;
}

Waterfall::Waterfall(const int numRounds) {
	if (numRounds == -1) {
		waterfallFinalUpdates = WATERFALL_FINALUPDATES;
		waterfallFinalSteps = WATERFALL_FINALSTEPS;
	} else {
		waterfallFinalUpdates = numRounds;
		waterfallFinalSteps = numRounds;
	}
}