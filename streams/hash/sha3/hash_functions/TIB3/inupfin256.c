/*
 * inupfin256.c
 *
 *		Implements the 224 and 256 bit versions of TibInit(), TibUpdate(), TibFinal() and TibHash()
 *		Optimized for 32 bits
 *		Authors: Penazzi - Montes
 */


#include <string.h>
#include "inupfin.h"

/* Sizes used across all the functions */
#define STATE_WORDS TIB_STATE_WORDS_256
#define STATE_BYTES TIB_STATE_BYTES_256
#define BLOCK_WORDS TIB_BLOCK_WORDS_256
#define BLOCK_BYTES TIB_BLOCK_BYTES_256
#define HASH_BYTES TIB_HASH_BYTES_256
#define BLOCK_BITS 512


/* Updates the bits processed before calling Transform. 
 * The increment is BLOCK_BITS in all the blocks except the last.
 */
#define UPDATEBITS  \
	state->bits_processed[0]+=BLOCK_BITS;\
	if(state->bits_processed[0]<BLOCK_BITS) \
		state->bits_processed[1]++;

 /* Expansion */

/* Processes 8 32 bits words, and returns 2.
 * Used by EXPANSION
 * Input words:
 *	wi20,wi19,wi16,wi15,wi6,wi5,wi4,wi3
 * Output words:
 *	wi,wim1
 */
#define PSI(wi,wim1,wi20,wi19,wi16,wi15,wi6,wi5,wi4,wi3) \
      wi=wi20^wi16^wi6^wi3;\
      wim1=((wi5+wi4)^wi19^wi15)+wi+(wi<<11);\
      wi^=(wim1>>7);

/* Expands 16 32 bit words to 64 words
 * Input:
 *	W: an array of 32 bit words, with the first 16 already filled
 *	pk: the previous block
 *	bits: array of 2 elements, containing the bits processed, including this block
 * Output:
 *	W: the array with the 64 words filled
 */
#define EXPANSION(W,pk,bits){ \
	int i;\
	PSI(W[16],W[17],(W[6]^pk[0]),(W[7]^pk[1]),(W[8]^pk[2]),(W[9]^pk[3]),(W[10]^pk[4]),(W[11]^pk[5]),(W[2]^pk[6]),(W[3]^pk[7]));\
	PSI(W[18],W[19],(W[4]^0xd728ae22^pk[8]),(W[5]^0x428a2f98^pk[9]),(W[14]^bits[0]^pk[10]),(W[15]^bits[1]^pk[11]),(W[12]^pk[14]),(W[13]^pk[15]),(W[0]^pk[12]),(W[1]^pk[13]));\
	for( i = 20; i!=64; i=i+2) {\
		PSI(W[i],W[i+1],W[i-20],W[i-19],W[i-16],W[i-15],W[i-6],W[i-5],W[i-4],W[i-3]);\
	}\
}


/* Macros used in the round function */
#define F(X,Y,Z) ((~X)^(Y&(~Z)))
#define G(X,Y,Z) (Z^((~X)&(~Y)))
#define H(X,Y,Z) (Y^(X&Z))


/*Sbox in bitslice mode */
#define Sbox(m0,m1,m2) \
  { register unsigned int temp0, temp1;\
		temp0=F(m0,m1,m2);\
        temp1=G(m0,m1,m2);\
        m2=H(m0,m1,m2);\
        m0=temp0;\
		m1=temp1; }



#define PHTX(L,H)  \
     H+=L+(L<<15);\
	 L^=H^(H>>11);


#define DIFFUSION(A,B,C,D,E,F,G,H) \
	A+=G;\
	B+=H;\
	PHTX(C,D);\
	G+=E;\
	H+=F;


/* Round function
 * Input:
 *	A,B,C,D,E,F,G,H: state, as 8 32 bit words
 *	k0,k1,k2,k3,k4,k5: round key, as 6 32 bit words.
 * Output:
 *	A,B,C,D,E,F,G,H: state, modified by the S-Box
 */
#define ROUND(A,B,C,D,E,F,G,H,k0,k1,k2,k3,k4,k5) \
    A^=k0;\
	B^=k1;\
	C^=k2;\
	D^=k3;\
	E^=k4;\
    F^=k5;\
	G^=C;\
	H^=D;\
	PHTX(G,H);\
    Sbox(A,C,E) ;\
	Sbox(B,D,F) ;\
    DIFFUSION(A,B,C,D,E,F,G,H);


