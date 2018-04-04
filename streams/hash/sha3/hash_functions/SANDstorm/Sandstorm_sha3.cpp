#include <stdio.h>
#include <memory.h>
#include "Sandstorm_sha3.h"
#include "SHA3_ref.h"
#include "DoBlockModMix.h"

// Constants to be used in initCompress for 224 and 256 bit compressions
DataLength sand_levelOneToFourConst[4][5][4];
DataLength sand_mainConstantInputWords[5][4];
DataLength sand_initialVectorTempWords[5][4];

/* As specified in the write up document for SANDstorm 256 bit hash constants*/
DataLength static const sand_constants_256[5][4] = {
	{0x6a09e667bb67ae85ull, 0x3c6ef372a54ff53aull, 0x510e527f9b05688cull, 0x1f83d9ab5be0cd19ull},
	{0xbb67ae853c6ef372ull, 0xa54ff53a510e527full, 0x9b05688c1f83d9abull, 0x5be0cd196a09e667ull},
	{0x3c6ef372a54ff53aull, 0x510e527f9b05688cull, 0x1f83d9ab5be0cd19ull, 0x6a09e667bb67ae85ull},
	{0xa54ff53a510e527full, 0x9b05688c1f83d9abull, 0x5be0cd196a09e667ull, 0xbb67ae853c6ef372ull},
	{0x510e527f9b05688cull, 0x1f83d9ab5be0cd19ull, 0x6a09e667bb67ae85ull, 0x3c6ef372a54ff53aull}
};
/* As specified in the write up document for SANDstorm 224 bit hash constants*/
DataLength static const sand_constants_224[5][4] = {
	{0xc1059ed8367cd507ull, 0x3070dd17f70e5939ull, 0xffc00b3168581511ull, 0x64f98fa7befa4fa4ull},
	{0x367cd5073070dd17ull, 0xf70e5939ffc00b31ull, 0x6858151164f98fa7ull, 0xbefa4fa4c1059ed8ull},
	{0x3070dd17f70e5939ull, 0xffc00b3168581511ull, 0x64f98fa7befa4fa4ull, 0xc1059ed8367cd507ull},
	{0xf70e5939ffc00b31ull, 0x6858151164f98fa7ull, 0xbefa4fa4c1059ed8ull, 0x367cd5073070dd17ull},
	{0xffc00b3168581511ull, 0x64f98fa7befa4fa4ull, 0xc1059ed8367cd507ull, 0x3070dd17f70e5939ull}
};

// constants for initializing (super)blocks at various levels
// based on the SHA-384/512 initial-value constants, which are
// derived from the fractional part (in hex) of the square roots
// of the primes 2-19 (for SHA-512) and 23-53 (for SHA-384).
sand_u128 sand_sandwlvlc384[20] =
{ 0xcbbb9d5dc1059ed8ull, 0x629a292a367cd507ull,
  0x9159015a3070dd17ull, 0x152fecd8f70e5939ull,
  0x67332667ffc00b31ull, 0x8eb44a8768581511ull,
  0xdb0c2e0d64f98fa7ull, 0x47b5481dbefa4fa4ull,

                         0x629a292a367cd507ull,
  0x9159015a3070dd17ull, 0x152fecd8f70e5939ull,
  0x67332667ffc00b31ull, 0x8eb44a8768581511ull,
  0xdb0c2e0d64f98fa7ull, 0x47b5481dbefa4fa4ull,
  0xcbbb9d5dc1059ed8ull,

  0x9159015a3070dd17ull, 0x152fecd8f70e5939ull,
  0x67332667ffc00b31ull, 0x8eb44a8768581511ull,
  0xdb0c2e0d64f98fa7ull, 0x47b5481dbefa4fa4ull,
  0xcbbb9d5dc1059ed8ull, 0x629a292a367cd507ull,

                         0x152fecd8f70e5939ull,
  0x67332667ffc00b31ull, 0x8eb44a8768581511ull,
  0xdb0c2e0d64f98fa7ull, 0x47b5481dbefa4fa4ull,
  0xcbbb9d5dc1059ed8ull, 0x629a292a367cd507ull,
  0x9159015a3070dd17ull,

  0x67332667ffc00b31ull, 0x8eb44a8768581511ull,
  0xdb0c2e0d64f98fa7ull, 0x47b5481dbefa4fa4ull,
  0xcbbb9d5dc1059ed8ull, 0x629a292a367cd507ull,
  0x9159015a3070dd17ull, 0x152fecd8f70e5939ull };

sand_u128 sand_sandwlvlc512[20] =
{ 0x6a09e667f3bcc908ull, 0xbb67ae8584caa73bull,
  0x3c6ef372fe94f82bull, 0xa54ff53a5f1d36f1ull,
  0x510e527fade682d1ull, 0x9b05688c2b3e6c1full,
  0x1f83d9abfb41bd6bull, 0x5be0cd19137e2179ull,

                         0xbb67ae8584caa73bull,
  0x3c6ef372fe94f82bull, 0xa54ff53a5f1d36f1ull,
  0x510e527fade682d1ull, 0x9b05688c2b3e6c1full,
  0x1f83d9abfb41bd6bull, 0x5be0cd19137e2179ull,
  0x6a09e667f3bcc908ull,

  0x3c6ef372fe94f82bull, 0xa54ff53a5f1d36f1ull,
  0x510e527fade682d1ull, 0x9b05688c2b3e6c1full,
  0x1f83d9abfb41bd6bull, 0x5be0cd19137e2179ull,
  0x6a09e667f3bcc908ull, 0xbb67ae8584caa73bull,

                         0xa54ff53a5f1d36f1ull,
  0x510e527fade682d1ull, 0x9b05688c2b3e6c1full,
  0x1f83d9abfb41bd6bull, 0x5be0cd19137e2179ull,
  0x6a09e667f3bcc908ull, 0xbb67ae8584caa73bull,
  0x3c6ef372fe94f82bull,

  0x510e527fade682d1ull, 0x9b05688c2b3e6c1full,
  0x1f83d9abfb41bd6bull, 0x5be0cd19137e2179ull,
  0x6a09e667f3bcc908ull, 0xbb67ae8584caa73bull,
  0x3c6ef372fe94f82bull, 0xa54ff53a5f1d36f1ull };

sand_u128 sand_sandwblkc = { 0x6135f68d4c0cbb6full, 0xb43b47a245778989ull };
sand_u128 sand_sandwblkd = { 0x79cc45195cf5b7a4ull, 0xaec4e7496801dbb9ull };

/* SAND_B constants used in the message schedule and the round functions */
static const unsigned long long SAND_B[32] = {
	0x428a2f9871374491ULL, 0xb5c0fbcfe9b5dba5ULL, 0x3956c25b59f111f1ULL, 0x923f82a4ab1c5ed5ULL,
	0xd807aa9812835b01ULL, 0x243185be550c7dc3ULL, 0x72be5d7480deb1feULL, 0x9bdc06a7c19bf174ULL,
	0xe49b69c1efbe4786ULL, 0x0fc19dc6240ca1ccULL, 0x2de92c6f4a7484aaULL, 0x5cb0a9dc76f988daULL,
	0x983e5152a831c66dULL, 0xb00327c8bf597fc7ULL, 0xc6e00bf3d5a79147ULL, 0x06ca635114292967ULL,
	0x27b70a852e1b2138ULL, 0x4d2c6dfc53380d13ULL, 0x650a7354766a0abbULL, 0x81c2c92e92722c85ULL,
	0xa2bfe8a1a81a664bULL, 0xc24b8b70c76c51a3ULL, 0xd192e819d6990624ULL, 0xf40e3585106aa070ULL,
	0x19a4c1161e376c08ULL};

