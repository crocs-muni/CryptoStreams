/*
 * inupfin512.c
 *
 *		Implements the 384 and 512 bit versions of TibInit(), TibUpdate(), TibFinal() and TibHash()
 *		Optimized for 32 bits
 *		Authors: Penazzi - Montes
 */


#include <string.h>
#include "inupfin.h"

/* Sizes used across all the functions */
#define STATE_WORDS TIB_STATE_WORDS_512
#define STATE_BYTES TIB_STATE_BYTES_512
#define BLOCK_WORDS TIB_BLOCK_WORDS_512
#define BLOCK_BYTES TIB_BLOCK_BYTES_512
#define HASH_BYTES TIB_HASH_BYTES_512
#define BLOCK_BITS 1024

/* Updates the bits processed before calling Transform. 
 * The increment is BLOCK_BITS in all the blocks except the last.
 */
#define UPDATEBITS  \
	state->bits_processed[0]+=BLOCK_BITS;\
	if(state->bits_processed[0]<BLOCK_BITS) \
		state->bits_processed[1]++;


/* Expansion */

/* Processes 16 32 bits words, and returns 4.
 * Used by the key expansion
 * Input words:
 *	wi20L,wi20H,wi19L,wi19H,wi16L,wi16H,wi15L,wi15H,wi6L,wi6H,wi5L,wi5H,wi4L,wi4H,wi3L,wi3H
 * Output words:
 *	wiL,wiH,wim1L,wim1H
 */
#define PHI(wiL,wiH,wim1L,wim1H,wi20L,wi20H,wi19L,wi19H,wi16L,wi16H,wi15L,wi15H,wi6L,wi6H,wi5L,wi5H,wi4L,wi4H,wi3L,wi3H) \
	{ register unsigned int wi54L,wiL23;\
	wiL=wi20L^wi16L^wi6L^wi3L;\
	wiH=wi20H^wi16H^wi6H^wi3H;\
        wi54L = wi5L+wi4L; \
        wiL23 = wiL+(wiL<<23); \
	wim1L=(wi54L^wi19L^wi15L)+wiL23;\
	wim1H= ((wi5H+wi4H+(wi54L<wi5L))^wi19H^wi15H) + wiH+(wiH<<23|wiL>>9)+(wiL23<wiL) + (wim1L<wiL23);\
	wiL^=wim1L>>15|wim1H<<17;\
        wiH^=wim1H>>15;}

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

#define PHTX(DL, DH)  \
	DH+=DL+(DL<<15);\
	DL^=DH^(DH>>11);

#define PHTXD(LL,LH,HL,HH)  \
	HL^=LL; HH^=LH;\
	PHTX(HL,HH);\
	LL^=HL; LH^=HH;\
	PHTX(LL,LH);

#define DIFFUSION(AL,AH,BL,BH,CL,CH,DL,DH,EL,EH,FL,FH,GL,GH,HL,HH) \
	AL+=GL; AH+=GH+(AL<GL);\
	BL+=HL; BH+=HH+(BL<HL);\
	PHTXD(CL,CH,DL,DH);\
	GL+=EL; GH+=EH+(GL<EL);\
	HL+=FL; HH+=FH+(HL<FL);

/* Round function
 * Input:
 *	AL,AH,BL,BH,CL,CH,DL,DH,EL,EH,FL,FH,GL,GH,HL,HH: state, as 16 32 bit words
 *	k0L,k0H,k1L,k1H,k2L,k2H,k3L,k3H,k4L,k4H,k5L,k5H: round key, as 12 32 bit words.
 * Output:
 *	AL,AH,BL,BH,CL,CH,DL,DH,EL,EH,FL,FH,GL,GH,HL,HH: state, modified by the S-Box
 */
#define ROUND(AL,AH,BL,BH,CL,CH,DL,DH,EL,EH,FL,FH,GL,GH,HL,HH,k0L,k0H,k1L,k1H,k2L,k2H,k3L,k3H,k4L,k4H,k5L,k5H) \
	AL^=k0L; AH^=k0H;\
	BL^=k1L; BH^=k1H;\
	CL^=k2L; CH^=k2H;\
	DL^=k3L; DH^=k3H;\
	EL^=k4L; EH^=k4H;\
	FL^=k5L; FH^=k5H;\
	GL^=CL; GH^=CH;\
	HL^=DL; HH^=DH;\
	PHTXD(GL,GH,HL,HH);\
	Sbox(AL,CL,EL) ;\
	Sbox(BL,DL,FL) ;\
	Sbox(AH,CH,EH) ;\
	Sbox(BH,DH,FH) ;\
	DIFFUSION(AL,AH,BL,BH,CL,CH,DL,DH,EL,EH,FL,FH,GL,GH,HL,HH);