/* Calls the round function 4 times, rotating the words of the state,
 * and changing the round key.
 * Input:
 *	k0,k1,k2,k3,k4,k5,k6,k7,k8,k9,k10,k11,k12,k13,k14,k15,k16,k17,k18,k19,k20,k21,k22,k23: key material
 *	h0,h1,h2,h3,h4,h5,h6,h7: 256 bits state
 * Output:
 *	h0,h1,h2,h3,h4,h5,h6,h7: state, modified by the 4 round functions.
 */
#define FOURROUNDSBOX(k0,k1,k2,k3,k4,k5,k6,k7,k8,k9,k10,k11,k12,k13,k14,k15,k16,k17,k18,k19,k20,k21,k22,k23,h0,h1,h2,h3,h4,h5,h6,h7,rr)\
		if (rr >= 1) ROUND(h0,h1,h2,h3,h4,h5,h6,h7, k0, k1, k2, k3, k4, k5);\
		if (rr >= 2) ROUND(h2,h3,h4,h5,h6,h7,h0,h1, k6, k7, k8, k9,k10,k11);\
		if (rr >= 3) ROUND(h4,h5,h6,h7,h0,h1,h2,h3,k12,k13,k14,k15,k16,k17);\
		if (rr >= 4) ROUND(h6,h7,h0,h1,h2,h3,h4,h5,k18,k19,k20,k21,k22,k23);


void Transform256(tibHashState256* state, const int rounds);

/* Bitmask for zeroing the unused bits of the last byte of the message */
const static unsigned char BITMASK[] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
/* Bitmask for adding a 1 after the last bit of the message */
const static unsigned char BITPADDING[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

/* IV for the 256 bit version */
const unsigned int i256[STATE_WORDS] =
{
	0xf3bcc908, 0x6a09e667, 0x84caa73b, 0xbb67ae85,
	0xfe94f82b, 0x3c6ef372, 0x5f1d36f1, 0xa54ff53a
};

/* IV for the 224 bit version */
const unsigned int  i224[STATE_WORDS] =
{
	0xade682d1, 0x510e527f, 0x2b3e6c1f, 0x9b05688c,
	0xfb41bd6b, 0x1f83d9ab, 0x137e2179, 0x5be0cd19
};




/* Initializes a tibHashState256 with the appropriate values for the 256 bit hash.
 * Parameters:
 *	state: a structure that holds the tibHashState256 information
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibInit256(tibHashState256 *state){
	state->previous_block = state->buffer; /* previous_block points to the first half of the buffer */
	state->data_block = state->buffer+BLOCK_WORDS; /* data_block points to the second half of the buffer */
	state->bits_processed[0] = state->bits_processed[1] = 0;  /* No bits, yet */
	state->bits_waiting_for_process = 0;	/* No bits waiting */
	memcpy(state->state, i256, STATE_BYTES); /* Copying the IV */
	memcpy(state->previous_block, i224, STATE_BYTES); /* Initial state of the previous block. We copy the 224 bit IV, twice */
	memcpy(state->previous_block+STATE_WORDS, i224, STATE_BYTES);
	return TIB_SUCCESS;
}

/* Initializes a hashState224 with the appropriate values for the 224 bit hash.
 * Parameters:
 *	state: a structure that holds the hashState224 information
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibInit224(tibHashState256 *state){
	state->previous_block = state->buffer;/* previous_block points to the first half of the buffer */
	state->data_block = state->buffer+BLOCK_WORDS; /* data_block points to the second half of the buffer */
	state->bits_processed[0] = state->bits_processed[1] = 0;  /* No bits, yet */
	state->bits_waiting_for_process = 0;	/* No bits waiting */
	memcpy(state->state, i224, STATE_BYTES); /* Copying the IV */
	memcpy(state->previous_block, i256, STATE_BYTES);/* Initial state of the previous block. We copy the 256 bit IV, twice */
	memcpy(state->previous_block+STATE_WORDS, i256, STATE_BYTES);
	return TIB_SUCCESS;
}