// constants for message schedule & round constants
// these are a subset of the SHA2 round constants for SHA-512
// fractional part (in hex) of the cube roots of the first fifty primes
// the leading digits aren't completely random, but it's good enough
sand_u128 sand_sandwmsc[] = {
  0x428a2f98d728ae22ull, 0x7137449123ef65cdull, 0xb5c0fbcfec4d3b2full, 0xe9b5dba58189dbbcull,
  0x3956c25bf348b538ull, 0x59f111f1b605d019ull, 0x923f82a4af194f9bull, 0xab1c5ed5da6d8118ull,
  0xd807aa98a3030242ull, 0x12835b0145706fbeull, 0x243185be4ee4b28cull, 0x550c7dc3d5ffb4e2ull,
  0x72be5d74f27b896full, 0x80deb1fe3b1696b1ull, 0x9bdc06a725c71235ull, 0xc19bf174cf692694ull,
  0xe49b69c19ef14ad2ull, 0xefbe4786384f25e3ull, 0x0fc19dc68b8cd5b5ull, 0x240ca1cc77ac9c65ull,
  0x2de92c6f592b0275ull, 0x4a7484aa6ea6e483ull, 0x5cb0a9dcbd41fbd4ull, 0x76f988da831153b5ull,
  0x983e5152ee66dfabull, 0xa831c66d2db43210ull, 0xb00327c898fb213full, 0xbf597fc7beef0ee4ull,
  0xc6e00bf33da88fc2ull, 0xd5a79147930aa725ull, 0x06ca6351e003826full, 0x142929670a0e6e70ull,
  0x27b70a8546d22ffcull, 0x2e1b21385c26c926ull, 0x4d2c6dfc5ac42aedull, 0x53380d139d95b3dfull,
  0x650a73548baf63deull, 0x766a0abb3c77b2a8ull, 0x81c2c92e47edaee6ull, 0x92722c851482353bull,
  0xa2bfe8a14cf10364ull, 0xa81a664bbc423001ull, 0xc24b8b70d0f89791ull, 0xc76c51a30654be30ull,
  0xd192e819d6ef5218ull, 0xd69906245565a910ull, 0xf40e35855771202aull, 0x106aa07032bbd1b8ull,
  0x19a4c116b8d2d0c8ull, 0x1e376c085141ab53ull };

/* SB(x) = x ^ AES_sbox[x] */
static const unsigned char sand_fsbox[256] = {
	99, 125, 117, 120, 246, 110, 105, 194, 56, 8, 109, 32, 242, 218, 165, 121,
	218, 147, 219, 110, 238, 76, 81, 231, 181, 205, 184, 180, 128, 185, 108, 223,
	151, 220, 177, 5, 18, 26, 209, 235, 28, 140, 207, 218, 93, 245, 31, 58,
	52, 246, 17, 240, 44, 163, 51, 173, 63, 43, 186, 217, 215, 26, 140, 74,
	73, 194, 110, 89, 95, 43, 28, 231, 26, 114, 156, 248, 101, 174, 97, 203,
	3, 128, 82, 190, 116, 169, 231, 12, 50, 146, 228, 98, 22, 17, 6, 144,
	176, 142, 200, 152, 39, 40, 85, 226, 45, 144, 104, 20, 60, 81, 241, 199,
	33, 210, 50, 252, 230, 232, 78, 130, 196, 207, 160, 90, 108, 130, 141, 173,
	77, 141, 145, 111, 219, 18, 194, 144, 76, 46, 244, 182, 232, 208, 151, 252,
	240, 16, 221, 79, 182, 191, 6, 31, 222, 119, 34, 143, 66, 195, 149, 68,
	64, 147, 152, 169, 237, 163, 130, 251, 106, 122, 6, 201, 61, 56, 74, 214,
	87, 121, 133, 222, 57, 96, 248, 30, 212, 239, 78, 81, 217, 199, 16, 183,
	122, 185, 231, 237, 216, 99, 114, 1, 32, 20, 190, 212, 135, 112, 69, 69,
	160, 239, 103, 181, 156, 214, 32, 217, 185, 236, 141, 98, 90, 28, 195, 65,
	1, 25, 122, 242, 141, 60, 104, 115, 115, 247, 109, 2, 34, 184, 198, 48,
	124, 80, 123, 254, 75, 19, 180, 159, 185, 96, 215, 244, 76, 169, 69, 233};

/*===== Global variables used in ModMix =====*/
unsigned long long SAND_MSd[33];	/* message schedule buffer*/
unsigned long long SAND_ws[4];		/* hash state buffer */


int SandStorm::Init(int hashBitLen){
	sandState.hashbitlen = hashBitLen;

	if(hashBitLen <=256){
		int i,j;
		sandState.pipedBits = 0;
		sandState.initCompressFlag = 0;
		sandState.blockIters[0] = 0;
		sandState.blockIters[1] = 0;
		sandState.blockIters[2] = 0;
		// set the constants depending upon the hash length desired
		if (hashBitLen == 224){
			for(i=0;i<5;i++){
				for(j=0;j<4;j++){
					sand_initialVectorTempWords[i][j] = sand_constants_224[i][j];
					sand_mainConstantInputWords[i][j] = sand_constants_224[i][j];
				}
			}
		}else{
			for(i=0;i<5;i++){
				for(j=0;j<4;j++){
					sand_initialVectorTempWords[i][j] = sand_constants_256[i][j];
					sand_mainConstantInputWords[i][j] = sand_constants_256[i][j];
				}
			}
		}
	}else{
		int i;
		sandState.mbcnt = 0;
		sandState.msglen = k128(0,0);
		sandState.blkn1 = k128(0,1); 
		sandState.blkn2 = k128(0,1);
		sandState.lvl0flag = 0;
		sandState.lvl1cnt = 0; 
		sandState.lvl2cnt = 0;
		sandState.lvl3flag = 0;
		sandState.outsize = hashBitLen;
		if (hashBitLen==384) 
			sandState.lvlc = sand_sandwlvlc384; 
		else 
			sandState.lvlc = sand_sandwlvlc512;

		// setup level 0 constants
		for (i=0;i<20;i++) 
			sandState.prev[i] = (sandState.lvlc)[i];
	}
	return SAND_SUCCESS;
}


int SandStorm::Update(const BitSequence *data, DataLength databitlen){
	unsigned int neededBits, qDataIndex, bytesToCpy;
	unsigned long long dataBitsLeft, dataIndex;

	/*check to see if we have a larger hash and use alternate 512 bit update*/
	if(sandState.hashbitlen > 256){
		sandwupdate( &sandState, databitlen, data );
		return SAND_SUCCESS;
	}

	dataIndex = 0;
	dataBitsLeft = databitlen;
	qDataIndex = sandState.pipedBits / SAND_BYTELENGTH;

	// byteOffset = sandState.pipedBits >> 3;
	neededBits = SAND_MAXBITS - sandState.pipedBits;

	// This will check to see if we have enough bits to run compress (512), and if not it
	// will add those bits to the queue and then return
	if(dataBitsLeft < neededBits ){
		// if we are on the last call to update we could have a bits left that is not divisible
		// by 8. So if we have any bits we have to make sure we get the full last byte. 
		if( (dataBitsLeft & 7) > 0 ){
			bytesToCpy = ((unsigned int)databitlen / SAND_BYTELENGTH) + 1; 
		}else{
			bytesToCpy = (unsigned int)databitlen / SAND_BYTELENGTH;
		}
		memcpy(&(sandState.queuedData[qDataIndex]),data,(size_t)bytesToCpy);
		sandState.pipedBits += (int)databitlen;
		return SAND_SUCCESS;
	}

	/*	
	 *	This is true when the piped bits are greater than 0, but not yet 512, but because 
	 *	we passed the previous condition we know we have enough bits to process between
	 *	what is in the queue already and what has currently been input to the function
	 */
	if(sandState.pipedBits > 0){
		// figure out how many bytes are needed to complete 512 bits then put them into 
		// the message queue
		bytesToCpy = neededBits / SAND_BYTELENGTH;
		memcpy(&(sandState.queuedData[qDataIndex]),data,(size_t)bytesToCpy);		
		sandState.pipedBits = SAND_MAXBITS;
		dataIndex = bytesToCpy;
		dataBitsLeft -= neededBits;
	}
	else{
		// input must be >= full block (SAND_MAXBITS) to have gotten here
		memcpy(sandState.queuedData,&(data[dataIndex]),SAND_MAXBYTES);
		dataIndex = SAND_MAXBYTES;
		dataBitsLeft -= SAND_MAXBITS;
	} 

	/*
	 *	now we need to check if this is the first call to compress, and if it is it will
	 *	require a special first run to set state variables to all subsequent calls to compress.
	 *	 if it is not the first time then do a normal compress
	 */	
	if(sandState.initCompressFlag == 0){
		initCompress(&sandState);
	}else{
		compress(&sandState);
	}

	// fix this while loop so we don't have to set neededBits each time through
	while(dataBitsLeft>=SAND_MAXBITS){
		memcpy(sandState.queuedData,&(data[dataIndex]),SAND_MAXBYTES);
		dataBitsLeft -= SAND_MAXBITS;
		dataIndex += SAND_MAXBYTES;
		sandState.pipedBits = SAND_MAXBITS;

		// compress(state);
		compress(&sandState);
	}

	sandState.pipedBits = (int)dataBitsLeft;
	if( (dataBitsLeft & 7) > 0 ){
		bytesToCpy = ((unsigned int)dataBitsLeft / SAND_BYTELENGTH) + 1; 
	}else{
		bytesToCpy = (unsigned int)dataBitsLeft / SAND_BYTELENGTH;
	}
	if(bytesToCpy != 0) memcpy(sandState.queuedData,&(data[dataIndex]),(size_t)bytesToCpy);
	return SAND_SUCCESS;
}


