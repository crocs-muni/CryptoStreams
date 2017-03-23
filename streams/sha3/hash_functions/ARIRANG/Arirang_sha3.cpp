#include <stdio.h>
#include <memory.h>
extern "C" {
#include "Arirang_OP32.h"
}
#include "Arirang_sha3.h"


Arirang::Arirang(int numRounds) {
	if (numRounds == -1) {
		arirangNumRounds = ARIRANG_DEFAULT_ROUNDS;
	} else {
		arirangNumRounds = numRounds;
	}
}

//
// PROJECT : ARIRANG family(ARIRANG-224,ARIRANG-256,ARIRANG-384,ARIRANG-512)
//
// DATE    : 2008.10.23
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE  : SHA3api_OP32.c
//
// NOTES : Optimized code of ARIRANG family
// 
//         Based on 32-bit platform (with four 32-bit S-box and sixteen 32-bit S-box version)
//
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : Init
//
// DESCRIPTION    : Initialize a hashState with the intended hash length of particular instantiation.
//
// PARAMETERS     : state - a structure that holds the hashState information
//                  hashbitlen - an integer value that indicates the length of the hash output in bits
//
// RETURNS        : SUCCESS - on success
//                  BAD_HASHLEN - hashbitlen is invalid
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int Arirang::Init(int hashbitlen)
{
	if ((hashbitlen != 224) && (hashbitlen != 256) && (hashbitlen != 384) && (hashbitlen != 512))
		return BAD_HASHLEN;

	arirang_gen_tabs();
	// Setting the Hash Length
	arirangState.hashbitlen = hashbitlen;
	
	// Setting the Counter Values
	arirangState.counter[0] = arirangState.counter[1] = arirangState.counter[2] = arirangState.counter[3]= 0;	
	
	// Initialize the Data Length
	arirangState.count[0] = arirangState.count[1] = arirangState.count[2] = arirangState.count[3] = 0;

	// Setting the Initial Hash Value
	if(arirangState.hashbitlen == 224){
		arirangState.workingvar[0] = 0xcbbb9d5d;
		arirangState.workingvar[1] = 0x629a292a;
		arirangState.workingvar[2] = 0x9159015a;
		arirangState.workingvar[3] = 0x152fecd8;
		arirangState.workingvar[4] = 0x67332667;
		arirangState.workingvar[5] = 0x8eb44a87;
		arirangState.workingvar[6] = 0xdb0c2e0d;
		arirangState.workingvar[7] = 0x47b5481d;
		arirangState.blocklen=ARIRANG256_BLOCK_LEN;
	}
		
	else if(arirangState.hashbitlen == 256){
		arirangState.workingvar[0] = 0x6a09e667;
		arirangState.workingvar[1] = 0xbb67ae85;
		arirangState.workingvar[2] = 0x3c6ef372;
		arirangState.workingvar[3] = 0xa54ff53a;
		arirangState.workingvar[4] = 0x510e527f;
		arirangState.workingvar[5] = 0x9b05688c;
		arirangState.workingvar[6] = 0x1f83d9ab;
		arirangState.workingvar[7] = 0x5be0cd19;
		arirangState.blocklen=ARIRANG256_BLOCK_LEN;
	
	}
	else if(hashbitlen == 384){	
		arirangState.workingvar[ 0]=0xcbbb9d5d; arirangState.workingvar[ 1]=0xc1059ed8;
		arirangState.workingvar[ 2]=0x629a292a; arirangState.workingvar[ 3]=0x367cd507;
		arirangState.workingvar[ 4]=0x9159015a; arirangState.workingvar[ 5]=0x3070dd17;	
		arirangState.workingvar[ 6]=0x152fecd8; arirangState.workingvar[ 7]=0xf70e5939;
		arirangState.workingvar[ 8]=0x67332667; arirangState.workingvar[ 9]=0xffc00b31;
		arirangState.workingvar[10]=0x8eb44a87; arirangState.workingvar[11]=0x68581511;
		arirangState.workingvar[12]=0xdb0c2e0d; arirangState.workingvar[13]=0x64f98fa7;
		arirangState.workingvar[14]=0x47b5481d; arirangState.workingvar[15]=0xbefa4fa4;
		arirangState.blocklen=ARIRANG512_BLOCK_LEN;
	
	}													
	else if(hashbitlen == 512){									
		arirangState.workingvar[ 0]=0x6a09e667; arirangState.workingvar[ 1]=0xf3bcc908;
		arirangState.workingvar[ 2]=0xbb67ae85; arirangState.workingvar[ 3]=0x84caa73b;
		arirangState.workingvar[ 4]=0x3c6ef372; arirangState.workingvar[ 5]=0xfe94f82b;
		arirangState.workingvar[ 6]=0xa54ff53a; arirangState.workingvar[ 7]=0x5f1d36f1;
		arirangState.workingvar[ 8]=0x510e527f; arirangState.workingvar[ 9]=0xade682d1;
		arirangState.workingvar[10]=0x9b05688c; arirangState.workingvar[11]=0x2b3e6c1f;
		arirangState.workingvar[12]=0x1f83d9ab; arirangState.workingvar[13]=0xfb41bd6b;
		arirangState.workingvar[14]=0x5be0cd19; arirangState.workingvar[15]=0x137e2179;
		arirangState.blocklen=ARIRANG512_BLOCK_LEN;		
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : Update
//
// DESCRIPTION    : Process the supplied data.
//
// PARAMETERS     : state - a structure that holds the hashState information
//                  data - the data to be hashed
//                  Databitlen - the length, in bits, of the data to be hashed
//
// RETURNS        : SUCCESS - on success
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int Arirang::Update(const BitSequence *data, DataLength databitlen)
{
	unsigned int RemainedLen, PartLen, temp;
	unsigned int databytelen;

	arirangState.remainderbit = databitlen & 7;
	
	databytelen = ( databitlen >> 3) + (arirangState.remainderbit != 0); 
	
	// Compute the number of hashed bytes mod ARIRANG_BLOCK_LEN
	RemainedLen = (arirangState.count[0] >> 3) % arirangState.blocklen;

	// Compute the number of bytes that can be filled up
	PartLen = arirangState.blocklen - RemainedLen;

	// Update count (number of toatl data bits)
	temp = arirangState.count[0] + (databytelen << 3);
	if( temp  < arirangState.count[0] ){
		arirangState.count[ 1]++; if(arirangState.count[ 1] != 0x00);
		else {arirangState.count[ 2]++; if(arirangState.count[ 2] != 0x00); 
		else {arirangState.count[ 3]++;}}
	}
	arirangState.count[0]=temp;
	
	temp = arirangState.count[1] + (databytelen >> 29);
	if( temp  < arirangState.count[1] ){
		arirangState.count[ 2]++; if(arirangState.count[ 1] != 0x00);
		else {arirangState.count[ 3]++;}
	}
	arirangState.count[1]=temp;

	if ((databytelen > PartLen) || ((databytelen == PartLen) && (arirangState.remainderbit == 0)) ) {
		memcpy(arirangState.block + RemainedLen, data, (int)PartLen);
		if(arirangState.hashbitlen <257) Arirang_Compression256(&arirangState, arirangNumRounds);
		else Arirang_Compression512(&arirangState, arirangNumRounds);

		data += PartLen;
		databytelen -= PartLen;
		RemainedLen = 0;

		while( (databytelen > arirangState.blocklen) || ((databytelen == arirangState.blocklen) && (arirangState.remainderbit == 0)) ) {
			memcpy((unsigned char *)arirangState.block, data, (int)arirangState.blocklen);
			if(arirangState.hashbitlen <257) Arirang_Compression256(&arirangState, arirangNumRounds);
			else Arirang_Compression512(&arirangState, arirangNumRounds);

			data += arirangState.blocklen;
			databytelen -= arirangState.blocklen;
		}
	}

	//	Buffer remaining input
	memcpy((unsigned char *)arirangState.block + RemainedLen, data, (int)databytelen);

	return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : Final
//
// DESCRIPTION    : Perform any post processing and ouput filtering required and return the final 
//                  hash value.
//
// PARAMETERS     : state - a structure that holds the hashState information
//                  hashval - the storage for the final hash value to be returned
//
// RETURNS        : SUCCESS - on success
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int Arirang::Final(unsigned char *hashval)
{
	unsigned int i, dwIndex, Count[4];
	unsigned int temp=(arirangState.blocklen >> 3);
	
	// Padding the message
	if(arirangState.remainderbit){
		Count[0] = arirangState.count[0] + arirangState.remainderbit - 8;
		Count[1] = arirangState.count[1];
		Count[2] = arirangState.count[2];
		Count[3] = arirangState.count[3];

		dwIndex = ((Count[0] + (arirangState.blocklen<<3) ) >> 3) % arirangState.blocklen;
		arirangState.block[dwIndex] &= 0xff-(1<<(8-arirangState.remainderbit))+1;		
		arirangState.block[dwIndex++] ^= 0x80>>(arirangState.remainderbit);
	}
	else{
		Count[0] = arirangState.count[0];
		Count[1] = arirangState.count[1];
		Count[2] = arirangState.count[2];
		Count[3] = arirangState.count[3];
		dwIndex = (Count[0] >> 3) % arirangState.blocklen;		
		arirangState.block[dwIndex++] = 0x80;	
	}
		
	if (dwIndex > (arirangState.blocklen - temp)){
		memset((unsigned char *)arirangState.block + dwIndex, 0, (int)(arirangState.blocklen - dwIndex));
		if(arirangState.hashbitlen <257) Arirang_Compression256(&arirangState, arirangNumRounds);
		else Arirang_Compression512(&arirangState, arirangNumRounds);

		memset((unsigned char *)arirangState.block, 0, (int)arirangState.blocklen - temp);
	}
	else
		memset((unsigned char *)arirangState.block + dwIndex, 0, (int)(arirangState.blocklen - dwIndex - temp));

#if defined(ARIRANG_LITTLE_ENDIAN)
	Count[0] = ARIRANG_ENDIAN_REVERSE_DWORD(Count[0]);
	Count[1] = ARIRANG_ENDIAN_REVERSE_DWORD(Count[1]);
	Count[2] = ARIRANG_ENDIAN_REVERSE_DWORD(Count[2]);
	Count[3] = ARIRANG_ENDIAN_REVERSE_DWORD(Count[3]);
#endif
	
	// Fixed counter value for the last message block
	if(arirangState.hashbitlen > 257){
		((unsigned int *)arirangState.block)[arirangState.blocklen/4-4] = Count[3];
		((unsigned int *)arirangState.block)[arirangState.blocklen/4-3] = Count[2];
		((unsigned int *)arirangState.block)[arirangState.blocklen/4-2] = Count[1];
		((unsigned int *)arirangState.block)[arirangState.blocklen/4-1] = Count[0];
		arirangState.counter[3]=0xb7e15162;	arirangState.counter[2]=0x8aed2a6a;
		arirangState.counter[1]=0xbf715880;	arirangState.counter[0]=0x9cf4f3c7;
	}
	else{
		((unsigned int *)arirangState.block)[arirangState.blocklen/4-2] = Count[1];
		((unsigned int *)arirangState.block)[arirangState.blocklen/4-1] = Count[0];
		arirangState.counter[1]=0xb7e15162;	arirangState.counter[0]=0x8aed2a6a;	
	}
	
	if(arirangState.hashbitlen <257) Arirang_Compression256(&arirangState, arirangNumRounds);
	else Arirang_Compression512(&arirangState, arirangNumRounds);

	for (i = 0; i < (arirangState.hashbitlen >> 3); i += 4)
		ARIRANG_BIG_D2B((arirangState.workingvar)[i / 4], &(hashval[i]));

	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : Hash
//
// DESCRIPTION    : Hash the supplied data and provide the resulting hash value. 
//                  
//
// PARAMETERS     : state      - a structure that holds the hashState information
//                  data       - the data to be hashed
//                  databitlen - the length, in bits, of the data to be hashed
//                  hashval    - the storage for the final hash value to be returned
//
// RETURNS        : SUCCESS - on success
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int Arirang::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval){

	//hashState State;

	Arirang::Init(hashbitlen);

	Arirang::Update(data, databitlen);

	Arirang::Final(hashval);

	return SUCCESS;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////