TibHashReturn TibUpdate256(tibHashState256 *state, const TibBitSequence *data,
		TibDataLength databitlen, const int rounds) {
	TibDataLength offset, databytes;
	unsigned int remaining, index;
	TibBitSequence* byte_buffer;
	databytes = (databitlen >> 3); /* number of bytes to process */

	/*bytes waiting to be processed
	 * According to specifications, except for the last time, update will never be called with
	 * a number of bits not multiple of 8. Therefore, the number of bits that have been
	 * fed into the buffer but not processed after the last call of update must be
	 * a multiple of 8. Also, this number should be less than BLOCK_BYTES
	 */
	index = (state->bits_waiting_for_process >> 3);

	remaining = BLOCK_BYTES - index; /* space available in the buffer */
	offset = 0;

	/* If we have bytes waiting from a previous call, and there are enough
	 * to fill a block, we copy them and call to Transform
	 */
	if ((index != 0) && (databytes >= remaining)) {
		byte_buffer = (TibBitSequence*) state->data_block; /* buffer as an array of char */
		memcpy(byte_buffer + index, data, remaining); /* We fill the buffer */
		UPDATEBITS;
		Transform256(state, rounds);
		offset += remaining; /* We consumed 'remaining' bytes from data */
		index = 0; /* There are not bytes waiting for process */
		state->bits_waiting_for_process = 0; /* Therefore, no bits */
	}

	/* We process as many blocks as we can */
	while (offset + BLOCK_BYTES <= databytes) {
		memcpy(state->data_block, data + offset, BLOCK_BYTES);
		UPDATEBITS;
		Transform256(state, rounds);
		offset += BLOCK_BYTES;
	}
	if (databitlen & 7)
		databytes++; /* The number of bits is not a multiple of 8. This can only happen in the last call to update */
	byte_buffer = (TibBitSequence*) state->data_block;

	/* If there are still bytes in data, we copy them to the buffer */
	memcpy(byte_buffer + index, data + offset,(size_t) (databytes - offset));
	state->bits_waiting_for_process += (unsigned int) (databitlen - 8 * offset);;
	return TIB_SUCCESS;
}


/* Process the block stored in the buffer. 
 * Parameters:
 *	state: a structure that holds the tibHashState256 information
 * This optimized version also implements the functionality of the 
 * function Encrypt256() used in the Reference Implementation.
 */