int SandStorm::Final(BitSequence *hashval){
	int lastDataByteIndex, validBits, i, j;
	BitSequence mask, ormask; 
	DataLength Mess[8], messageLength;
	sand_u128 tmp;

	if(sandState.hashbitlen > 256){
		sandwfinish(&sandState);

		/* final hash is in sandState.prev[96-99], so convert to message */
		for (i=0; i < sandState.outsize/8; i++){
			tmp = r128(sandState.prev[96+i/16],8*i+8); 
			hashval[i] = (unsigned char) (tmp.lo & 0xFF); 
		}
	}else{
		// if we have some bits leftover, pad them with a one bit value of 1 and then zeros till 512 bits
		validBits = (BitSequence)(sandState.pipedBits & 7);
		lastDataByteIndex = sandState.pipedBits >> 3;
		if(validBits > 0){
			// last byte index will be the index of the byte that we have an integral number of bits
			// left to deal with. i.e. we have 4 bits left on the character indexed by this value. 
			mask = SAND_WORDMASK << (7-validBits);

			// this mask will zero out the non message bits. 
			sandState.queuedData[lastDataByteIndex] = sandState.queuedData[lastDataByteIndex] & mask;

			// set the bit to one to start the padding then perform inclusive or on last character
			ormask = SAND_ORMASK << (7-validBits);
			sandState.queuedData[lastDataByteIndex] = sandState.queuedData[lastDataByteIndex] | ormask;
		}else{
			sandState.queuedData[lastDataByteIndex] = 0x80; 
		}
		for(i=lastDataByteIndex+1;i<64;i++)
			sandState.queuedData[i] = 0x00;		// pad the rest of the message out with zeros

		// If we haven't done Level 0 compress do it, and go straight to Level 4 from there
		if(sandState.initCompressFlag == 0){
			// no level zero has been performed so we run here and set the message for level 4
			initCompress(&sandState);
			createMessageFromBlock(Mess,sand_initialVectorTempWords); 
			messageLength = sandState.pipedBits;
		}else{
			messageLength = (sandState.blockIters[0] + 1) * SAND_MAXBITS + sandState.pipedBits;		// add 1 for level 0
			// perform one last run of compress then find out where last ouput is at
			compress(&sandState);
			createMessageFromBlock(Mess,sandState.prevBlock[0]);
			// handle last set of do_block calls
			if(sandState.blockIters[1] > 0){
				Do_Block_ModMix_Ref(Mess,sandState.prevBlock[1]);
				createMessageFromBlock(Mess,sandState.prevBlock[1]);
				if(sandState.blockIters[2] > 0){
					Do_Block_ModMix_Ref(Mess,sandState.prevBlock[2]);
					createMessageFromBlock(Mess,sandState.prevBlock[2]);
				}
			}
		}
		// xor message length into level four constants
		for(i=0;i<5;i++){
			for(j=1;j<4;j+=2){
				sand_levelOneToFourConst[3][i][j] = sand_levelOneToFourConst[3][i][j] ^ messageLength; 
			}
		}
		#if SAND_INTERMEDIATE_VALUES == 1
			printf("Level 4:\n");
		#endif
		// final solution will be in sand_levelOneToFourConst[3]
		Do_Block_ModMix_Ref(Mess,sand_levelOneToFourConst[3]);	
		/* output = s1 ^ s2 ^ s3 ^ s4 4 ULLs */
		for(i=0;i<4;i++){
			Mess[i] = sand_levelOneToFourConst[3][1][i] ^ sand_levelOneToFourConst[3][2][i] ^ sand_levelOneToFourConst[3][3][i] ^ sand_levelOneToFourConst[3][4][i];	
		}
		// Final hash is in Mess so move it into hashval
		for(j=0;j<4;j++){
			for(i=7;i>=0;i--){
				int finalValIndex = (j*8) + (7-i);
				if(!(sandState.hashbitlen == 224 && j == 3 && i <= 3)){ 
					hashval[finalValIndex] = (char)(Mess[j] >> (i*8));
				}
			}
		}
	}
	return SAND_SUCCESS;
}


int SandStorm::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval){
	//sandHashState state;
	//SandHashReturn ercode;
	int ercode;

	// Initialize Hash Algorithm
	ercode = Init(hashbitlen);

	// Hash the data
	if (ercode == SAND_SUCCESS)
		ercode = Update (data, databitlen);
	
	// Finalize the hashing, passing the var hashval to hold the data
	if (ercode == SAND_SUCCESS)
		ercode = Final(hashval);

	// return status code
	return ercode;
}

void SandStorm::initCompress(sandHashState *state){
	int i, j, k; 
	DataLength Mess[8];
	state->initCompressFlag = 1;

						#if SAND_INTERMEDIATE_VALUES == 1
							printf("Level 0:\n");
						#endif

	// Copy the data in the queue to a variable capable of holding it as 8 64 bit words (unsigned long long)
	createMessageFromQueue(Mess,state);
	
	// perform initial do_block and store all outputs into the array LevelZeroConst, we won't need
	// these any more for the rest of the algorithm
	Do_Block_ModMix_Ref(Mess, sand_initialVectorTempWords);
	
	// now we need to set up the constants for levels 1 through 4 constants as some function of the
	// output of the initial block. index works like for C0 words it is mainConstINW[0][i] C1=MCIW[1][i]
	// for level 1
	for(i=0;i<4;i++){
		sand_levelOneToFourConst[0][0][i] = sand_mainConstantInputWords[0][i] ^ sand_initialVectorTempWords[4][i]; //for level 1 constant 0 = co ^ s4
		sand_levelOneToFourConst[0][1][i] = sand_mainConstantInputWords[1][i] ^ sand_initialVectorTempWords[1][i]; //for level 1 constant 1 = c1 ^ s1
		sand_levelOneToFourConst[0][2][i] = sand_mainConstantInputWords[2][i] ^ sand_initialVectorTempWords[2][i]; //for level 1 constant 2 = c2 ^ s2
		sand_levelOneToFourConst[0][3][i] = sand_mainConstantInputWords[3][i] ^ sand_initialVectorTempWords[3][i]; //for level 1 constant 3 = c3 ^ s3
		sand_levelOneToFourConst[0][4][i] = sand_mainConstantInputWords[4][i] ^ sand_initialVectorTempWords[4][i]; //for level 1 constant 4 = c4 ^ s4
	}

	// level 2, 3, and 4
	for(i=0;i<5;i++){
		for(j=0;j<4;j++){
			// level 2
			sand_levelOneToFourConst[1][i][j] = sand_levelOneToFourConst[0][i][j];
			// level 3
			sand_levelOneToFourConst[2][i][j] = sand_levelOneToFourConst[0][i][j];
			// level 4 
			sand_levelOneToFourConst[3][i][j] = sand_mainConstantInputWords[i][j];
		}
	}
	// special fixups for extra terms in constant table
	for (i=0;i<5;i++) {
	  sand_levelOneToFourConst[1][i][3] ^= SAND_CCONST_256;
	  sand_levelOneToFourConst[2][i][3] ^= SAND_DCONST_256;
	  sand_levelOneToFourConst[3][i][3] ^= SAND_DCONST_256;
	  sand_levelOneToFourConst[3][i][0] = ~sand_levelOneToFourConst[3][i][0];
	  sand_levelOneToFourConst[3][i][1] = ~sand_levelOneToFourConst[3][i][1];
	}

	/* Copy over new constants into the prevBlock array for subsequent calls to compress */
	for(i=0;i<3;i++){
		for(j=0;j<5;j++){
			for(k=0;k<4;k++){
				state->prevBlock[i][j][k] = sand_levelOneToFourConst[i][j][k];
			}
		}
	}

	// fixup for superblock numbers starting at 1 in levels 1 and 2
	for(i=0;i<2;i++){
		for(j=0;j<5;j++){
			for(k=1;k<4;k+=2){
				state->prevBlock[i][j][k] ^= 1;
			}
		}
	}
}