void Transform512(tibHashState512* state, const int rounds);

/* Bitmask for zeroing the unused bits of the last byte of the message */
const static unsigned char BITMASK[] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
/* Bitmask for adding a 1 after the last bit of the message */
const static unsigned char BITPADDING[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

/* IV for the 512 bit version */
const unsigned int  i512[STATE_WORDS] =  
{
	0xf3bcc908, 0x6a09e667, 0x84caa73b, 0xbb67ae85,
	0xfe94f82b, 0x3c6ef372, 0x5f1d36f1, 0xa54ff53a,
	0xade682d1, 0x510e527f, 0x2b3e6c1f, 0x9b05688c,
	0xfb41bd6b, 0x1f83d9ab, 0x137e2179, 0x5be0cd19
};

/* IV for the 384 bit version */
const unsigned int  i384[STATE_WORDS] =
{
	0xade682d1, 0x510e527f, 0x2b3e6c1f, 0x9b05688c,
	0xfb41bd6b, 0x1f83d9ab, 0x137e2179, 0x5be0cd19,
	0xf3bcc908, 0x6a09e667, 0x84caa73b, 0xbb67ae85,
	0xfe94f82b, 0x3c6ef372, 0x5f1d36f1, 0xa54ff53a
};

/* Initializes a tibHashState512 with the appropriate values for the 512 bit hash.
 * Parameters:
 *	state: a structure that holds the tibHashState512 information
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibInit512(tibHashState512 *state){
	state->previous_block = state->buffer; /* previous_block points to the first half of the buffer */
	state->data_block = state->buffer+BLOCK_WORDS; /* data_block points to the second half of the buffer */
	state->bits_processed[0] = state->bits_processed[1] = 0;  /* No bits, yet */
	state->bits_waiting_for_process = 0;	/* No bits waiting */
	memcpy(state->state, i512, STATE_BYTES); /* Copying the IV */
	memcpy(state->previous_block, i384, STATE_BYTES); /* Initial state of the previous block. We copy the 384 bit IV, twice */
	memcpy(state->previous_block+STATE_WORDS, i384, STATE_BYTES);
	return TIB_SUCCESS;
}

/* Initializes a hashState384 with the appropriate values for the 384 bit hash.
 * Parameters:
 *	state: a structure that holds the hashState384 information
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibInit384(tibHashState512 *state){
	state->previous_block = state->buffer;/* previous_block points to the first half of the buffer */
	state->data_block = state->buffer+BLOCK_WORDS; /* data_block points to the second half of the buffer */
	state->bits_processed[0] = state->bits_processed[1] = 0;  /* No bits, yet */
	state->bits_waiting_for_process = 0;	/* No bits waiting */
	memcpy(state->state, i384, STATE_BYTES); /* Copying the IV */
	memcpy(state->previous_block, i512, STATE_BYTES);/* Initial state of the previous block. We copy the 512 bit IV, twice */
	memcpy(state->previous_block+STATE_WORDS, i512, STATE_BYTES);
	return TIB_SUCCESS;
}