void Transform256(tibHashState256* state, const int rounds) {
    unsigned int *k, *pk; /* Pointers for storing the block and the previous block */
    unsigned int H0, H1, H2, H3, H4, H5, H6, H7; /* 8 32-bit words for storing the state */
	unsigned int W[64]; /* Array for the expanded key */
	int i;
	k = state->data_block;
	pk = state->previous_block;

	/*expansion*/
	W[0]  = pk[0]  ^ k[0];
	W[1]  = pk[1]  ^ k[1];
	W[2]  = pk[2]  ^ k[2];
	W[3]  = pk[3]  ^ k[3];
	W[4]  = pk[4]  ^ k[4];
	W[5]  = pk[5]  ^ k[5];
	W[6]  = pk[6]  ^ k[6];
	W[7]  = pk[7]  ^ k[7];
	W[8]  = pk[8]  ^ k[8];
	W[9]  = pk[9]  ^ k[9];
	W[10] = pk[10] ^ k[10];
	W[11] = pk[11] ^ k[11];
	W[12] = pk[12] ^ k[12];
	W[13] = pk[13] ^ k[13];
	W[14] = pk[14] ^ k[14];
	W[15] = pk[15] ^ k[15];


	/* Expand the first 16 words of W to 64 words */
	PSI(W[16],W[17],(W[6]^pk[0]),(W[7]^pk[1]),(W[8]^pk[2]),(W[9]^pk[3]),(W[10]^pk[4]),(W[11]^pk[5]),(W[2]^pk[6]),(W[3]^pk[7]));
	PSI(W[18],W[19],(W[4]^0xd728ae22^pk[8]),(W[5]^0x428a2f98^pk[9]),(W[14]^state->bits_processed[0]^pk[10]),(W[15]^state->bits_processed[1]^pk[11]),(W[12]^pk[14]),(W[13]^pk[15]),(W[0]^pk[12]),(W[1]^pk[13]));
	for( i = 20; i!=64; i=i+2) {
		PSI(W[i],W[i+1],W[i-20],W[i-19],W[i-16],W[i-15],W[i-6],W[i-5],W[i-4],W[i-3]);
	}

	/* Now, we copy the state to the 8 words  H0, H1, H2, H3, H4, H5, H6, H7 */
	H0 = state->state[0];
	H1 = state->state[1];
	H2 = state->state[2];
	H3 = state->state[3];
	H4 = state->state[4];
	H5 = state->state[5];
	H6 = state->state[6];
	H7 = state->state[7];

	/* Now, the 16 rounds */
	if ((rounds >= 1) && (rounds <= 4)) FOURROUNDSBOX(W[0],W[1],k[0],k[1],W[2],W[3],W[4],W[5],k[2],k[3],W[6],W[7],W[8],W[9],k[4],k[5],W[10],W[11],W[12],W[13],k[6],k[7],W[14],W[15],H0,H1,H2,H3,H4,H5,H6,H7,rounds);
	if ((rounds >= 5) && (rounds <= 8)) {
		FOURROUNDSBOX(W[0],W[1],k[0],k[1],W[2],W[3],W[4],W[5],k[2],k[3],W[6],W[7],W[8],W[9],k[4],k[5],W[10],W[11],W[12],W[13],k[6],k[7],W[14],W[15],H0,H1,H2,H3,H4,H5,H6,H7,4);
		FOURROUNDSBOX(W[16],W[17],k[8],k[9],W[18],W[19],W[20],W[21],k[10],k[11],W[22],W[23],W[24],W[25],k[12],k[13],W[26],W[27],W[28],W[29],k[14],k[15],W[30],W[31],H0,H1,H2,H3,H4,H5,H6,H7,(rounds-4));
	}
	if ((rounds >= 9) && (rounds <= 12)) {
		FOURROUNDSBOX(W[0],W[1],k[0],k[1],W[2],W[3],W[4],W[5],k[2],k[3],W[6],W[7],W[8],W[9],k[4],k[5],W[10],W[11],W[12],W[13],k[6],k[7],W[14],W[15],H0,H1,H2,H3,H4,H5,H6,H7,4);
		FOURROUNDSBOX(W[16],W[17],k[8],k[9],W[18],W[19],W[20],W[21],k[10],k[11],W[22],W[23],W[24],W[25],k[12],k[13],W[26],W[27],W[28],W[29],k[14],k[15],W[30],W[31],H0,H1,H2,H3,H4,H5,H6,H7,4);
		FOURROUNDSBOX(pk[0],pk[1],W[32],W[33],pk[2],pk[3],pk[4],pk[5],W[34],W[35],pk[6],pk[7],pk[8],pk[9],W[36],W[37],pk[10],pk[11],pk[12],pk[13],W[38],W[39],pk[14],pk[15],H0,H1,H2,H3,H4,H5,H6,H7,(rounds-8));
	}
	if (rounds >= 13) {
		FOURROUNDSBOX(W[0],W[1],k[0],k[1],W[2],W[3],W[4],W[5],k[2],k[3],W[6],W[7],W[8],W[9],k[4],k[5],W[10],W[11],W[12],W[13],k[6],k[7],W[14],W[15],H0,H1,H2,H3,H4,H5,H6,H7,4);
		FOURROUNDSBOX(W[16],W[17],k[8],k[9],W[18],W[19],W[20],W[21],k[10],k[11],W[22],W[23],W[24],W[25],k[12],k[13],W[26],W[27],W[28],W[29],k[14],k[15],W[30],W[31],H0,H1,H2,H3,H4,H5,H6,H7,4);
		FOURROUNDSBOX(pk[0],pk[1],W[32],W[33],pk[2],pk[3],pk[4],pk[5],W[34],W[35],pk[6],pk[7],pk[8],pk[9],W[36],W[37],pk[10],pk[11],pk[12],pk[13],W[38],W[39],pk[14],pk[15],H0,H1,H2,H3,H4,H5,H6,H7,4);
		FOURROUNDSBOX(W[40],W[41],W[42],W[43],W[44],W[45],W[46],W[47],W[48],W[49],W[50],W[51],W[52],W[53],W[54],W[55],W[56],W[57],W[58],W[59],W[60],W[61],W[62],W[63],H0,H1,H2,H3,H4,H5,H6,H7,(rounds-12));
	}

	/* We update the state, xoring with the encrypted state */
	state->state[0] ^= H0;
	state->state[1] ^= H1;
	state->state[2] ^= H2;
	state->state[3] ^= H3;
	state->state[4] ^= H4;
	state->state[5] ^= H5;
	state->state[6] ^= H6;
	state->state[7] ^= H7;

	/* Finally, we update the pointers to previous_block and data_block */
	state->previous_block = k;
	state->data_block = pk;
}