void SandStorm::compress(sandHashState *state){
	// First thing to do is convert the BitSequence character array to 8 64 bit ULLs
	int i, j;
	DataLength MessL1[8],MessL2[8],MessL3[8],levelOneSuperBlock, levelTwoSuperBlock;

	// Copy the data in the queue to a variable capable of holding it as 8 64 bit words (unsigned long long)
	createMessageFromQueue(MessL1,state);

	if(((unsigned char)(state->blockIters[0])%10) == 0 && state->blockIters[0] > 1){

						#if SAND_INTERMEDIATE_VALUES == 1
							printf("Level 3: Block: %lld\n",state->blockIters[2]);
						#endif

		createMessageFromBlock(MessL2,state->prevBlock[0]);
		// run block function on the 100 block level
		Do_Block_ModMix_Ref(MessL2,state->prevBlock[1]);

		// Reset rounds 2 through 5 for the next set of 10 and xor the super block #
		levelOneSuperBlock = state->blockIters[0] / SAND_L1SBLENGTH + 1;
		for(i=0;i<5;i++){
			for(j=0;j<4;j++){
				if (j&1)
					state->prevBlock[0][i][j] = sand_levelOneToFourConst[0][i][j] ^ levelOneSuperBlock;
				else
					state->prevBlock[0][i][j] = sand_levelOneToFourConst[0][i][j];
			}
		}
		// increment the block counters
		state->blockIters[1]++;

		/* This will evaluate to true when there is enough information to run a comprees
		* at the last level of mod mix. If we have 100 level 1 blocks complete then we 
		* pass the outputs of rounds 4 and 5 up to the third level as the message to a 
		* mod mix block. 
		*/
		if(((unsigned char)(state->blockIters[1])%100) == 0 && (state->blockIters[1] > 1) ){
							#if SAND_INTERMEDIATE_VALUES == 1
								if(state->blockIters[1] == 1 || state->blockIters[1] == 100){
									printf("Level 2: Block: %lld\n", state->blockIters[1]);
									sand_print_flag = 1;
								}
							#endif
			createMessageFromBlock(MessL3,state->prevBlock[1]);
			// run block function on level 3
			Do_Block_ModMix_Ref(MessL3,state->prevBlock[2]);
			// Reset rounds 2 through 5 for the next set of 100
			levelTwoSuperBlock = state->blockIters[1] / SAND_L2SBLENGTH + 1;
			for(i=0;i<5;i++){
				for(j=0;j<4;j++){
					if (j&1)
						state->prevBlock[1][i][j] = sand_levelOneToFourConst[1][i][j] ^ levelTwoSuperBlock;
					else
						state->prevBlock[1][i][j] = sand_levelOneToFourConst[1][i][j];
				}
			}	
			state->blockIters[2]++;
		}
	}
	#if SAND_INTERMEDIATE_VALUES == 1
		if(state->blockIters[0] == 1 || state->blockIters[0] == 10)
			printf("Level 1: Block: %d\n",state->blockIters[0]);
	#endif
	// run block function on the 10 block level (Level 1)
	Do_Block_ModMix_Ref(MessL1,state->prevBlock[0]);
	state->blockIters[0]++;
}

void SandStorm::createMessageFromQueue(DataLength m[8], sandHashState *state){
	int i; 
	unsigned int whi, wlo;

	// Copy the data in the queue to a variable capable of holding it as 8 64 bit words (unsigned long long)
	for(i=0;i<8;i++){
		memcpy(&m[i],&(state->queuedData[i*8]),8);
		whi = SAND_bswap_32((unsigned int)(m[i] >> 32));
		wlo = SAND_bswap_32((unsigned int)m[i]);
		m[i] = ((unsigned long long)wlo << 32) | whi;
	}
}

void SandStorm::createMessageFromBlock(DataLength M[8], DataLength input[5][4]){
	M[0] = input[1][0] ^ input[3][0];
	M[1] = input[1][1] ^ input[3][1];
	M[2] = input[1][2] ^ input[3][2];
	M[3] = input[1][3] ^ input[3][3];
	M[4] = input[2][0] ^ input[4][0];
	M[5] = input[2][1] ^ input[4][1];
	M[6] = input[2][2] ^ input[4][2];
	M[7] = input[2][3] ^ input[4][3];
}

void SandStorm::printPrevBlock(DataLength input[5][4], int level){
	int i,j;
	printf("\n\nThis is the output of the current constants for level %d", level);
	for(i=0;i<5;i++){
		printf("\n\tRound %d",i+1);
		for(j=0;j<4;j++){
			printf("\n\t\tWord[%d]: 0x%llx",j,input[i][j]);
		}
	}
	printf("\n");
}

void SandStorm::printMessage(DataLength Mess[8]){
	int i;
	printf("\n\nThe following is the current message:");
	for(i=0;i<8;i++){
		printf("\n\tMess[%d] = 0x%llx",i,Mess[i]);
	}
}

void SandStorm::printQueue(sandHashState *state, int id){
	int i;
	printf("\n\nID: %d\n  Queued Data:",id);
	for(i=0;i<64;i++){
		printf("%c",state->queuedData[i]);
	}
	printf("\n  Piped Bits:%d\n",state->pipedBits);
}
void SandStorm::print64BS(BitSequence bstmp[64], int id){
	int i;
	printf("\n\nID: %d\n  Queued Data:",id);
	for(i=0;i<64;i++){
		printf("%c",bstmp[i]);
	}
}