TibHashReturn TibUpdate512(tibHashState512 *state, const TibBitSequence *data,
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
		Transform512(state, rounds);
		offset += remaining; /* We consumed 'remaining' bytes from data */
		index = 0; /* There are not bytes waiting for process */
		state->bits_waiting_for_process = 0; /* Therefore, no bits */
	}

	/* We process as many blocks as we can */
	while (offset + BLOCK_BYTES <= databytes) {
		memcpy(state->data_block, data + offset, BLOCK_BYTES);
		UPDATEBITS;
		Transform512(state, rounds);
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


/* Encrypt function. Encrypts the state, using the block, the previous block 
 * and the bits processed as key material. Modifies the state in place.
 * This version differs from the one in the reference implementation: it copies
 * the state to local variables, and performs the xor between the state and the
 * encrypted state before returning.
 * Input:
 *	hsh: Array of 16 32-bit words, storing the state.
 *	k: Array of 32 32-bit words, storing the current block.
 *	pk: Array of 32 32-bit words, storing the previous block.
 *	bits: Array of 2 32-bit words representing the bits processed, up to and including the current block.
 * Output:
 *	hsh: the state, encrypted.
 */
void Encrypt512(unsigned int hsh[16], unsigned int k[32], unsigned int pk[32],
		unsigned int bits[2], const int rounds) {
	unsigned int W[128]; /* Array for the expanded key */
	unsigned int H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15; /* 32 bit words for holding the state */
	int i;

	/*expansion*/
	W[0] = pk[0] ^ k[0];
	for (i = 31; i; i--)
		W[i] = pk[i] ^ k[i];
	/* Expand the first 32 words of W to 128 words */
	PHI(W[32],W[33],W[34],W[35],(W[12]^pk[0]),(W[13]^pk[1]),(W[14]^pk[2]),(W[15]^pk[3]),(W[16]^pk[4]),(W[17]^pk[5]),(W[18]^pk[6]),(W[19]^pk[7]),(W[20]^pk[8]),(W[21]^pk[9]),(W[22]^pk[10]),(W[23]^pk[11]),(W[4]^pk[12]),(W[5]^pk[13]),(W[6]^pk[14]),(W[7]^pk[15]));
	PHI(W[36],W[37],W[38],W[39],(W[8]^0xd728ae22^pk[16]),(W[9]^0x428a2f98^pk[17]),(W[10]^pk[18]),(W[11]^pk[19]),(W[28]^bits[0]^pk[20]),(W[29]^bits[1]^pk[21]),(W[30]^pk[22]),(W[31]^pk[23]),(W[24]^pk[28]),(W[25]^pk[29]),(W[26]^pk[30]),(W[27]^pk[31]),(W[0]^pk[24]),(W[1]^pk[25]),(W[2]^pk[26]),(W[3]^pk[27]));
	for( i = 40; i!=128; i+=4) {
		PHI(W[i],W[i+1],W[i+2],W[i+3],W[i-40],W[i-39],W[i-38],W[i-37],W[i-32],W[i-31],W[i-30],W[i-29],W[i-12],W[i-11],W[i-10],W[i-9],W[i-8],W[i-7],W[i-6],W[i-5]);
	}


	/* Now, we copy the state to the 16 words H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15 */
    H0 = hsh[0];
    H1 = hsh[1];
    H2 = hsh[2];
    H3 = hsh[3];
    H4 = hsh[4];
    H5 = hsh[5];
    H6 = hsh[6];
    H7 = hsh[7];
    H8 = hsh[8];
    H9 = hsh[9];
    H10 = hsh[10];
    H11 = hsh[11];
    H12 = hsh[12];
    H13 = hsh[13];
    H14 = hsh[14];
    H15 = hsh[15];

	/* Now, the 16 rounds */
    if (rounds >= 1) ROUND(H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,W[0],W[1],W[2],W[3],k[0],k[1],k[2],k[3],W[4],W[5],W[6],W[7]);
    if (rounds >= 2) ROUND(H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,W[8],W[9],W[10],W[11],k[4],k[5],k[6],k[7],W[12],W[13],W[14],W[15]);
    if (rounds >= 3) ROUND(H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,W[16],W[17],W[18],W[19],k[8],k[9],k[10],k[11],W[20],W[21],W[22],W[23]);
    if (rounds >= 4) ROUND(H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,W[24],W[25],W[26],W[27],k[12],k[13],k[14],k[15],W[28],W[29],W[30],W[31]);

	if (rounds >= 5) ROUND(H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,W[32],W[33],W[34],W[35],k[16],k[17],k[18],k[19],W[36],W[37],W[38],W[39]);
    if (rounds >= 6) ROUND(H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,W[40],W[41],W[42],W[43],k[20],k[21],k[22],k[23],W[44],W[45],W[46],W[47]);
    if (rounds >= 7) ROUND(H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,W[48],W[49],W[50],W[51],k[24],k[25],k[26],k[27],W[52],W[53],W[54],W[55]);
    if (rounds >= 8) ROUND(H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,W[56],W[57],W[58],W[59],k[28],k[29],k[30],k[31],W[60],W[61],W[62],W[63]);

	if (rounds >= 9) ROUND(H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,pk[0],pk[1],pk[2],pk[3],W[64],W[65],W[66],W[67],pk[4],pk[5],pk[6],pk[7]);
    if (rounds >= 10) ROUND(H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,pk[8],pk[9],pk[10],pk[11],W[68],W[69],W[70],W[71],pk[12],pk[13],pk[14],pk[15]);
    if (rounds >= 11) ROUND(H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,pk[16],pk[17],pk[18],pk[19],W[72],W[73],W[74],W[75],pk[20],pk[21],pk[22],pk[23]);
    if (rounds >= 12) ROUND(H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,pk[24],pk[25],pk[26],pk[27],W[76],W[77],W[78],W[79],pk[28],pk[29],pk[30],pk[31]);

	if (rounds >= 13) ROUND(H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,W[80],W[81],W[82],W[83],W[84],W[85],W[86],W[87],W[88],W[89],W[90],W[91]);
    if (rounds >= 14) ROUND(H4,H5,H6,H7,H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,W[92],W[93],W[94],W[95],W[96],W[97],W[98],W[99],W[100],W[101],W[102],W[103]);
    if (rounds >= 15) ROUND(H8,H9,H10,H11,H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,W[104],W[105],W[106],W[107],W[108],W[109],W[110],W[111],W[112],W[113],W[114],W[115]);
    if (rounds >= 16) ROUND(H12,H13,H14,H15,H0,H1,H2,H3,H4,H5,H6,H7,H8,H9,H10,H11,W[116],W[117],W[118],W[119],W[120],W[121],W[122],W[123],W[124],W[125],W[126],W[127]);

	/* We update the state, xoring with the encrypted state */
    hsh[0]  ^= H0;
    hsh[1]  ^= H1;
    hsh[2]  ^= H2;
    hsh[3]  ^= H3;
    hsh[4]  ^= H4;
    hsh[5]  ^= H5;
    hsh[6]  ^= H6;
    hsh[7]  ^= H7;
    hsh[8]  ^= H8;
    hsh[9]  ^= H9;
    hsh[10] ^= H10;
    hsh[11] ^= H11;
    hsh[12] ^= H12;
    hsh[13] ^= H13;
    hsh[14] ^= H14;
    hsh[15] ^= H15;

}



/* Process the block stored in the buffer. This version differs from the one
 * in the reference implementation. The copy of the state and the xor are
 * now in the Encrypt512 function.
 * Parameters:
 *	state: a structure that holds the tibHashState512 information
 */
void Transform512(tibHashState512* state, const int rounds){
	unsigned int* swap_aux;

	Encrypt512(state->state, state->data_block, state->previous_block,state->bits_processed, rounds);

	/* Finally, we swap pointers */
	swap_aux = state->data_block;
	state->data_block = state->previous_block;
	state->previous_block = swap_aux; /* now previous_block points to the block just processed */
}


/* Perform post processing and output filtering and return the final hash value.
 * This function is called from TibFinal512() or TibFinal384()
 * Parameters:
 *	state: a structure that holds the tibHashState512 information
 *	hashval: the storage for the final hash value to be returned
 *	hashbytes: the number of bytes to store in hashval
 *		(48 for the 384 bit hash, 64 for the 512 bit hash)
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn _Final512 (tibHashState512 *state, TibBitSequence *hashval, int hashbytes, const int rounds){
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

		Transform512(state, rounds);
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
	
	Transform512(state, rounds);
	
	/* We output the appropriate number of bytes, truncating the state if necessary */
	for (j = 0; j < hashbytes; j++)
		hashval[j] = ((TibBitSequence*)state->state)[j];
	return TIB_SUCCESS;
}