/* Perform post processing and output filtering and return the final hash value.
 * This function is called from TibFinal256() or TibFinal224()
 * Parameters:
 *	state: a structure that holds the tibHashState256 information
 *	hashval: the storage for the final hash value to be returned
 *	hashbytes: the number of bytes to store in hashval
 *		(28 for the 224 bit hash, 32 for the 256 bit hash)
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn _Final256 (tibHashState256 *state, TibBitSequence *hashval, int hashbytes, const int rounds){
	int j;
	TibBitSequence* byte_buffer;
	unsigned int index, bits;

	/* if there is any data to be processed, we need to pad and process, 
	 * otherwise we	go to the last block*/
	if(state->bits_waiting_for_process){
		/* If bits_waiting_for_process is not a multiple of 8, index will be the last byte,
		 * and bits will be the number of valid bits in that byte.
		 */
		index = state->bits_waiting_for_process >> 3;
		bits =  state->bits_waiting_for_process & 7;
		byte_buffer = (TibBitSequence*) state->data_block;

		/* We zero the unused bits of the last byte, and set the fist unused bit to 1 */
		byte_buffer[index] = (byte_buffer[index] & BITMASK[bits]) | BITPADDING[bits];
		index++;
		memset(byte_buffer+index, 0, BLOCK_BYTES - index); /* We fill the rest of the block with zeros */

		/*the usual update of bits processed cannot be used here, since
		 *less than a full block of DATA has been processed
		 */
		state->bits_processed[0] += state->bits_waiting_for_process; 
		if(state->bits_processed[0]<state->bits_waiting_for_process) \
			state->bits_processed[1]++;

		Transform256(state, rounds);
	}/*endif(bits_waiting_for_process)*/
   
	/* All data (plus any padding) has been processed. A last call, so that the last block
	 * of data is treated like all the others (i.e., twice) is made. The new block is simply the number of
	 * bits processed, xor the last state. 
	 */
	state->data_block[0]=state->bits_processed[0];
	state->data_block[1]=state->bits_processed[1];
	for(j=2;j < STATE_WORDS;j++) state->data_block[j]=0; /* The size of the block is twice the size of the state */
	memcpy(state->data_block + STATE_WORDS, state->state, STATE_BYTES);
	state->bits_processed[0] = state->bits_processed[1] = 0; /* This ensures that the final transform is the same regardless of the bits processed */
	
	Transform256(state, rounds);
	
	/* We output the appropriate number of bytes, truncating the state if necessary */
	for (j = 0; j < hashbytes; j++)
		hashval[j] = ((TibBitSequence*)state->state)[j];
	return TIB_SUCCESS;
}

/* Perform post processing and output filtering and return the final 256 bit hash value.
 * Parameters:
 *	state: a structure that holds the tibHashState256 information
 *	hashval: the storage for the final hash value to be returned
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibFinal256(tibHashState256 *state, TibBitSequence *hashval, const int rounds){
	return _Final256(state,hashval,TIB_HASH_BYTES_256, rounds);
}

/* Perform post processing and output filtering and return the final 224 bit hash value.
 * Parameters:
 *	state: a structure that holds the tibHashState256 information
 *	hashval: the storage for the final hash value to be returned
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibFinal224(tibHashState256 *state, TibBitSequence *hashval, const int rounds){
	return _Final256(state,hashval,TIB_HASH_BYTES_224, rounds);
}





/* TibHash the supplied data and provide the resulting hash value. Set return code as
 * appropriate. 256 bit version.
 * Parameters:
 *	data: the data to be hashed
 *	databitlen: the length, in bits, of the data to be hashed
 *	hashval: the resulting hash value of the provided data
 * Returns:
 *	TIB_SUCCESS - on success
*/
TibHashReturn TibHash256(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds){
	tibHashState256 state;
	TibHashReturn status;
	status = TibInit256(&state);
	if (status != TIB_SUCCESS) return status;
	status = TibUpdate256(&state, data, databitlen, rounds);
	if (status != TIB_SUCCESS) return status;
	return _Final256(&state, hashval,TIB_HASH_BYTES_256, rounds);
}

/* TibHash the supplied data and provide the resulting hash value. Set return code as
 * appropriate. 224 bit version
 * Parameters:
 *	data: the data to be hashed
 *	databitlen: the length, in bits, of the data to be hashed
 *	hashval: the resulting hash value of the provided data
 * Returns:
 *	TIB_SUCCESS - on success
*/
TibHashReturn TibHash224(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds){
	tibHashState256 state;
	TibHashReturn status;
	status = TibInit224(&state);
	if (status != TIB_SUCCESS) return status;
	status = TibUpdate256(&state, data, databitlen, rounds);
	if (status != TIB_SUCCESS) return status;
	return _Final256(&state, hashval,TIB_HASH_BYTES_224, rounds);
}