/* Low-level compression function for SANDstorm 224/256 */
void SandStorm::Do_Block_ModMix_Ref (unsigned long long data_input[8], unsigned long long prevBlockArr[5][4]) {

#if tunableSecurityParameter > 0 || SAND_INTERMEDIATE_VALUES == 1
	unsigned short i;
#endif
						#if SAND_INTERMEDIATE_VALUES == 1
							unsigned short j;
						#endif

	/*===== MESSAGE SCHEDULE =====*/

	/* First 8 M's of the message schedule are the input into this function */
	SAND_MSd[0] = data_input[0];
	SAND_MSd[1] = data_input[1];
	SAND_MSd[2] = data_input[2];
	SAND_MSd[3] = data_input[3];
	SAND_MSd[4] = data_input[4];
	SAND_MSd[5] = data_input[5];
	SAND_MSd[6] = data_input[6];
	SAND_MSd[7] = data_input[7];

	/* Calculate the rest of the message schedule */
	SAND_MSG_SCH(8);
	SAND_MSG_SCH(9);
	SAND_MSG_SCH(10);
	SAND_MSG_SCH(11);
	SAND_MSG_SCH(12);
	SAND_MSG_SCH(13);
	SAND_MSG_SCH(14);
	SAND_MSG_SCH(15);
  	SAND_MSG_SCH(16);
	SAND_MSG_SCH(17);
	SAND_MSG_SCH(18);
	SAND_MSG_SCH(19);
  	SAND_MSG_SCH(20);
	SAND_MSG_SCH(21);
	SAND_MSG_SCH(22);
	SAND_MSG_SCH(23);
  	SAND_MSG_SCH(24);
	SAND_MSG_SCH(25);
	SAND_MSG_SCH(26);
	SAND_MSG_SCH(27);
  	SAND_MSG_SCH(28);
	SAND_MSG_SCH(29);
	SAND_MSG_SCH(30);
	SAND_MSG_SCH(31);
  	SAND_MSG_SCH(32);

	/* Set up inputs for the first round; this is an XOR of the data input. */
  	SAND_ws[0] = SAND_ROT_LEFT64(SAND_MSd[0], SAND_BITMIX_ROT_BITS) ^ SAND_MSd[4];
  	SAND_ws[1] = SAND_ROT_LEFT64(SAND_MSd[1], SAND_BITMIX_ROT_BITS) ^ SAND_MSd[5];
  	SAND_ws[2] = SAND_ROT_LEFT64(SAND_MSd[2], SAND_BITMIX_ROT_BITS) ^ SAND_MSd[6];
  	SAND_ws[3] = SAND_ROT_LEFT64(SAND_MSd[3], SAND_BITMIX_ROT_BITS) ^ SAND_MSd[7];
	SAND_BIT_MIX();

						#if SAND_INTERMEDIATE_VALUES == 1
							printf("\nData:\n");
							for (i=0; i < 8; i++)
								printf("d%d = %016llX\n", i, data_input[i]);
							printf("\nMessage Schedule:\n");
							printf("MS(0,D) = ");
							for (i=0; i < 4; i++)
								printf("%016llX ", SAND_ws[i]);
							for (j=1; j < 5; j++)
							{
								printf("\nMS(%d,D) = ", j);
								for (i=0; i < 4; i++)
									printf("%016llX ", SAND_MSd[9+j*5+i]);
							}
							printf("\n\nInput Constants:\n");
							for (i=0; i < 5; i++)
							{
								printf("c%d = ", i);
								for (j=0; j < 4; j++)
									printf("%016llX ", prevBlockArr[i][j]);
								printf("\n");
							}
							printf("\n");
						#endif

	/*===== ROUND FUNCTIONS =====*/

	if (sandStormNumRounds >=1) {
	/* Round 0 */
	SAND_RND1_IN(prevBlockArr[0]);
	SAND_MOD_MIX_RND(0,0);
	SAND_MOD_MIX_RND(0,1);
	SAND_MOD_MIX_RND(0,2);	SAND_MOD_MIX_RND(0,3);
	SAND_BIT_MIX();
	SAND_RND_OUT(prevBlockArr[1], prevBlockArr[1]);
	}

	if (sandStormNumRounds >=2) {
	/* Round 1 */
	SAND_RND_IN(prevBlockArr[1], 14);
	SAND_MOD_MIX_RND(1,0);	SAND_MOD_MIX_RND(1,1);	SAND_MOD_MIX_RND(1,2);	SAND_MOD_MIX_RND(1,3);
	SAND_BIT_MIX();
	SAND_RND_OUT(prevBlockArr[2], prevBlockArr[1]);
	}

	if (sandStormNumRounds >=3) {
	/* Round 2 */
	SAND_RND_IN(prevBlockArr[1], 19);
	SAND_MOD_MIX_RND(2,0);	SAND_MOD_MIX_RND(2,1);	SAND_MOD_MIX_RND(2,2);	SAND_MOD_MIX_RND(2,3);
	SAND_BIT_MIX();
	SAND_RND_OUT(prevBlockArr[3], prevBlockArr[2]);
	}

	if (sandStormNumRounds >=4) {
	/* Round 3 */
	SAND_RND_IN(prevBlockArr[2], 24);
	SAND_MOD_MIX_RND(3,0);	SAND_MOD_MIX_RND(3,1);	SAND_MOD_MIX_RND(3,2);	SAND_MOD_MIX_RND(3,3);
	SAND_BIT_MIX();
	SAND_RND_OUT(prevBlockArr[4], prevBlockArr[3]);
	}

	if (sandStormNumRounds >=5) {
	/* Round 4 - Do the round, then store the output into prevBlock for next block's Round 3 input */
	SAND_RND_IN(prevBlockArr[3], 29);
	SAND_MOD_MIX_RND(4,0);	SAND_MOD_MIX_RND(4,1);	SAND_MOD_MIX_RND(4,2);	SAND_MOD_MIX_RND(4,3);
	SAND_BIT_MIX();
	}

	/* Extended Security Rounds */
#if tunableSecurityParameter > 0
	for (i=0; i < tunableSecurityParameter; i++){
		SAND_MOD_MIX_RND(4,0);	SAND_MOD_MIX_RND(4,1);	SAND_MOD_MIX_RND(4,2);	SAND_MOD_MIX_RND(4,3);
		SAND_BIT_MIX();
	}
#endif
	SAND_RND4_OUT(prevBlockArr[4]);

						#if SAND_INTERMEDIATE_VALUES == 1
							printf("State Outputs:\n");
							for (i=1; i < 5; i++)
							{
								printf("s%d = ", i);
								for (j=0; j < 4; j++)
									printf("%016llX ", prevBlockArr[i][j]);
								printf("\n");
							}
							printf("\n");
						#endif
}

// make a 128 bit constant from two 64 bit constants
sand_u128 SandStorm::k128(unsigned long long hi, unsigned long long lo) { sand_u128 hl; hl.hi = hi; hl.lo = lo; return hl; }

// add two 128 bit numbers, dropping any carry out of the high bit
sand_u128 SandStorm::a128(sand_u128 x, sand_u128 y)
{ sand_u128 sum;  sum.hi = x.hi + y.hi; sum.lo = x.lo + y.lo;
 if (sum.lo<x.lo) sum.hi++; return sum; }

// xor two 128 bit numbers
sand_u128 SandStorm::x128(sand_u128 x, sand_u128 y)
{ sand_u128 xor; xor.hi = x.hi ^ y.hi; xor.lo = x.lo ^ y.lo; return xor; }

// complement a 128 bit number
sand_u128 SandStorm::c128(sand_u128 x)
{ sand_u128 comp; comp.hi = ~ x.hi; comp.lo = ~ x.lo; return comp; }

// and two 128 bit numbers
sand_u128 SandStorm::b128(sand_u128 x, sand_u128 y)
{ sand_u128 and; and.hi = x.hi & y.hi; and.lo = x.lo & y.lo; return and; }

// rotate left a 128 bit number
sand_u128 SandStorm::r128(sand_u128 x, int sh)
{ sand_u128 rot; sh &= 127;
 if (sh==0)       { rot.hi = x.hi;
                    rot.lo = x.lo; }
 else if (sh<64)  { rot.hi = (x.hi << sh) | (x.lo >> (64-sh));
                    rot.lo = (x.lo << sh) | (x.hi >> (64-sh)); }
 else if (sh==64) { rot.hi = x.lo;
                    rot.lo = x.hi; }
 else             { rot.hi = (x.lo << (sh-64)) | (x.hi >> (128-sh));
                    rot.lo = (x.hi << (sh-64)) | (x.lo >> (128-sh)); };
 return rot; }

// multiply two 64 bit numbers, producing a 128 bit product
unsigned int xhi_s, xlo_s;
unsigned long long xhi_m, xlo_m, yhi_m, ylo_m;
unsigned long long tmp_m, tmp2_m;
sand_u128 prod_m;