/* Perform post processing and output filtering and return the final 512 bit hash value.
 * Parameters:
 *	state: a structure that holds the tibHashState512 information
 *	hashval: the storage for the final hash value to be returned
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibFinal512(tibHashState512 *state, TibBitSequence *hashval, const int rounds){
	return _Final512(state,hashval,TIB_HASH_BYTES_512, rounds);
}

/* Perform post processing and output filtering and return the final 384 bit hash value.
 * Parameters:
 *	state: a structure that holds the tibHashState512 information
 *	hashval: the storage for the final hash value to be returned
 * Returns:
 *	TIB_SUCCESS - on success
 */
TibHashReturn TibFinal384(tibHashState512 *state, TibBitSequence *hashval, const int rounds){
	return _Final512(state,hashval,TIB_HASH_BYTES_384, rounds);
}


/* TibHash the supplied data and provide the resulting hash value. Set return code as
 * appropriate. 512 bit version.
 * Parameters:
 *	data: the data to be hashed
 *	databitlen: the length, in bits, of the data to be hashed
 *	hashval: the resulting hash value of the provided data
 * Returns:
 *	TIB_SUCCESS - on success
*/
TibHashReturn TibHash512(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds){
	tibHashState512 state;
	TibHashReturn status;
	status = TibInit512(&state);
	if (status != TIB_SUCCESS) return status;
	status = TibUpdate512(&state, data, databitlen, rounds);
	if (status != TIB_SUCCESS) return status;
	return _Final512(&state, hashval,TIB_HASH_BYTES_512, rounds);
}

/* TibHash the supplied data and provide the resulting hash value. Set return code as
 * appropriate. 384 bit version.
 * Parameters:
 *	data: the data to be hashed
 *	databitlen: the length, in bits, of the data to be hashed
 *	hashval: the resulting hash value of the provided data
 * Returns:
 *	TIB_SUCCESS - on success
*/
TibHashReturn TibHash384(const TibBitSequence *data, TibDataLength databitlen, TibBitSequence *hashval, const int rounds){
	tibHashState512 state;
	TibHashReturn status;
	status = TibInit384(&state);
	if (status != TIB_SUCCESS) return status;
	status = TibUpdate512(&state, data, databitlen, rounds);
	if (status != TIB_SUCCESS) return status;
	return _Final512(&state, hashval,TIB_HASH_BYTES_384, rounds);
}