sand_u128 SandStorm::m128(unsigned long long x, unsigned long long y)
{ 
#if SAND_ASM_CODE == 0
  xhi_m = x>>32;
  xlo_m = x&sand_msk32;
  yhi_m = y>>32;
  ylo_m = y&sand_msk32;

  prod_m.hi = xhi_m*yhi_m;
  prod_m.lo = xlo_m*ylo_m;
  tmp_m = xlo_m*yhi_m;
  tmp2_m = xhi_m*ylo_m;
  tmp_m += tmp2_m;
  if (tmp_m<tmp2_m) 
	  prod_m.hi += sand_b32;
  prod_m.hi += tmp_m>>32;
  tmp_m <<= 32;
  prod_m.lo += tmp_m;
  if (prod_m.lo<tmp_m)
	  prod_m.hi++;
#else
__asm{
	push	eax
	push	edx
; DWORD PTR y	; ylo_m
; DWORD PTR y+4	; yhi_m
; DWORD PTR x	; xlo_m
; DWORD PTR x+4	; xhi_m

; 763  :   prod_m.hi = xhi_m*yhi_m;
	mov	eax, DWORD PTR y+4
	mul	DWORD PTR x+4
	mov	DWORD PTR prod_m, eax
	mov	DWORD PTR prod_m+4, edx

; 764  :   prod_m.lo = xlo_m*ylo_m;
	mov	eax, DWORD PTR x
	mul	DWORD PTR y
	mov	DWORD PTR prod_m+8, eax
	mov	DWORD PTR prod_m+12, edx

; 766  :   tmp2_m = xhi_m*ylo_m;
	mov	eax, DWORD PTR x+4
	mul	DWORD PTR y
	mov	DWORD PTR tmp2_m, eax
	mov	DWORD PTR tmp2_m+4, edx

; 765  :   tmp_m = xlo_m*yhi_m;
	mov	eax, DWORD PTR x
	mul	DWORD PTR y+4
	mov	DWORD PTR tmp_m, eax
	mov	DWORD PTR tmp_m+4, edx

; 767  :   tmp_m += tmp2_m;
	add	eax, DWORD PTR tmp2_m
	adc	edx, DWORD PTR tmp2_m+4

; 768  :   if (tmp_m<tmp2_m) 
	cmp	edx, DWORD PTR tmp2_m+4
	ja	SHORT $tmp_ge_tmp2
	jb	SHORT $tmp_lt_tmp2
	cmp	eax, DWORD PTR tmp2_m
	jae	SHORT $tmp_ge_tmp2

$tmp_lt_tmp2:
; 769  : 	  prod_m.hi += sand_b32;
	add	DWORD PTR prod_m+4, 1

$tmp_ge_tmp2:
; 770  :   prod_m.hi += tmp_m>>32;
	add	DWORD PTR prod_m, edx
	adc	DWORD PTR prod_m+4, 0

; 771  :   tmp_m <<= 32;
; 772  :   prod_m.lo += tmp_m;
	add	DWORD PTR prod_m+12, eax

; 773  :   if (prod_m.lo<tmp_m)
	cmp	DWORD PTR prod_m+12, eax
	jae	SHORT $prodlo_ge_tmp

; 774  : 	  prod_m.hi++;
	add	DWORD PTR prod_m, 1
	adc	DWORD PTR prod_m+4, 0
$prodlo_ge_tmp:
	pop	edx
	pop	eax
}
#endif
  return prod_m; 
}

// square a 64 bit number, producing a 128 bit result
sand_u128 SandStorm::s128(unsigned long long x)
{ 
#if SAND_ASM_CODE == 0
  unsigned int xhi_m = (unsigned int)(x>>32), xlo_m = (unsigned int)(x&sand_msk32);
  prod_m.hi = ((unsigned long long)xhi_m)*xhi_m; 
  prod_m.lo = ((unsigned long long)xlo_m)*xlo_m; 
  tmp_m = ((unsigned long long)xhi_m)*xlo_m;
  prod_m.hi += tmp_m>>31; 
  tmp_m<<=33; 
  prod_m.lo += tmp_m; 
  if (prod_m.lo<tmp_m) 
	  prod_m.hi++;
  #else
__asm{
	push	eax
	push	ecx
	push	edx
	push	esi
; DWORD PTR x	; xlo_s
; DWORD PTR x+4	; xhi_s
; 763  :   prod_m.hi = xhi_s*xhi_s;
	mov	eax, DWORD PTR x+4
	mul	eax
	mov	DWORD PTR prod_m, eax
	mov	DWORD PTR prod_m+4, edx

; 764  :   prod_m.lo = xlo_s*xlo_s;
	mov	eax, DWORD PTR x
	mul	eax
	mov	DWORD PTR prod_m+8, eax
	mov	DWORD PTR prod_m+12, edx

; 766  :   tmp_m = xhi_s*xlo_s;
	mov	eax, DWORD PTR x+4
	mul	DWORD PTR x

; 765  :   prod_m.hi += tmp_m>>31; 
	mov	esi, DWORD PTR prod_m+8
	mov	ecx, eax
	shrd	ecx, edx, 31
	shr	edx, 31
	add	DWORD PTR prod_m, ecx
	mov	ecx, DWORD PTR prod_m+12
	adc	DWORD PTR prod_m+4, edx
	add	eax, eax
	xor	edx, edx
	add	esi, edx
	adc	ecx, eax
	mov	DWORD PTR prod_m+8, esi
	mov	DWORD PTR prod_m+12, ecx

; 755  :   if (prod_m.lo<tmp_m) 
	cmp	ecx, eax
	ja	SHORT $prodmlo_ge_tmpm
	jb	SHORT $prodmlo_lt_tmpm
	cmp	esi, edx
	jae	SHORT $prodmlo_ge_tmpm

; 756  : 	  prod_m.hi++;
$prodmlo_lt_tmpm:
	add	DWORD PTR prod_m, 1
	adc	DWORD PTR prod_m+4, edx
$prodmlo_ge_tmpm:
	pop esi
	pop	edx
	pop ecx
	pop	eax
}
#endif
  return prod_m;
}

sand_u128 SandStorm::sandwf(sand_u128 x)
{ return a128(s128(x.hi),s128(x.lo)); }

sand_u128 SandStorm::sandwg(sand_u128 x)
{ return a128(s128(x.hi),
              a128(s128(x.lo),r128(m128(x.hi+SAND_ACONST_512,
										x.lo+SAND_BCONST_512),64))); }

// apply the AES sbox to the low byte of X
sand_u128 SandStorm::sb128(sand_u128 x)
{ sand_u128 val; val.hi = x.hi; val.lo = x.lo ^ sand_fsbox[x.lo & 255]; return val; }

// where to start picking in Message Schedule
int sandwmspick[] = { 4, 14, 19, 24, 29 };

void SandStorm::mscpr(sand_u128 *ms)
{ int i; printf("ms ");
   for (i=0;i<33;i++) { printf(" %016llx %016llx",ms[i].hi,ms[i].lo);
   if (i&1) printf("\n   "); } printf("\n"); }

// Compute the message schedule
// Assumes the message is present in the first 8 words (128 bits each)
// Begin by xoring the first 4 words (rotated) into the second 4 words.
// Then apply the mixing function to compute words after the first 8.
void SandStorm::sandwmsgsch(sand_u128 *MS)
{ int i;  sand_u128 tmp;
 for (i=8;i<33;i++)
  MS[i] = r128(sb128(a128(    MS[i-8],
                      a128(   sand_sandwmsc[i-8],
                       a128(  sandwg(MS[i-1]),
			a128( x128(b128(MS[i-1],MS[i-2]),   // Ch(i-1,i-2,i-3)
                                   b128(c128(MS[i-1]),MS[i-3])),
                              MS[i-4]))))),
               59 );
 for (i=0;i<4;i++) MS[i+4] = x128(r128(MS[i],37),MS[i+4]);
 // bitmix
   tmp = b128(x128(MS[4],MS[6]),SAND_J6_128); MS[4] = x128(MS[4],tmp); MS[6] = x128(MS[6],tmp);
   tmp = b128(x128(MS[5],MS[7]),SAND_J3_128); MS[5] = x128(MS[5],tmp); MS[7] = x128(MS[7],tmp);
   tmp = b128(x128(MS[4],MS[5]),SAND_J5_128); MS[4] = x128(MS[4],tmp); MS[5] = x128(MS[5],tmp);
   tmp = b128(x128(MS[6],MS[7]),SAND_J5_128); MS[6] = x128(MS[6],tmp); MS[7] = x128(MS[7],tmp);
 }

// Run the compression function
// message (in *msg) is 8 128-bit words;
// prior block output is in prev[20*level+4*round+word]; result goes here too.
// inner state is w[0-3], 4 128-bit words
void SandStorm::sandwcmprs(sand_u128 msg[], sand_u128 prev[], int level, int xrnds)
{ int i,j,rnd; sand_u128 MS[33], w[4];  sand_u128 tmp;
 for (i=0;i<8;i++) MS[i] = msg[i]; 
 sandwmsgsch(MS);

						#if SAND_INTERMEDIATE_VALUES == 1 
							if(level == 1) SAND_GLOBAL_512_L1_BNUM++;
							else if(level == 2) SAND_GLOBAL_512_L2_BNUM++;
							else if(level == 3) SAND_GLOBAL_512_L3_BNUM++;
							if(level == 0){ printf("Level 0:\n"); sand_print_flag = 1;}
							else if(level == 4){ printf("Level 4:\n"); sand_print_flag = 1;}
							else if(level == 3){
								printf("Level %d: Block Number %llu:\n",level,SAND_GLOBAL_512_L3_BNUM-1);
								sand_print_flag = 1;}
							else if(level == 2){
								printf("Level %d: Block Number %llu:\n",level,SAND_GLOBAL_512_L2_BNUM-1);
								sand_print_flag = 1;}
							else if(level == 1 && (SAND_GLOBAL_512_L1_BNUM == 1 || SAND_GLOBAL_512_L1_BNUM == 10)){
								sand_print_flag = 1;
								printf("Level 1: Block %llu:\n",SAND_GLOBAL_512_L1_BNUM-1);
							}
							else
								printf(" ...\n\n");

							if (sand_print_flag == 1){
								printf("\nCompression Function Data Input Words:\n");
								for (i=0; i < 8; i++)
									printf("d%d = %016llX%016llX\n", i, msg[i].hi, msg[i].lo);
								printf("\nMessage Schedule:");
								printf("\nMS(0,D) = %016llX%016llX\n", MS[4].hi, MS[4].lo);
								for (i=1; i < 4; i++)
									printf("          %016llX%016llX\n", MS[4+i].hi, MS[4+i].lo);
								for (j=1; j < 5; j++)
								{
									printf("\nMS(%d,D) = ", j);
									printf("%016llX%016llX\n", MS[9+j*5].hi, MS[9+j*5].lo);
									for (i=1; i < 4; i++)
										printf("          %016llX%016llX\n", MS[9+j*5+i].hi, MS[9+j*5+i].lo);
								}
								printf("\nInput Constants:");
								for (rnd=0; rnd < 5; rnd++)
								{
									int constantIndex = 20*level + 4*rnd; 
									printf("\nc%d = ", rnd);
									printf("%016llX%016llX\n", prev[constantIndex].hi, prev[constantIndex].lo);
									for (i=1; i < 4; i++)
										printf("     %016llX%016llX\n", prev[constantIndex+i].hi, prev[constantIndex+i].lo);
								}
								printf("\n");
								printf("State Outputs:\n");
							}
						#endif

 for (i=0;i<4;i++) w[i] = prev[20*level+i];
 for (rnd=0;rnd<=4;rnd++)
 { for (i=0;i<4;i++) w[i] = x128(w[i],MS[sandwmspick[rnd]+i]);
   for (j=0;j<=((rnd<4)?0:xrnds);j++)
   {for (i=0;i<4;i++)
    w[i] = r128(sb128(a128(   w[i],
                       a128(  sandwf(w[(i+3)&3]),
                        a128( x128(w[(i+1)&3],b128(w[(i+3)&3],x128(w[i^2],w[(i+1)&3]))),
			      sand_sandwmsc[24-i-(rnd<<2)])))),57);
    // bitmix
   tmp = b128(x128(w[0],w[2]),SAND_J6_128); w[0] = x128(w[0],tmp); w[2] = x128(w[2],tmp);
   tmp = b128(x128(w[1],w[3]),SAND_J3_128); w[1] = x128(w[1],tmp); w[3] = x128(w[3],tmp);
   tmp = b128(x128(w[0],w[1]),SAND_J5_128); w[0] = x128(w[0],tmp); w[1] = x128(w[1],tmp);
   tmp = b128(x128(w[2],w[3]),SAND_J5_128); w[2] = x128(w[2],tmp); w[3] = x128(w[3],tmp);
   }
   if (rnd<4) for (i=0;i<4;i++) w[i] = x128(w[i],prev[20*level+4*rnd+4+i]);
						#if SAND_INTERMEDIATE_VALUES == 1
							if (sand_print_flag == 1){
								for (rnd=1;rnd<5;rnd++){
									printf("s%d = ", rnd);
									printf("%016llX%016llX\n", prev[20*level+4*rnd+i].hi, prev[20*level+4*rnd+i].lo);
									for (i=1; i < 4; i++)
										printf("     %016llX%016llX\n", prev[20*level+4*rnd+i].hi, prev[20*level+4*rnd+i].lo);
									printf("\n");
								}
								sand_print_flag = 0;
							}
						#endif
   if (rnd) for (i=0;i<4;i++) prev[20*level+4*rnd+i] = w[i]; };
 }

void SandStorm::hstpr(sandHashState *hst)
{ int i;
 printf("\nsandstate %x:\n", hst);
 printf("mbcnt %d\n", hst->mbcnt);
 for (i=0;i<128;i++) { if (!i) printf("msgbytes ");
 printf(" %02x",hst->msgbytes[i]); if (i%20==16 || i==127) printf("\n"); };
 printf("msg ");
 for(i=0;i<8;i++) { printf(" %016llx %016llx",hst->msg[i].hi,hst->msg[i].lo);
                    if (i&1 && i<7) printf("\n    "); if (i==7) printf("\n"); }
 printf("msglen %lld %lld  blkn1 %lld %lld  blkn2 %lld %lld\n",
	hst->msglen.hi, hst->msglen.lo, hst->blkn1.hi,
	hst->blkn1.lo, hst->blkn2.hi, hst->blkn2.lo);
 printf("lvl0flag %d  lvl1cnt %d  lvl2cnt %d  lvl3flag %d\n",
	hst->lvl0flag, hst->lvl1cnt, hst->lvl2cnt, hst->lvl3flag);
 printf("prev:\n");
 for (i=0;i<100;i++) { printf(" %16llx %16llx",hst->prev[i].hi,hst->prev[i].lo);
                       if (i&1)printf("\n"); if (i%20==19)printf("\n"); };
 printf("outsize %d  lvlc %x\n\n", hst->outsize, hst->lvlc); }

//int xtrnds=tunableSecurityParameter;     // number of extra times to do round 4

// general philosophy is lazy evaluation
// we postpone all use of the compression function until absolutely needed

// process a block of message
// may trigger processing of higher levels if necessary
void SandStorm::sandwdoit( sandHashState *hstate )
{ int i; sand_u128 w;
  // check if there's higher level work to do
  if (hstate->lvl1cnt==10)
  { if (hstate->lvl2cnt==100)
    { for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[44+i],hstate->prev[52+i]);
      if (!hstate->lvl3flag)  // setup level 3 starting round constants
      { for (i=0;i<20;i++) hstate->prev[60+i] = x128((hstate->lvlc)[i],hstate->prev[(i<4)?i+16:i]);
        for (i=0;i<20;i+=4) hstate->prev[63+i] = x128(hstate->prev[63+i],sand_sandwblkd); }
      sandwcmprs(hstate->msg,hstate->prev,3,tunableSecurityParameter);
      hstate->lvl2cnt=0; hstate->blkn2 = a128(hstate->blkn2,k128(0,1));
      hstate->lvl3flag=1; }
    if (!hstate->lvl2cnt)  // setup level2 superblock starting round constants
    { for (i=0;i<20;i++) hstate->prev[40+i] = x128((hstate->lvlc)[i],hstate->prev[(i<4)?i+16:i]);
      for (i=0;i<20;i+=4) hstate->prev[43+i] = x128(hstate->prev[43+i],sand_sandwblkc);
      for (i=0;i<20;i+=2) hstate->prev[41+i] = x128(hstate->prev[41+i], hstate->blkn2); }
    for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[24+i],hstate->prev[32+i]);
    sandwcmprs(hstate->msg,hstate->prev,2,tunableSecurityParameter);
    hstate->lvl1cnt=0; hstate->blkn1 = a128(hstate->blkn1,k128(0,1));
    hstate->lvl2cnt++; }
  for (i=0;i<8;i++) hstate->msg[i] = k128(0,0);
  for (i=0;i<128;i++)   // pack 128 message bytes into eight 128-bit words
  { w = k128(0,hstate->msgbytes[i]);
    hstate->msg[i>>4] = x128(hstate->msg[i>>4], r128(w,120-((i&15)<<3))); }
  hstate->mbcnt = 0;   // reset byte count to 0.
  // if lvl0flag = 0, we're doing level 0
  if (hstate->lvl0flag==0)
  { sandwcmprs(hstate->msg,hstate->prev,0,tunableSecurityParameter); hstate->lvl0flag = 1; }
  else
  { if (!hstate->lvl1cnt)  // setup level1 superblock starting round constants
    { for (i=0;i<20;i++) hstate->prev[20+i] = x128((hstate->lvlc)[i],hstate->prev[(i<4)?i+16:i]);
      for (i=0;i<20;i+=2) hstate->prev[21+i] = x128(hstate->prev[21+i], hstate->blkn1); }
    sandwcmprs(hstate->msg,hstate->prev,1,tunableSecurityParameter); hstate->lvl1cnt++; };
 }

// bits of data must be a multiple of 8, except for last partial byte
// update is responsible for zeroing unused low order bits in last byte
// mbcnt points to first unused byte, including after any partial byte
void SandStorm::sandwupdate( sandHashState *hstate, unsigned long long bitsofdata, const unsigned char *data )
{ int j;
  for (j=0;j<bitsofdata;)
  { if (hstate->mbcnt>=128) sandwdoit(hstate); // we've got a full message block to process
    hstate->msgbytes[hstate->mbcnt] = *data; data++; j+=8;
    if (j>bitsofdata) // handle partial byte
      hstate->msgbytes[hstate->mbcnt] &= 0xFF<<(j-bitsofdata);
    hstate->mbcnt++; }
  hstate->msglen = a128(hstate->msglen,k128(0,bitsofdata)); }

// do padding, process any partial block, propagate up levels
void SandStorm::sandwfinish( sandHashState *hstate )
{ int i, padbitpos;
  // process message buffer if full
  if (hstate->mbcnt>=128 && !(hstate->msglen.lo & 7)) sandwdoit(hstate);
  for (i=hstate->mbcnt;i<128;i++) hstate->msgbytes[i]=0; // zero out unused message bytes
  padbitpos = (int)(hstate->msglen.lo & 7);
  if (!padbitpos) hstate->mbcnt++;
  hstate->msgbytes[hstate->mbcnt-1] |= 0x80 >> padbitpos;
  sandwdoit(hstate);

  // at this point, the message buffer is emptied out.
  // we've just completed either a level 0 block, or a level 1 block.
  // if a level 0 block, we can push on with impunity.
  // if a level 1 block, we need to ask about the situation with levels 2 and 3.
  // if we've never started level 2, we can just use the level 1 results, for
  // input to level 4, as we do below.  but if we've started level 2, and
  // there's a level 1 residue, we need to move that level 1 block up to level 2.
  // we might need to vacate the present level 2 occupant, moving him up to level 3.
  // and we might need to terminate level 2 and go directly to level 4;
  // or to push out any level 2 partial superblock into level 3.

  // if we've ever used level 2, we might need to cleanup partial blocks
  //  from levels 1-3.
  if (!(hstate->lvl2cnt==0 && hstate->blkn2.hi==0 && hstate->blkn2.lo==1))
  { // unless level 1 is empty, send partial superblock to level 2
    if (hstate->lvl1cnt)
    { // make sure there's room at level 2
      if (hstate->lvl2cnt==100)
      { for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[44+i],hstate->prev[52+i]);
        if (!hstate->lvl3flag)  // setup level 3 starting round constants
        { for (i=0;i<20;i++) hstate->prev[60+i] = x128((hstate->lvlc)[i],hstate->prev[(i<4)?i+16:i]);
          for (i=0;i<20;i+=4) hstate->prev[63+i] = x128(hstate->prev[63+i],sand_sandwblkd); }
	sandwcmprs(hstate->msg,hstate->prev,3,tunableSecurityParameter);
	hstate->lvl2cnt=0; hstate->blkn2 = a128(hstate->blkn2,k128(0,1));
	hstate->lvl3flag=1; }
      if (!hstate->lvl2cnt)  // setup level2 superblock starting round constants
      { for (i=0;i<20;i++) hstate->prev[40+i] = x128((hstate->lvlc)[i],hstate->prev[(i<4)?i+16:i]);
        for (i=0;i<20;i+=4) hstate->prev[43+i] = x128(hstate->prev[43+i],sand_sandwblkc);
	for (i=0;i<20;i+=2) hstate->prev[41+i] = x128(hstate->prev[41+i], hstate->blkn2); }
      for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[24+i],hstate->prev[32+i]);
      sandwcmprs(hstate->msg,hstate->prev,2,tunableSecurityParameter);
      hstate->lvl1cnt=0; hstate->blkn1 = a128(hstate->blkn1,k128(0,1));
      hstate->lvl2cnt++; }
    // we've forwarded any partial superblock from level 1 to level 2
    // if we've ever used level 3, we need to forward any partial level 2 superblock
    if (hstate->lvl3flag && hstate->lvl2cnt)

    { for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[44+i],hstate->prev[52+i]);
      sandwcmprs(hstate->msg,hstate->prev,3,tunableSecurityParameter);
      hstate->lvl2cnt=0; hstate->blkn2 = a128(hstate->blkn2,k128(0,1)); }; };

  // any partial superblocks have been forwarded up the tree, unless they
  // are the first superblock at their level.

  // ready for level 4.  input might come from level 0, 1, 2, or 3.

  // setup level4 round constants
  for (i=0;i<20;i++) hstate->prev[80+i] = (hstate->lvlc)[i];
  for (i=0;i<20;i+=4) hstate->prev[83+i] = x128(hstate->prev[83+i],sand_sandwblkd);
  for (i=0;i<20;i+=2) hstate->prev[81+i] = x128(hstate->prev[81+i], hstate->msglen);
  for (i=0;i<20;i++) if (!(i&2)) hstate->prev[80+i] = c128(hstate->prev[80+i]);
  // see if we've only used level 0
  if (hstate->lvl1cnt==0 && hstate->blkn1.hi==0 && hstate->blkn1.lo==1)
   for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[4+i],hstate->prev[12+i]);
  // or if we've only used levels 0 & 1
  else if (hstate->lvl2cnt==0 && hstate->blkn2.hi==0 && hstate->blkn2.lo==1)
   for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[24+i],hstate->prev[32+i]);
  // or if we've only used level 0, 1, & 2
  else if (hstate->lvl3flag==0)
   for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[44+i],hstate->prev[52+i]);
  else  // we've used level 3
   for (i=0;i<8;i++) hstate->msg[i] = x128(hstate->prev[64+i],hstate->prev[72+i]);
  sandwcmprs(hstate->msg,hstate->prev,4,tunableSecurityParameter);
  // xor round values from rounds 1-4 to create final hash value
  for (i=0;i<4;i++)
    hstate->prev[96+i] = x128(hstate->prev[84+i],
			  x128(hstate->prev[88+i],
			   x128(hstate->prev[92+i],hstate->prev[96+i])));
  // final hash in prev[96-99]
}

void SandStorm::sandwprint( sandHashState *hstate )
{ if (hstate->outsize==384)
    printf(" %016llx %016llx %016llx\n %016llx %016llx %016llx\n",
	   hstate->prev[96].hi, hstate->prev[96].lo, hstate->prev[97].hi,
	   hstate->prev[97].lo, hstate->prev[98].hi, hstate->prev[98].lo);
  else printf(" %016llx %016llx %016llx %016llx\n %016llx %016llx %016llx %016llx\n",
  hstate->prev[96].hi, hstate->prev[96].lo, hstate->prev[97].hi, hstate->prev[97].lo,
  hstate->prev[98].hi, hstate->prev[98].lo, hstate->prev[99].hi, hstate->prev[99].lo);
 }

SandStorm::SandStorm(const int numRounds) {
	if (numRounds > 5) {
		tunableSecurityParameter = (numRounds - 5);
		sandStormNumRounds = 5;
	} else {
		tunableSecurityParameter = 0;
		if (numRounds == -1) {
			sandStormNumRounds = SAND_DEFAULT_ROUNDS;
		} else {
			sandStormNumRounds = numRounds;
		}
	}
}